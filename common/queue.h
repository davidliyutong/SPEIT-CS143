//
// Created by 厉宇桐 on 2020/11/15.
//
/**@file  queue.h
* @brief    Types related to lac_queue
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/
#ifndef LAC_QUEUE_H
#define LAC_QUEUE_H

#include <memory.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

/** @struct
 * Node of lac_queue
 */
typedef struct queue_node_t {
    void *pData; /// The data pointer, type: void
    int iNumBytes; /// The size of data
    struct queue_node_t *pPrev; /// The pointer to previously node
    struct queue_node_t *pNext; /// The pointer to next node
} queue_node_t;

/** @struct
 * Master node of lac_queue
 * @remarks - If the stack is empty, pFront==pRear==NULL. If the stack contains 1 element, then pFront==pRear
 *          - We have queue.pFront->pPrev = queue.pFront and queue.pRear->pNext = queue.pRear
 */
typedef struct lac_queue_t {
    int iLength; /// The queue length counter
    struct queue_node_t *pFront; /// The queue front pointer
    struct queue_node_t *pRear; /// The queue rear pointer
} lac_queue_t;

lac_queue_t *queue_create();

void queue_init(lac_queue_t *pQueue);

bool queue_is_empty(lac_queue_t *pQueue);

bool queue_has_node(lac_queue_t *pQueue, struct queue_node_t *pNode);

bool queue_push_front(lac_queue_t *pQueue, const void *pData, int iNumBytes);

int queue_push_back(lac_queue_t *pQueue, void *pData, int iNumBytes);

int queue_pop_front(lac_queue_t *pQueue, void *pData, bool bFreeData);

int queue_pop_back(lac_queue_t *pQueue, void *pData, bool bFreeData);

int queue_del(lac_queue_t *pQueue, struct queue_node_t *pNode, void *pData, bool Check, bool bFreeData);

void queue_destroy(lac_queue_t *pQueue);

void queue_clear(lac_queue_t *pQueue);

void queue_next(queue_node_t **ppNode);

void queue_prev(queue_node_t **ppNode);

#endif
