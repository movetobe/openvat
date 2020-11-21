#ifndef NETSOCK_UNIX_SOCK_H
#define NETSOCK_UNIX_SOCK_H
#include "netsock.h"

#define NETSOCK_UNIX_SOCK_MSG_BUF_LEN (1024)

int netsock_unix_sock_register(msg_handler_cb *cb);
void netsock_unix_sock_unregister(void);
#endif /* NETSOCK_UNIX_SOCK_H */