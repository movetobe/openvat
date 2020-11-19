#ifndef NETSOCK_UNIX_SOCK_H
#define NETSOCK_UNIX_SOCK_H
#include "netsock.h"

#define NETSOCK_UNIX_SOCK_MSG_BUF_LEN (1024)

int netsock_unix_sock_register(msg_handler_cb *cb);

#endif /* NETSOCK_UNIX_SOCK_H */