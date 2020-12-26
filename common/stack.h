//
// Created by 厉宇桐 on 2020/11/15.
//

#ifndef LAC_STACK_H
#define LAC_STACK_H

#include <memory.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

typedef struct stack_node_t {
    void *pData;
    int iNumBytes;
    struct stack_node_t *pPrev;
} stack_node_t;

typedef struct lac_stack_t {
    int iLength;
    struct stack_node_t *pTop;
} lac_stack_t;

lac_stack_t *stack_create();

void stack_init(lac_stack_t *pStack);

int stack_is_empty(lac_stack_t *pStack);

int stack_push(lac_stack_t *pStack, void *pData, int iNumBytes);

int stack_pop(lac_stack_t *pStack, void **pData);

void stack_destroy(lac_stack_t *pStack);

void stack_clear(lac_stack_t *pStack);

#endif
