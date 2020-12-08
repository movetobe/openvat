#include "ovat-if.h"
#include "ovat-j1939nmif.h"
#include "ovat-ctl.h"
#include "ovat-netsock-msg.h"
#include "dynamic-string.h"
#include "Nm.h"
#include "J1939Nm.h"

static void
ovat_j1939nmif_pass_start_up(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (J1939Nm_PassiveStartUp(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call J1939Nm_PassiveStartUp()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call J1939Nm_PassiveStartUp()", OVAT_IF_ACTION_OK);
}

static void
ovat_j1939nmif_network_request(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (J1939Nm_NetworkRequest(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call J1939Nm_NetworkRequest()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call J1939Nm_NetworkRequest()", OVAT_IF_ACTION_OK);
}

static void
ovat_j1939nmif_network_release(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    if (J1939Nm_NetworkRelease(atoi(command_msg->argv[2])) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call J1939Nm_NetworkRelease()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ovat_if_action_reply(fd, aux, "Call J1939Nm_NetworkRelease()", OVAT_IF_ACTION_OK);
}

static void
ovat_j1939nmif_get_state(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    Nm_StateType state = NM_STATE_BUS_SLEEP;
    Nm_ModeType mode = NM_MODE_BUS_SLEEP;
    struct ds s;

    if (J1939Nm_GetState(atoi(command_msg->argv[2]), &state, &mode) != E_OK) {
        ovat_if_action_reply(fd, aux, "Call J1939Nm_GetState()", OVAT_IF_ACTION_NOT_OK);
        return;
    }
    ds_init(&s);
    ds_put_format(&s, "Mode: %s, State: %s", nmModes[mode], nmStates[state]);
    OVAT_LOG(INFO, J1939NMIF, "NetworkHandle: %s, Mode: %s, State: %s\n", command_msg->argv[2], nmModes[mode], nmStates[state]);
    ovat_if_action_reply(fd, aux, "Call J1939Nm_GetState()", s.string);
    ds_destroy(&s);
}

static void
ovat_j1939nmif_get_versioninfo(int fd, void *msg, void *aux)
{
    Std_VersionInfoType version;
    struct ds s;

    J1939Nm_GetVersionInfo(&version);
    ds_init(&s);
    ds_put_format(&s, "VendorID: 0x%02x, ModuleID: 0x%02x, Major: 0x%02x, Minor: 0x%02x",
            version.vendorID, version.moduleID, version.sw_major_version, version.sw_minor_version);
    OVAT_LOG(INFO, J1939NMIF, "VendorID: 0x%02x, ModuleID: 0x%02x, Major: 0x%02x, Minor: 0x%02x",
            version.vendorID, version.moduleID, version.sw_major_version, version.sw_minor_version);
    ovat_if_action_reply(fd, aux, "Call J1939Nm_GetVersionInfo()", s.string);
    ds_destroy(&s);
}

static void
ovat_j1939nmif_get_busoffdelay(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    uint8 delayCyclesPtr = 0;
    struct ds s;

    J1939Nm_GetBusOffDelay(atoi(command_msg->argv[2]), &delayCyclesPtr);

    ds_init(&s);
    ds_put_format(&s, "delayCyclesPtr: %u", delayCyclesPtr);
    ovat_if_action_reply(fd, aux, "Call J1939Nm_GetBusOffDelay()", s.string);
    ds_destroy(&s);
}

static void
ovat_j1939nmif_tx_confirmation(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;

    J1939Nm_TxConfirmation(atoi(command_msg->argv[2]), atoi(command_msg->argv[3]));
    ovat_if_action_reply(fd, aux, "Call J1939Nm_TxConfirmation()", OVAT_IF_ACTION_OK);
}

static void
ovat_j1939nmif_rx_indication(int fd, void *msg, void *aux)
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
    J1939Nm_RxIndication(atoi(command_msg->argv[2]), &pduinfo);
    ovat_if_action_reply(fd, aux, "Call J1939Nm_RxIndication()", OVAT_IF_ACTION_OK);
}

static void
ovat_j1939nmif_request_ind(int fd, void *msg, void *aux)
{
    struct ovat_netsock_msg *command_msg = (struct ovat_netsock_msg *)msg;
    J1939Rm_ExtIdInfoType extIdInfo;
    uint8 node, sourceAddress, destAddress, priority;
    uint32 requestedPgn;
    NetworkHandleType channel;

    node = atoi(command_msg->argv[2]);
    channel = atoi(command_msg->argv[3]);
    requestedPgn = atoi(command_msg->argv[4]);
    extIdInfo.extIdType = atoi(command_msg->argv[5]);
    extIdInfo.extId1 = atoi(command_msg->argv[6]);
    extIdInfo.extId2 = atoi(command_msg->argv[7]);
    extIdInfo.extId3 = atoi(command_msg->argv[8]);
    sourceAddress = atoi(command_msg->argv[9]);
    destAddress = atoi(command_msg->argv[10]);
    priority = atoi(command_msg->argv[11]);
    
    J1939Nm_RequestIndication(node, channel, requestedPgn, &extIdInfo,
                                sourceAddress, destAddress, priority);

    ovat_if_action_reply(fd, aux, "Call J1939Nm_RequestIndication()", OVAT_IF_ACTION_OK);
}

void
ovat_j1939nmif_command_register(void *aux)
{
    ovat_ctl_command_register("j1939nm/pass-start-up", "[nmChannelHandle]",
                                1, 1, ovat_j1939nmif_pass_start_up, aux);
    ovat_ctl_command_register("j1939nm/network-request", "[nmChannelHandle]",
                                1, 1, ovat_j1939nmif_network_request, aux);
    ovat_ctl_command_register("j1939nm/network-release", "[nmChannelHandle]",
                                1, 1, ovat_j1939nmif_network_release, aux);
    ovat_ctl_command_register("j1939nm/get-state", "[nmChannelHandle]",
                                1, 1, ovat_j1939nmif_get_state, aux);
    ovat_ctl_command_register("j1939nm/get-versioninfo", "",
                                0, 0, ovat_j1939nmif_get_versioninfo, aux);
    ovat_ctl_command_register("j1939nm/get_busoffdelay", "[nmChannelHandle]",
                                1, 1, ovat_j1939nmif_get_busoffdelay, aux);
     ovat_ctl_command_register("j1939nm/tx-confirmation", "[TxPduId] [result]",
                                2, 2, ovat_j1939nmif_tx_confirmation, aux);
    ovat_ctl_command_register("j1939nm/rx-indication", "[RxPduId] [pdudata]",
                                9, 9, ovat_j1939nmif_rx_indication, aux);
    ovat_ctl_command_register("j1939nm/request_ind", "[node] [channel] [requestedPgn] \
[extIdInfo.extIdType] [extIdInfo.extId1] [extIdInfo.extId2] [extIdInfo.extId3] \
[sourceAddress] [destAddress] [priority]",
                                10, 10, ovat_j1939nmif_request_ind, aux);
}

OVAT_LOG_REGISTER(j1939nmif_logtype, J1939NMIF, INFO);

