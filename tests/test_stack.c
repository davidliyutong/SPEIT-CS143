//
// Created by 厉宇桐 on 2020/11/18.
//
#include <stdio.h>
#include "../common/stack.h"

int main() {
    stack_t queue;
    stack_init(&queue);
    stack_clear(&queue);
//    int * p1 = (int *)malloc(sizeof(int));
//    int * p2 = (int *)malloc(sizeof(int));
//    int * p3 = (int *)malloc(sizeof(int));
//    *p1 = 1;
//    *p2 = 2;
//    *p3 = 3;
    int p1 = 1;
    int p2 = 2;
    int p3 = 3;
    stack_push(&queue, (void *) &p1, sizeof(int));
    stack_push(&queue, (void *) &p2, sizeof(int));
    stack_push(&queue, (void *) &p3, sizeof(int));
    int res;
    stack_pop(&queue, (void *) &res);
    printf("Out: %d\n", res);
    stack_pop(&queue, (void *) &res);
    printf("Out: %d\n", res);
    stack_pop(&queue, (void *) &res);
    printf("Out: %d\n", res);
    fflush(stdout);

    stack_clear(&queue);
    return 0;

}

