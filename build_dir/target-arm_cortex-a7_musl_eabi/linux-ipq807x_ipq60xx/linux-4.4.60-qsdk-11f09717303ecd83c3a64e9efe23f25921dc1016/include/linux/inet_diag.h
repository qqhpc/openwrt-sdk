#ifndef _INET_DIAG_H_
#define _INET_DIAG_H_ 1

#include <net/netlink.h>
#include <uapi/linux/inet_diag.h>

struct sock;
struct inet_hashinfo;
struct nlattr;
struct nlmsghdr;
struct sk_buff;
struct netlink_callback;

struct inet_diag_handler {
	void		(*dump)(struct sk_buff *skb,
				struct netlink_callback *cb,
				const struct inet_diag_req_v2 *r,
				struct nlattr *bc);

	int		(*dump_one)(struct sk_buff *in_skb,
				    const struct nlmsghdr *nlh,
				    const struct inet_diag_req_v2 *req);

	void		(*idiag_get_info)(struct sock *sk,
					  struct inet_diag_msg *r,
					  void *info);
	__u16		idiag_type;
	__u16		idiag_info_size;
};

struct inet_connection_sock;
int inet_sk_diag_fill(struct sock *sk, struct inet_connection_sock *icsk,
		      struct sk_buff *skb, const struct inet_diag_req_v2 *req,
		      struct user_namespace *user_ns,
		      u32 pid, u32 seq, u16 nlmsg_flags,
		      const struct nlmsghdr *unlh);
void inet_diag_dump_icsk(struct inet_hashinfo *h, struct sk_buff *skb,
			 struct netlink_callback *cb,
			 const struct inet_diag_req_v2 *r,
			 struct nlattr *bc);
int inet_diag_dump_one_icsk(struct inet_hashinfo *hashinfo,
			    struct sk_buff *in_skb, const struct nlmsghdr *nlh,
			    const struct inet_diag_req_v2 *req);

int inet_diag_bc_sk(const struct nlattr *_bc, struct sock *sk);

static inline size_t inet_diag_msg_attrs_size(void)
{
	return	  nla_total_size(1)  /* INET_DIAG_SHUTDOWN */
		+ nla_total_size(1)  /* INET_DIAG_TOS */
#if IS_ENABLED(CONFIG_IPV6)
		+ nla_total_size(1)  /* INET_DIAG_TCLASS */
		+ nla_total_size(1)  /* INET_DIAG_SKV6ONLY */
#endif
		+ nla_total_size(4)  /* INET_DIAG_MARK */
		+ nla_total_size(4); /* INET_DIAG_CLASS_ID */
}

extern int  inet_diag_register(const struct inet_diag_handler *handler);
extern void inet_diag_unregister(const struct inet_diag_handler *handler);
#endif /* _INET_DIAG_H_ */
