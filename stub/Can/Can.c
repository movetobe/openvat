#include "netsock.h"
#include "netsock-conn.h"
#include "netsock-can-sock.h"
#include "ovat-log.h"
#include "ovat-list.h"
#include "ovat-utils.h"
#include "Can.h"
#include "CanIf.h"
#include "ovat-list.h"
#include "ovat-ring.h"
#include <linux/can.h>
#include <unistd.h>

struct vcan_channel {
    struct netsock *vcan_netsock;
    PduIdType swid;
    Can_HwHandleType hwid;
    struct list_head vcan_node;
};
static struct list_head vcan_channels_list;
static struct ovat_ring *tx_ring;
#define OVAT_TX_RING_SIZE 256
/* There is only one connection */
static int
can_get_sockfd(struct netsock *can_netsock)
{
    struct netsock_conn *conn = NULL;

    list_for_each_entry(conn, &(can_netsock->conn_list), conn_node) {
        if (conn->fd > 0) {
            return conn->fd;
        }
    }

    return -1;
}

static struct vcan_channel *
can_vchannel_lookup_by_name(const char *device)
{
    struct vcan_channel *vchannel = NULL;
    list_for_each_entry(vchannel, &vcan_channels_list, vcan_node) {
        if (!strcmp(vchannel->vcan_netsock->path, device)) {
            return vchannel;
        }
    }
    return NULL;
}

static struct vcan_channel *
can_vchannel_lookup_by_swid(Can_IdType swid)
{
    struct vcan_channel *vchannel = NULL;
    list_for_each_entry(vchannel, &vcan_channels_list, vcan_node) {
        if (vchannel->swid == swid) {
            return vchannel;
        }
    }
    return NULL;
}

static struct vcan_channel *
can_vchannel_lookup_by_hwid(Can_HwHandleType hwid)
{
    struct vcan_channel *vchannel = NULL;
    list_for_each_entry(vchannel, &vcan_channels_list, vcan_node) {
        if (vchannel->hwid == hwid) {
            return vchannel;
        }
    }
    return NULL;
}

static int
can_message_process(int fd, struct netsock *netsock_, void *msg)
{
    struct can_frame *frame = (struct can_frame *)msg;

    /* other ecu's message, indication to upper layer */
    return CanIf_RxIndication(frame->can_id, frame->can_dlc, frame->data);
}

int
can_create_vcan_dev(const char *device)
{
    int ret = OVAT_EOK;
    char command[128] = {0};

    if (can_vchannel_lookup_by_name(device) != NULL) {
        OVAT_LOG(INFO, CANSTUB, "vcan device has been created, name: %s", device);
        ret = -OVAT_EEXIST;
        goto err1;
    }

    /* system() is an unsafety way to execute following shell commands:
     * ip link add dev vcan0 type vcan
     * ip link set up vcan0
     */
    snprintf(command, sizeof(command), "ip link add dev %s type vcan", device);
    if (system(command) != 0) {
        OVAT_LOG(ERR, CANSTUB, "ip link add dev %s type vcan FAILED, errno: %u", device, errno);
        goto err1;
    }
    memset(command, 0, sizeof(command));
    snprintf(command, sizeof(command), "ip link set up %s", device);
    if (system(command) != 0) {
        OVAT_LOG(ERR, CANSTUB, "ip link set up %s FAILED, errno:%u", device, errno);
        goto err1;
    }

    struct vcan_channel *new_vchannel = calloc(1, sizeof(struct vcan_channel));
    ret =  netsock_open("virtual-can", NETSOCK_CONN_TYPE_LOOPBACK, device, "can_sock", &(new_vchannel->vcan_netsock));
    if (ret < 0) {
        OVAT_LOG(ERR, CANSTUB, "netsock open failed, path: %s, ret: %d\n", device, ret);
        goto err2;
    }
    new_vchannel->hwid = list_size(&vcan_channels_list);
    /* swid and hwid shall be configured, here just set the same */
    new_vchannel->swid = new_vchannel->hwid;
    list_add_tail(&(new_vchannel->vcan_node), &vcan_channels_list);

    return ret;

err2:
    free(new_vchannel);
err1:
    return ret;
}

