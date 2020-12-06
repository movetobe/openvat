#ifndef OVAT_RING_H
#define OVAT_RING_H

#include <stdint.h>
#include <stdlib.h>

struct ovat_ring {
    uint32_t *ring;
    uint32_t size;
    uint32_t prod_head;
    uint32_t prod_tail;
    uint32_t cons_head;
    uint32_t cons_tail;
};

struct ovat_ring *ovat_ring_create(uint32_t size);
void ovat_ring_free(struct ovat_ring *r);
int ovat_ring_enqueue(struct ovat_ring *r, uint32_t *value, uint32_t count);
int ovat_ring_dequeue(struct ovat_ring *r, uint32_t *value, uint32_t count);

#endif /* OVAT_RING_H */
