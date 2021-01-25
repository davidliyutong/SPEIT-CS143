//
// Created by 厉宇桐 on 2020/11/15.
//
/**@file  stack.h
* @brief    Types related to lac_stack
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/
#ifndef LAC_STACK_H
#define LAC_STACK_H

#include <memory.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

/** @struct
 * Node of lac_stack
 */
typedef struct stack_node_t {
    void *pData; /// The data pointer, type: void
    int iNumBytes; /// The size of data
    struct stack_node_t *pPrev; /// The pointer to the previously pushed node
} stack_node_t;

/** @struct
 * Master node of lac_stack
 */
typedef struct lac_stack_t {
    int iLength; /// The stack length counter
    struct stack_node_t *pTop; /// The stack top pointer
} lac_stack_t;

lac_stack_t *stack_create();

void stack_init(lac_stack_t *pStack);

bool stack_is_empty(lac_stack_t *pStack);

bool stack_push(lac_stack_t *pStack, void *pData, int iNumBytes);

bool stack_pop(lac_stack_t *pStack, void *pData);

void stack_destroy(lac_stack_t *pStack);

void stack_clear(lac_stack_t *pStack);

#endif
