#include <stdio.h>

#include "queue.h"

void Setup_Queue(Queue *q) {
    q->head = 0;
    q->tail = 0;
}

bool Push_Queue(Queue *q, void *item) {
    while (true) {
        int head = __atomic_load_n(&q->head, __ATOMIC_SEQ_CST);
        int tail = __atomic_load_n(&q->tail, __ATOMIC_SEQ_CST);

        int next_slot = (tail + 1) % 50;

        if (next_slot == head) {
            return false;
        }

        int expected = tail;
        if (!__atomic_compare_exchange_n(&q->tail, &expected, next_slot, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            continue;
        }

        __atomic_store_n(&q->queue_items[tail], item, __ATOMIC_SEQ_CST);
        return true;
    }
}

void *Pop_Queue(Queue *q) {
    while (true) {
        int head = __atomic_load_n(&q->head, __ATOMIC_SEQ_CST);
        int tail = __atomic_load_n(&q->tail, __ATOMIC_SEQ_CST);

        if (head == tail) {
            return NULL;
        }

        int expected = head;
        int next_slot = (head + 1) % 50;

        if (!__atomic_compare_exchange_n(&q->head, &expected, next_slot, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
            continue;
        }

        // We need a spin loop here to wait for the write to complete (they could be a little slow)
        while (true) {
            void* res = __atomic_exchange_n(&q->queue_items[head], NULL, __ATOMIC_SEQ_CST);
            if (res != NULL) {
                return res;
            }
        }
    }
}
