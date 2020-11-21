#include "ovat-if.h"
#include "ovat-cannmif.h"
#include "ovat-ctl.h"
#include "ovat-netsock-msg.h"
#include "dynamic-string.h"
#include "Nm.h"
#include "CanNm.h"

static void
ovat_cannmif_pass_start_up(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_PassiveStartUp(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_PassiveStartUp()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_PassiveStartUp()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_network_request(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_NetworkRequest(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_NetworkRequest()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_NetworkRequest()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_network_release(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_NetworkRelease(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_NetworkRelease()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_NetworkRelease()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_disable_communication(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_DisableCommunication(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_DisableCommunication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_DisableCommunication()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_enable_communication(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_EnableCommunication(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_EnableCommunication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_EnableCommunication()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_set_userdata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 userdata[8] = {0};
    int i = 0;

    for (i = 0; i < 8; i++) {
        userdata[i] = strtol(command_msg->argv[3 + i], NULL, 16);
    }

    if (CanNm_SetUserData(atoi(command_msg->argv[2]), (const uint8 *)userdata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_SetUserData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, userdata 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s\n",
                                    command_msg->argv[2], command_msg->argv[3],
                                    command_msg->argv[4], command_msg->argv[5],
                                    command_msg->argv[6], command_msg->argv[7],
                                    command_msg->argv[8], command_msg->argv[9],
                                    command_msg->argv[10]);
    ovat_if_action_reply(fd, aux, "Call CanNm_SetUserData()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_get_userdata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 userdata[8] = {0};
    struct ds s;

    if (CanNm_GetUserData(atoi(command_msg->argv[2]), userdata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_GetUserData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }

    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, userdata 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                command_msg->argv[2], userdata[0], userdata[1], userdata[2], userdata[3],
                userdata[4], userdata[5], userdata[6], userdata[7]);
    ds_init(&s);
    ds_put_format(&s, "userdata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                    userdata[0], userdata[1], userdata[2], userdata[3],
                    userdata[4], userdata[5], userdata[6], userdata[7]);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetUserData()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_transmit(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    PduInfoType pduInfo = {0, 0, 0};
    uint8 pdu[8] = {0};
    pduInfo.SduDataPtr = pdu;
    int i = 0;

    for (i = 0; i < 8; i++) {
        pdu[i] = strtol(command_msg->argv[3 + i], NULL, 16);
    }
    pduInfo.SduLength = 8;

    if (CanNm_Transmit(atoi(command_msg->argv[2]), &pduInfo) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_Transmit()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    OVAT_LOG(INFO, CANNMIF, "TxPduId %s, userdata 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s 0x%s\n",
                                    command_msg->argv[2], command_msg->argv[3],
                                    command_msg->argv[4], command_msg->argv[5],
                                    command_msg->argv[6], command_msg->argv[7],
                                    command_msg->argv[8], command_msg->argv[9],
                                    command_msg->argv[10]);
    ovat_if_action_reply(fd, aux, "Call CanNm_Transmit()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_get_nodeid(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8_t nodeid = 0;
    struct ds s;

    if (CanNm_GetNodeIdentifier(atoi(command_msg->argv[2]), (uint8 *)&nodeid) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_GetNodeIdentifier()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "nodeid: %u", nodeid);
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, nodeid 0x%x\n", command_msg->argv[2], nodeid);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetNodeIdentifier()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_get_localnodeid(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8_t nodeid = 0;
    struct ds s;

    if (CanNm_GetLocalNodeIdentifier(atoi(command_msg->argv[2]), (uint8 *)&nodeid) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_GetLocalNodeIdentifier()", OVAT_IF_ACTION_NOT_OK);
        return;
    }

    ds_init(&s);
    ds_put_format(&s, "nodeid: %u", nodeid);
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, nodeid 0x%x\n", command_msg->argv[2], nodeid);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetLocalNodeIdentifier()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_repeat_message_request(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_RepeatMessageRequest(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_RepeatMessageRequest()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_RepeatMessageRequest()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_get_pdudata(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 pdudata[8] = {0};
    struct ds s;

    if (CanNm_GetPduData(atoi(command_msg->argv[2]), pdudata) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_GetPduData()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, pdudata 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                    command_msg->argv[2], pdudata[0], pdudata[1], pdudata[2], pdudata[3],
                    pdudata[4], pdudata[5], pdudata[6], pdudata[7]);
    ds_init(&s);
    ds_put_format(&s, "pdudata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                pdudata[0], pdudata[1], pdudata[2], pdudata[3],
                pdudata[4], pdudata[5], pdudata[6], pdudata[7]);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetPduData()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_get_state(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    Nm_StateType state = NM_STATE_BUS_SLEEP;
    Nm_ModeType mode = NM_MODE_BUS_SLEEP;
    struct ds s;

    if (CanNm_GetState(atoi(command_msg->argv[2]), &state, &mode) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_GetState()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "Mode: %u, State: %u", mode, state);
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, Mode %u, State %u\n", command_msg->argv[2], mode, state);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetState()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_get_versioninfo(int fd, void *msg, void *aux)
{
    Std_VersionInfoType version;
    struct ds s;

    CanNm_GetVersionInfo(&version);
    ds_init(&s);
    ds_put_format(&s, "VendorID: %u, ModuleID: %u, Major: %u, Minor: %u",
            version.vendorID, version.moduleID, version.sw_major_version, version.sw_minor_version);
    OVAT_LOG(INFO, CANNMIF, "VendorID: %u, ModuleID: %u, Major: %u, Minor: %u",
            version.vendorID, version.moduleID, version.sw_major_version, version.sw_minor_version);
    ovat_if_action_reply(fd, aux, "Call CanNm_GetVersionInfo()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_request_bussync(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_RequestBusSynchronization(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_RequestBusSynchronization()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_RequestBusSynchronization()", OVAT_IF_ACTION_OK);
}


static void
ovat_cannmif_check_remotesleepind(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    boolean ind = 0;
    struct ds s;

    if (CanNm_CheckRemoteSleepIndication(atoi(command_msg->argv[2]), (boolean *)&ind) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_CheckRemoteSleepIndication()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "ind: %u", ind);
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, Sleep Indication %u\n", command_msg->argv[2], ind);
    ovat_if_action_reply(fd, aux, "Call CanNm_CheckRemoteSleepIndication()", s.string);
    ds_destroy(&s);
}

static void
ovat_cannmif_set_sleepreadybit(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (CanNm_SetSleepReadyBit(atoi(command_msg->argv[2]), (boolean)atoi(command_msg->argv[3])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_SetSleepReadyBit()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    OVAT_LOG(INFO, CANNMIF, "NetworkHandle %s, sleep ready bit %s",
                                    command_msg->argv[2], command_msg->argv[3]);
    ovat_if_action_reply(fd, aux, "Call CanNm_SetSleepReadyBit()", OVAT_IF_ACTION_OK);
}

void
ovat_cannmif_command_register(void *aux)
{
    ovat_ctl_command_register("cannm/pass-start-up", "CanNm_PassiveStartUp [nmChannelHandle]",
                                1, 1, ovat_cannmif_pass_start_up, aux);
    ovat_ctl_command_register("cannm/network-request", "CanNm_NetworkRequest [nmChannelHandle]",
                                1, 1, ovat_cannmif_network_request, aux);
    ovat_ctl_command_register("cannm/network-release", "CanNm_NetworkRelease [nmChannelHandle]",
                                1, 1, ovat_cannmif_network_release, aux);
    ovat_ctl_command_register("cannm/disable-communication", "CanNm_DisableCommunication [nmChannelHandle]",
                                1, 1, ovat_cannmif_disable_communication, aux);
    ovat_ctl_command_register("cannm/enable-communication", "CanNm_EnableCommunication [nmChannelHandle]",
                                1, 1, ovat_cannmif_enable_communication, aux);
    ovat_ctl_command_register("cannm/set-userdata", "CanNm_SetUserData [nmChannelHandle] [UserData]",
                                7, 9, ovat_cannmif_set_userdata, aux);
    ovat_ctl_command_register("cannm/get-userdata", "CanNm_GetUserData [nmChannelHandle]",
                                1, 1, ovat_cannmif_get_userdata, aux);
    ovat_ctl_command_register("cannm/transmit", "CanNm_Transmit [nmChannelHandle] [PduInfo]",
                                9, 9, ovat_cannmif_transmit, aux);
    ovat_ctl_command_register("cannm/get-nodeid", "CanNm_GetNodeIdentifier [nmChannelHandle]",
                                1, 1, ovat_cannmif_get_nodeid, aux);
    ovat_ctl_command_register("cannm/get-localnodeid", "CanNm_GetLocalNodeIdentifier [nmChannelHandle]",
                                1, 1, ovat_cannmif_get_localnodeid, aux);
    ovat_ctl_command_register("cannm/repeat-message-request", "CanNm_RepeatMessageRequest [nmChannelHandle]",
                                1, 1, ovat_cannmif_repeat_message_request, aux);
    ovat_ctl_command_register("cannm/get-pdudata", "CanNm_GetPduData [nmChannelHandle]",
                                1, 1, ovat_cannmif_get_pdudata, aux);
    ovat_ctl_command_register("cannm/get-state", "CanNm_GetState [nmChannelHandle]",
                                1, 1, ovat_cannmif_get_state, aux);
    ovat_ctl_command_register("cannm/get-versioninfo", "CanNm_GetVersionInfo",
                                0, 0, ovat_cannmif_get_versioninfo, aux);
    ovat_ctl_command_register("cannm/request-bussync", "CanNm_RequestBusSynchronization [nmChannelHandle]",
                                1, 1, ovat_cannmif_request_bussync, aux);
    ovat_ctl_command_register("cannm/check-remotesleepind", "CanNm_CheckRemoteSleepIndication [nmChannelHandle]",
                                1, 1, ovat_cannmif_check_remotesleepind, aux);
    ovat_ctl_command_register("cannm/set-sleepreadybit", "CanNm_SetSleepReadyBit [nmChannelHandle]",
                                2, 2, ovat_cannmif_set_sleepreadybit, aux);
}

OVAT_LOG_REGISTER(cannmif_logtype, ovat.cannmif, INFO);