int
can_remove_vcan_dev(const char *device)
{
    int ret = OVAT_EOK;
    char command[128] = {0};

    struct vcan_channel *vchannel = can_vchannel_lookup_by_name(device);

    if (vchannel == NULL) {
        OVAT_LOG(INFO, CANSTUB, "vcan device does not exist, name: %s", device);
        ret = -OVAT_ENOEXIST;
        goto err1;
    }

    /* system() is an unsafety way to execute following shell commands:
     * ip link delete vcan0
     */
    snprintf(command, sizeof(command), "ip link delete %s", device);
    if (system(command) != 0) {
        OVAT_LOG(ERR, CANSTUB, "ip link add dev %s type vcan FAILED, errno: %u", device, errno);
    }

    ret =  netsock_close(vchannel->vcan_netsock);
    if (ret < 0) {
        OVAT_LOG(ERR, CANSTUB, "netsock open failed, path: %s, ret: %d\n", device, ret);
    }

    list_del(&(vchannel->vcan_node));
    free(vchannel);
err1:
    return ret;
}

int
can_destroy_vcan_dev(void)
{
    int ret = OVAT_EOK;
    struct vcan_channel *vchannel, *tvchannel;

    list_for_each_entry_safe(vchannel, tvchannel, &vcan_channels_list, vcan_node) {
        can_remove_vcan_dev(vchannel->vcan_netsock->path);
    }

    return ret;
}

static int
can_write(struct vcan_channel *vchannel, uint32_t hwid, void *buffer, size_t length)
{
    struct can_frame frame;
    size_t count = sizeof(struct can_frame);
    int ret = 0;
    int sockfd = can_get_sockfd(vchannel->vcan_netsock);

    if (sockfd < 0) {
        OVAT_LOG(ERR, CANSTUB, "no socket to write, tx id: %u", hwid);
        return -OVAT_ENOEXIST;
    }

    memset(&frame, 0, count);
    frame.can_id = hwid;
    frame.can_dlc = length;
    memcpy(frame.data, buffer, length);

    ret = vchannel->vcan_netsock->class->send(sockfd, vchannel->vcan_netsock, &frame, count);
    if (ret < count) {
        OVAT_LOG(INFO, CANSTUB, "can socket send failed, expected: %u, only send: %u",
                        count, ret);
        return -OVAT_ECALL;
    }
    return OVAT_EOK;
}

Std_ReturnType
Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo)
{
    struct vcan_channel *vchannel = can_vchannel_lookup_by_hwid(Hth);
    uint32_t pduid = (uint32_t)(PduInfo->swPduHandle);

    if (vchannel == NULL) {
        OVAT_LOG(INFO, CANSTUB, "No free vcan channel to use");
        return E_NOT_OK;
    }

    if (can_write(vchannel, Hth, PduInfo->sdu, PduInfo->length) < 0) {
        OVAT_LOG(INFO, CANSTUB, "can virtual device write failed");
        return E_NOT_OK;
    }
    if (ovat_ring_enqueue(tx_ring, &pduid, 1) < 0) {
        OVAT_LOG(INFO, CANSTUB, "can tx ring enqueue failed");
        return E_NOT_OK;
    }
    return E_OK;
}

void
Can_Init(const Can_ConfigType *config)
{
    INIT_LIST_HEAD(&vcan_channels_list);
    int ret = OVAT_EOK;

    ret = netsock_can_sock_register(can_message_process);
    if (ret < 0) {
        OVAT_LOG(ERR, CANSTUB, "netsock unix sock register failed");
        return;
    }
    tx_ring = ovat_ring_create(OVAT_TX_RING_SIZE);
}

void Can_MainFunction(void)
{
    uint32_t pduid = 0;

    if (ovat_ring_dequeue(tx_ring, &pduid, 1) == OVAT_EOK) {
        CanIf_TxConfirmation((PduIdType)pduid, E_OK);
    }
}

OVAT_LOG_REGISTER(canstub_logtype, CANSTUB, INFO);

