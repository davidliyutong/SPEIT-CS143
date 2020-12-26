//
// Created by 厉宇桐 on 2020/11/15.
//

#ifndef LAC_QUEUE_H
#define LAC_QUEUE_H

#include <memory.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

typedef struct queue_node_t {
    void *pData;
    int iNumBytes;
    struct queue_node_t *pPrev;
    struct queue_node_t *pNext;
} queue_node_t;

typedef struct lac_queue_t {
    int iLength;
    struct queue_node_t *pFront;
    struct queue_node_t *pRear;
} lac_queue_t;

lac_queue_t *queue_create();

void queue_init(lac_queue_t *pQueue);

int queue_is_empty(lac_queue_t *pQueue);

int queue_has_node(lac_queue_t *pQueue, struct queue_node_t *pNode);

int queue_push_front(lac_queue_t *pQueue, const void *pData, int iNumBytes);

int queue_push_back(lac_queue_t *pQueue, void *pData, int iNumBytes);

int queue_pop_front(lac_queue_t *pQueue, void *pData);

int queue_pop_front_no_free(lac_queue_t *pQueue, void *pData);

int queue_pop_back(lac_queue_t *pQueue, void *pData);

int queue_pop_back_no_free(lac_queue_t *pQueue, void *pData);

int queue_del(lac_queue_t *pQueue, struct queue_node_t *pNode, void *pData);

int queue_del_no_free(lac_queue_t *pQueue, struct queue_node_t *pNode, void *pData);

void queue_destroy(lac_queue_t *pQueue);

void queue_clear(lac_queue_t *pQueue);

void queue_next(queue_node_t **ppNode);

void queue_prev(queue_node_t **ppNode);

#endif
