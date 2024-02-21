#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

#include "queue.h"

const int LOOP_COUNT = 100000;
atomic_int write_fails = 0;
atomic_int read_fails = 0;

void* reader_thread(Queue* q) {
    for (int i = 0; i < LOOP_COUNT; i++) {
        int *pop_res = Pop_Queue(q);
        if (pop_res == NULL) {
            usleep(1);
            --i;

            read_fails++;
            continue;
        }

        printf("Pop res is: %d\n", *pop_res);
        free(pop_res);
    }

    return 0;
}

void* writer_thread(Queue* q) {
    for (int i = 0; i < LOOP_COUNT; i++) {
        int *tester = malloc(sizeof(int));
        *tester = i;

        if (!Push_Queue(q, tester)) {
            usleep(1);
            --i;

            write_fails++;
        }
    }

    return 0;
}

int queue_experiment() {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL) {
        perror("Couldn't allocate memory");
        return 1;
    }

    Setup_Queue(queue);

    pthread_t writer;
    pthread_create(&writer, NULL, writer_thread, queue);

    pthread_t reader;
    pthread_create(&reader, NULL, reader_thread, queue);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);


    printf("Total write fails: %d. Total read fails: %d\n", write_fails, read_fails);

    free(queue);
    return 0;
}

int main(void) {
    return queue_experiment();
}
