#ifndef OVAT_NETSOCK_MSG_H
#define OVAT_NETSOCK_MSG_H

#include "netsock-unix-sock.h"

#define OVAT_NETSOCK_MSG_ACK "OVAT_NETSOCK_MSG_ACK"

#define OVAT_NETSOCK_MSG_MAX_ARGC 6
#define OVAT_NETSOCK_MSG_MAX_ARGV_LEN 64
#define OVAT_NETSOCK_MSG_RESV (NETSOCK_UNIX_SOCK_MSG_BUF_LEN \
                               - sizeof(int) \
                               - (OVAT_NETSOCK_MSG_MAX_ARGC * OVAT_NETSOCK_MSG_MAX_ARGV_LEN))

struct ovat_netsock_msg {
    int argc;
    char argv[OVAT_NETSOCK_MSG_MAX_ARGC][OVAT_NETSOCK_MSG_MAX_ARGV_LEN];
    char reserve[OVAT_NETSOCK_MSG_RESV];
};

#endif /* OVAT_NETSOCK_MSG_H */
