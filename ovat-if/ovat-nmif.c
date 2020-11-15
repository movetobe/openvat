#include "ovat-if.h"
#include "ovat-nmif.h"
#include "ovat-ctl.h"
#include "ovat-netsock-msg.h"
#include "Nm.h"
/*
static void
ovat_nmif_pass_start_up(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_PassiveStartUp(atoi(command_msg->argv[2])) != OVAT_EOK) {
        ovat_if_action_reply(fd, aux, "Nm_PassiveStartUp()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Nm_PassiveStartUp", OVAT_IF_ACTION_OK);
}
*/

static void
ovat_nmif_network_request(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_NetworkRequest(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_NetworkRequest()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_NetworkRequest()", OVAT_IF_ACTION_OK);
}

static void
ovat_nmif_network_release(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_NetworkRelease(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_NetworkRelease()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_NetworkRelease()", OVAT_IF_ACTION_OK);
}

void
ovat_nmif_command_register(void *aux)
{
    //ovat_ctl_command_register("nm/pass-start-up", "Nm_PassiveStartUp [NetworkHandle]",
    //                            1, 1, ovat_nmif_pass_start_up, aux);
    ovat_ctl_command_register("nm/network-request", "Nm_NetworkRequest [NetworkHandle]",
                                1, 1, ovat_nmif_network_request, aux);
    ovat_ctl_command_register("nm/network-release", "Nm_NetworkRelease [NetworkHandle]",
                                1, 1, ovat_nmif_network_release, aux);
}
