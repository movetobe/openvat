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
    ds_put_format(&s, "Mode: %s, State: %s", nmModes[mode], nmStates[state]);
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
    ds_put_format(&s, "VendorID: 0x%02x, ModuleID: 0x%02x, Major: 0x%02x, Minor: 0x%02x",
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
    ovat_if_action_reply(fd, aux, "Call CanNm_SetSleepReadyBit()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_tx_confirmation(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    CanNm_TxConfirmation(atoi(command_msg->argv[2]), atoi(command_msg->argv[3]));
    ovat_if_action_reply(fd, aux, "Call CanNm_TxConfirmation()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_rx_indication(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 pdu[8] = {0};
    PduInfoType pduinfo;
    int i = 0;

    for (i = 0; i < 8; i++) {
        pdu[i] = strtol(command_msg->argv[3 + i], NULL, 16);
    }

    pduinfo.SduDataPtr = pdu;
    pduinfo.SduLength = 8;
    CanNm_RxIndication(atoi(command_msg->argv[2]), &pduinfo);
    ovat_if_action_reply(fd, aux, "Call CanNm_RxIndication()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_confirm_pnavail(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    CanNm_ConfirmPnAvailability(atoi(command_msg->argv[2]));
    ovat_if_action_reply(fd, aux, "Call CanNm_ConfirmPnAvailability()", OVAT_IF_ACTION_OK);
}

static void
ovat_cannmif_trigger_transmit(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 pdu[8] = {0};
    PduInfoType pduinfo;
    int i = 0;

    for (i = 0; i < 8; i++) {
        pdu[i] = strtol(command_msg->argv[3 + i], NULL, 16);
    }

    pduinfo.SduDataPtr = pdu;
    pduinfo.SduLength = 8;
    if (CanNm_TriggerTransmit(atoi(command_msg->argv[2]), &pduinfo) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call CanNm_TriggerTransmit()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call CanNm_TriggerTransmit()", OVAT_IF_ACTION_OK);
}

void
ovat_cannmif_command_register(void *aux)
{
    ovat_ctl_command_register("cannm/pass-start-up", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_pass_start_up, aux);
    ovat_ctl_command_register("cannm/network-request", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_network_request, aux);
    ovat_ctl_command_register("cannm/network-release", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_network_release, aux);
    ovat_ctl_command_register("cannm/disable-communication", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_disable_communication, aux);
    ovat_ctl_command_register("cannm/enable-communication", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_enable_communication, aux);
    ovat_ctl_command_register("cannm/set-userdata", "[nmChannelHandle] [UserData]",
                                7, 9, ovat_cannmif_set_userdata, aux);
    ovat_ctl_command_register("cannm/get-userdata", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_get_userdata, aux);
    ovat_ctl_command_register("cannm/transmit", "[nmChannelHandle] [PduInfo]",
                                9, 9, ovat_cannmif_transmit, aux);
    ovat_ctl_command_register("cannm/get-nodeid", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_get_nodeid, aux);
    ovat_ctl_command_register("cannm/get-localnodeid", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_get_localnodeid, aux);
    ovat_ctl_command_register("cannm/repeat-message-request", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_repeat_message_request, aux);
    ovat_ctl_command_register("cannm/get-pdudata", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_get_pdudata, aux);
    ovat_ctl_command_register("cannm/get-state", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_get_state, aux);
    ovat_ctl_command_register("cannm/get-versioninfo", "",
                                0, 0, ovat_cannmif_get_versioninfo, aux);
    ovat_ctl_command_register("cannm/request-bussync", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_request_bussync, aux);
    ovat_ctl_command_register("cannm/check-remotesleepind", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_check_remotesleepind, aux);
    ovat_ctl_command_register("cannm/set-sleepreadybit", "[nmChannelHandle]",
                                2, 2, ovat_cannmif_set_sleepreadybit, aux);
    ovat_ctl_command_register("cannm/tx-confirmation", "[TxPduId] [result]",
                                2, 2, ovat_cannmif_tx_confirmation, aux);
    ovat_ctl_command_register("cannm/rx-indication", "[RxPduId] [pdudata]",
                                9, 9, ovat_cannmif_rx_indication, aux);
    ovat_ctl_command_register("cannm/confirm-pnavail", "[nmChannelHandle]",
                                1, 1, ovat_cannmif_confirm_pnavail, aux);
    ovat_ctl_command_register("cannm/trigger-transmit", "[TxPduId] [pdudata]",
                                9, 9, ovat_cannmif_trigger_transmit, aux);
}

OVAT_LOG_REGISTER(cannmif_logtype, CANNMIF, INFO);

