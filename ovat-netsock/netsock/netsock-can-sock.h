#ifndef NETSOCK_CAN_SOCK_H
#define NETSOCK_CAN_SOCK_H
#include "netsock.h"

#define NETSOCK_CAN_SOCK_MSG_BUF_LEN (1024)

int netsock_can_sock_register(msg_handler_cb *cb);
void netsock_can_sock_unregister(void);
#endif /* NETSOCK_CAN_SOCK_H */

