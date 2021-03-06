#include "ovat-if.h"
#include "ovat-nmif.h"
#include "ovat-ctl.h"
#include "ovat-netsock-msg.h"
#include "dynamic-string.h"
#include "Nm.h"

static void
ovat_nmif_pass_start_up(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_PassiveStartUp(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_PassiveStartUp()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_PassiveStartUp()", OVAT_IF_ACTION_OK);
}

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

static void
ovat_nmif_disable_communication(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_DisableCommunication(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_DisableCommunication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_DisableCommunication()", OVAT_IF_ACTION_OK);
}

static void
ovat_nmif_enable_communication(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_EnableCommunication(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_EnableCommunication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_EnableCommunication()", OVAT_IF_ACTION_OK);
}

static void
ovat_nmif_set_userdata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 userdata[8] = {0};
    int i = 0;

    for (i = 0; i < 8; i++) {
        userdata[i] = strtol(command_msg->argv[3 + i], NULL, 16);
    }

    if (Nm_SetUserData(atoi(command_msg->argv[2]), (const uint8 *)userdata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_SetUserData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_SetUserData()", OVAT_IF_ACTION_OK);
}

static void
ovat_nmif_get_userdata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 userdata[8] = {0};
    struct ds s;

    if (Nm_GetUserData(atoi(command_msg->argv[2]), userdata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_GetUserData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }

    ds_init(&s);
    ds_put_format(&s, "userdata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                    userdata[0], userdata[1], userdata[2], userdata[3],
                    userdata[4], userdata[5], userdata[6], userdata[7]);
    ovat_if_action_reply(fd, aux, "Call Nm_GetUserData()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_get_pdudata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 pdudata[8] = {0};
    struct ds s;

    if (Nm_GetPduData(atoi(command_msg->argv[2]), pdudata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_GetPduData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "pdudata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                pdudata[0], pdudata[1], pdudata[2], pdudata[3],
                pdudata[4], pdudata[5], pdudata[6], pdudata[7]);
    ovat_if_action_reply(fd, aux, "Call Nm_GetPduData()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_repeat_message_request(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (Nm_RepeatMessageRequest(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_RepeatMessageRequest()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call Nm_RepeatMessageRequest()", OVAT_IF_ACTION_OK);
}

static void
ovat_nmif_get_nodeid(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8_t nodeid = 0;
    struct ds s;

    if (Nm_GetNodeIdentifier(atoi(command_msg->argv[2]), (uint8 *)&nodeid) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_GetNodeIdentifier()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "nodeid: %u", nodeid);
    ovat_if_action_reply(fd, aux, "Call Nm_GetNodeIdentifier()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_get_localnodeid(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8_t nodeid = 0;
    struct ds s;

    if (Nm_GetLocalNodeIdentifier(atoi(command_msg->argv[2]), (uint8 *)&nodeid) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_GetLocalNodeIdentifier()", OVAT_IF_ACTION_NOT_OK);
        return;
    }

    ds_init(&s);
    ds_put_format(&s, "nodeid: %u", nodeid);
    ovat_if_action_reply(fd, aux, "Call Nm_GetLocalNodeIdentifier()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_check_remotesleepind(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    boolean ind = 0;
    struct ds s;

    if (Nm_CheckRemoteSleepIndication(atoi(command_msg->argv[2]), (boolean *)&ind) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_CheckRemoteSleepIndication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "ind: %u", ind);
    ovat_if_action_reply(fd, aux, "Call Nm_CheckRemoteSleepIndication()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_get_state(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    Nm_StateType state = NM_STATE_BUS_SLEEP;
    Nm_ModeType mode = NM_MODE_BUS_SLEEP;
    struct ds s;

    if (Nm_GetState(atoi(command_msg->argv[2]), &state, &mode) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call Nm_GetState()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "Mode: %s, State: %s", nmModes[mode], nmStates[state]);
    ovat_if_action_reply(fd, aux, "Call Nm_GetState()", s.string);
    ds_destroy(&s);
}

static void
ovat_nmif_get_versioninfo(int fd, void *msg, void *aux)
{
    Std_VersionInfoType version;
    struct ds s;

    Nm_GetVersionInfo(&version);
    ds_init(&s);
    ds_put_format(&s, "VendorID: 0x%02x, ModuleID: 0x%02x, Major: 0x%02x, Minor: 0x%02x",
            version.vendorID, version.moduleID, version.sw_major_version, version.sw_minor_version);
    ovat_if_action_reply(fd, aux, "Call Nm_GetVersionInfo()", s.string);
    ds_destroy(&s);
}

void
ovat_nmif_command_register(void *aux)
{
    ovat_ctl_command_register("nm/pass-start-up", "[NetworkHandle]",
                                1, 1, ovat_nmif_pass_start_up, aux);
    ovat_ctl_command_register("nm/network-request", "[NetworkHandle]",
                                1, 1, ovat_nmif_network_request, aux);
    ovat_ctl_command_register("nm/network-release", "[NetworkHandle]",
                                1, 1, ovat_nmif_network_release, aux);
    ovat_ctl_command_register("nm/disable-communication", "[NetworkHandle]",
                                1, 1, ovat_nmif_disable_communication, aux);
    ovat_ctl_command_register("nm/enable-communication", "[NetworkHandle]",
                                1, 1, ovat_nmif_enable_communication, aux);
    ovat_ctl_command_register("nm/set-userdata", "[NetworkHandle] [UserData]",
                                7, 9, ovat_nmif_set_userdata, aux);
    ovat_ctl_command_register("nm/get-userdata", "[NetworkHandle]",
                                1, 1, ovat_nmif_get_userdata, aux);
    ovat_ctl_command_register("nm/get-pdudata", "[NetworkHandle]",
                                1, 1, ovat_nmif_get_pdudata, aux);
    ovat_ctl_command_register("nm/repeat-message-request", "[NetworkHandle]",
                                1, 1, ovat_nmif_repeat_message_request, aux);
    ovat_ctl_command_register("nm/get-nodeid", "[NetworkHandle]",
                                1, 1, ovat_nmif_get_nodeid, aux);
    ovat_ctl_command_register("nm/get-localnodeid", "[NetworkHandle]",
                                1, 1, ovat_nmif_get_localnodeid, aux);
    ovat_ctl_command_register("nm/check-remotesleepind", "[NetworkHandle]",
                                1, 1, ovat_nmif_check_remotesleepind, aux);
    ovat_ctl_command_register("nm/get-state", "[NetworkHandle]",
                                1, 1, ovat_nmif_get_state, aux);
    ovat_ctl_command_register("nm/get-versioninfo", "",
                                0, 0, ovat_nmif_get_versioninfo, aux);
}

OVAT_LOG_REGISTER(nmif_logtype, NMIF, INFO);

