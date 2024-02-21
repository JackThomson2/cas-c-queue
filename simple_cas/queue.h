#include <stdatomic.h>
#include <stdbool.h>

typedef struct Queue {
    void* queue_items[50];
    int head;
    int tail;
} Queue;

// Function prototypes
void Setup_Queue(Queue *q);
bool Push_Queue(Queue *q, void *item);
void *Pop_Queue(Queue *q);
