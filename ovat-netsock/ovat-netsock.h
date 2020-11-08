#ifndef OVAT_NETSOCK_H
#define OVAT_NETSOCK_H

#include "netsock.h"

void ovat_netsock_msg_ack(int fd, struct netsock *netsock_);
void ovat_netsock_msg_reply(int fd, struct netsock *netsock_, void *msg);

int ovat_netsock_create(char *name, int conn_type,
                        char *path, struct netsock **netsockp,
                        msg_handler_cb *cb);
void ovat_netsock_run(struct netsock *netsock_);
int ovat_netsock_destroy(struct netsock *netsock_);

#endif /* OVAT_NETSOCK_H */
