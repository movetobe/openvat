#include "ovat-utils.h"
#include "ovat-ring.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* size shall be pow of 2 */
struct ovat_ring *
ovat_ring_create(uint32_t size)
{
    struct ovat_ring *r = calloc(1, sizeof(struct ovat_ring));
    r->ring = calloc(1, sizeof(uint32_t) * size);
    r->size = size;
    return r;
}

void
ovat_ring_free(struct ovat_ring *r)
{
    free(r->ring);
    free(r);
}

int
ovat_ring_enqueue(struct ovat_ring *r, uint32_t *value, uint32_t count)
{
    uint32_t head, reserve_head;
    bool success;
    uint32_t free_entry;
    int i, ridx;

    do {
        head = r->prod_head;
        free_entry = (r->cons_tail - head - 1) % r->size;
        if (free_entry < count) {
            return -OVAT_ENOFREE;
        }
        reserve_head = head + count;
        success = __sync_bool_compare_and_swap(&(r->prod_head), head, reserve_head);
    } while (success != true);

    for (i = 0; i < count; i++) {
        ridx = (head + i) % r->size;
        r->ring[ridx] = value[i];
    }

    while (r->prod_tail != head);
    r->prod_tail = reserve_head;

    return OVAT_EOK;
}

int
ovat_ring_dequeue(struct ovat_ring *r, uint32_t *value, uint32_t count)
{
    uint32_t head, reserve_head;
    bool success;
    uint32_t full_entry;
    int i, ridx;

    /*  */
    do {
        head = r->cons_head;
        full_entry = (r->prod_tail - head) % r->size;
        if (full_entry < count) {
            return -OVAT_ENOFREE;
        }
        reserve_head = head + count;
        success = __sync_bool_compare_and_swap(&(r->cons_head), head, reserve_head);
    } while (success != true);

    for (i = 0; i < count; i++) {
        ridx = (head + i) % r->size;
        value[i] = r->ring[ridx];
    }

    while (r->cons_tail != head);
    r->cons_tail = reserve_head;

    return OVAT_EOK;
}
