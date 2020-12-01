#include "ovat-if.h"
#include "ovat-ctl.h"
#include "ovat-netsock-msg.h"
#include "dynamic-string.h"
#include "Can.h"
#include "ovat-canif.h"

static void
ovat_canif_create_vcan_dev(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    can_create_vcan_dev(command_msg->argv[2]);
    ovat_if_action_reply(fd, aux, "can_set_vcan_dev", OVAT_IF_ACTION_OK);
}

void
ovat_canif_command_register(void *aux)
{
    ovat_ctl_command_register("can/create-vcan-dev", "[name]",
                                1, 1, ovat_canif_create_vcan_dev, aux);
}

OVAT_LOG_REGISTER(canif_logtype, CANIF, INFO);

