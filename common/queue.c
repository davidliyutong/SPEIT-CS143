//
// Created by 厉宇桐 on 2020/11/19.
//
// Data structure: queue

#include "queue.h"

lac_queue_t *queue_create() {
    /* Create a queue dynamically, the pointer to the queue will be returned
     * This function will call malloc, must be careful with mem leak*/

    lac_queue_t *pQueue;
    pQueue = (lac_queue_t *) malloc(sizeof(lac_queue_t));
    if (NULL == pQueue) return NULL;
    pQueue->pFront = NULL;
    pQueue->pRear = NULL;

    pQueue->iLength = 0;
    return pQueue;
}

void queue_init(lac_queue_t *pQueue) {
    /* Initiate the queue that is defined outside of function */

    pQueue->pFront = NULL;
    pQueue->pRear = NULL;

    pQueue->iLength = 0;
}

int queue_is_empty(lac_queue_t *pQueue) {
    /* Tell if a queue is empty, return TRUE if it is, FALSE if not */

    if (pQueue->pFront == NULL && pQueue->pRear == NULL) return TRUE;
    else return FALSE;
}

//int queue_push_front(lac_queue_t *queue, const void *pData) {
//    /* This function enqueue pData at the front end */
//    return ERR_NOT_IMPLEMENTED;
//}

int queue_push_back(lac_queue_t *pQueue, void *pData, int iNumBytes) {
    /* This function enqueue pData at the rear end */

    /* Allocate memory */
    queue_node_t *pNode;
    pNode = (queue_node_t *) malloc(sizeof(queue_node_t));
    if (pNode == NULL) return FALSE;
    pNode->pData = malloc((size_t) iNumBytes);
    if (pNode->pData == NULL) return FALSE;
    memcpy(pNode->pData, pData, (size_t) iNumBytes);
    pNode->iNumBytes = iNumBytes;

    /* Join pNode in the pQueue */
    pNode->pNext = pNode;
    if (pQueue->pRear != NULL) {
        pNode->pPrev = pQueue->pRear;
    } else {
        pNode->pPrev = pNode;
    }

    /* Modify the pQueue */
    if (pQueue->pRear == NULL) {
        pQueue->pRear = pNode;
    } else {
        pQueue->pRear->pNext = pNode;
    }
    pQueue->pRear = pNode;
    if (pQueue->pFront == NULL) {
        pQueue->pFront = pNode;
    }

    pQueue->iLength += 1;
    return TRUE;
}

int queue_pop_front(lac_queue_t *pQueue, void *pData) {
    /* This function deque the data(ptr) at the front end
     * This function will free the memory of queue node automatically
     */

    if (queue_is_empty(pQueue)) {
        return FALSE;
    }

    if (pData != NULL) {
        memcpy(pData, pQueue->pFront->pData, (size_t) pQueue->pFront->iNumBytes);
    }

    queue_node_t *pNodeTmp = pQueue->pFront;
    /* Modify the pQueue */
    if (pQueue->pFront != pQueue->pRear) {
        pQueue->pFront = pQueue->pFront->pNext;
        pQueue->pFront->pPrev = pQueue->pFront;
    } else {
        pQueue->pFront = pQueue->pRear = NULL;
    }

    /* Free the node */
    free(pNodeTmp->pData);
    free(pNodeTmp);

    pQueue->iLength -= 1;
    return TRUE;
}

//int queue_pop_back(lac_queue_t *queue, void *pData) {
//    /* This function deque the data at the rear end */
//    /* This function will not free the memory, do it outside */
//    return ERR_NOT_IMPLEMENTED;
//}

void queue_destroy(lac_queue_t *pQueue) {
    /* This function destroys a to a dynamically created queue */

    while (pQueue->pFront != NULL) {
        queue_pop_front(pQueue, NULL);
    }
    free(pQueue);
}

void queue_clear(lac_queue_t *pQueue) {
    /* This function applies to a dynamically created queue or defined queue */

    while (pQueue->pFront != NULL) {
        queue_pop_front(pQueue, NULL);
    }
    pQueue->iLength = 0;
}

queue_node_t *queue_next(queue_node_t *pNode) {
    /* Return the pointer of next node in the queue */
    /* This function can be used to access queue nodes 1-by-1 with out pop them */

    queue_node_t *pRet = pNode->pNext;
    if (pRet == pNode) {
        /* It is the end of the queue */
        return NULL;
    } else {
        return pRet;
    }
}

queue_node_t *queue_prev(queue_node_t *pNode) {
    /* Return the pointer of previous node in the queue */
    /* This function can be used to access queue nodes 1-by-1 with out pop them */

    queue_node_t *pRet = pNode->pPrev;
    if (pRet == pNode) {
        /* It is the end of the queue */
        return NULL;
    } else {
        return pRet;
    }
}