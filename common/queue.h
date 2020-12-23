//
// Created by 厉宇桐 on 2020/11/15.
//

#ifndef LAC_QUEUE_H
#define LAC_QUEUE_H

#include <memory.h>
#include <stdlib.h>
#include "macros.h"
#include "types.h"

lac_queue_t *queue_create();

void queue_init(lac_queue_t *pQueue);

int queue_is_empty(lac_queue_t *pQueue);

//int queue_push_front(lac_queue_t *queue, const void *pData) ;
int queue_push_back(lac_queue_t *pQueue, void *pData, int iNumBytes);

int queue_pop_front(lac_queue_t *pQueue, void *pData);

//int queue_pop_back(lac_queue_t *queue, void *pData);
void queue_destroy(lac_queue_t *pQueue);

void queue_clear(lac_queue_t *pQueue);

queue_node_t *queue_next(queue_node_t *pNode);

queue_node_t *queue_prev(queue_node_t *pNode);

#endif
