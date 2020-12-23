//
// Created by 厉宇桐 on 2020/11/18.
//
#include <stdio.h>
#include "../common/queue.h"

int main() {
    queue_t queue;
    queue_init(&queue);
    queue_clear(&queue);
//    int * p1 = (int *)malloc(sizeof(int));
//    int * p2 = (int *)malloc(sizeof(int));
//    int * p3 = (int *)malloc(sizeof(int));
//    *p1 = 1;
//    *p2 = 2;
//    *p3 = 3;
    int p1 = 1;
    int p2 = 2;
    int p3 = 3;
    queue_push_back(&queue, (void *) &p1, sizeof(int));
    queue_push_back(&queue, (void *) &p2, sizeof(int));
    queue_push_back(&queue, (void *) &p3, sizeof(int));
    int res;
    queue_pop_front(&queue, (void *) &res);
    printf("Out: %d\n", res);
    queue_pop_front(&queue, (void *) &res);
    printf("Out: %d\n", res);
    queue_pop_front(&queue, (void *) &res);
    printf("Out: %d\n", res);
    fflush(stdout);

    queue_clear(&queue);
    return 0;

}

