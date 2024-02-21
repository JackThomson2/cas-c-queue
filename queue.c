#include <stdio.h>

#include "queue.h"

void Setup_Queue(Queue *q) {
    q->head = 0;
    q->tail = 0;
}

bool Push_Queue(Queue *q, void *item) {
    while (true) {
        int head = __atomic_load_n(&q->head, __ATOMIC_RELAXED);
        int tail = __atomic_load_n(&q->tail, __ATOMIC_RELAXED);

        int next_slot = (tail + 1) % 50;

        if (next_slot == head) {
            return false;
        }

        if (!__atomic_compare_exchange_n(&q->tail, &tail, next_slot, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            continue;
        }

        q->queue_items[tail] = item;
        return true;
    }
}

void *Pop_Queue(Queue *q) {
    while (true) {
        int head = __atomic_load_n(&q->head, __ATOMIC_RELAXED);
        int tail = __atomic_load_n(&q->tail, __ATOMIC_RELAXED);

        if (head == tail) {
            return NULL;
        }

        int next_slot = (head + 1) % 50;

        if (!__atomic_compare_exchange_n(&q->head, &head, next_slot, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            continue;
        }

        return q->queue_items[head];
    }
}
