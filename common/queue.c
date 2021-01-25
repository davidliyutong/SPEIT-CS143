//
// Created by 厉宇桐 on 2020/11/19.
//
/**@file queue.c
 * @brief This file contains function definition related to a queue
 * @details
 * @author 厉宇桐
 * @data 2020/12/25
 */

#include "queue.h"

/** @brief This function creates a lac_queue dynamically 
 * 
 * @return The queue pointer
 */
lac_queue_t *queue_create() {
    lac_queue_t *pQueue;
    pQueue = (lac_queue_t *) malloc(sizeof(lac_queue_t));
    if (NULL == pQueue) return NULL;
    pQueue->pFront = NULL;
    pQueue->pRear = NULL;

    pQueue->iLength = 0;
    return pQueue;
}

/** @brief This function inits a lac_queue dynamically 
 * 
 * @param pQueue The queue pointer
 */
void queue_init(lac_queue_t *pQueue) {
    pQueue->pFront = NULL;
    pQueue->pRear = NULL;

    pQueue->iLength = 0;
}

/** @brief This function judge if a lac_queue is empty 
 * 
 * @param pQueue 
 * @return 
 */
bool queue_is_empty(lac_queue_t *pQueue) {
    if (pQueue->pFront == NULL && pQueue->pRear == NULL) return TRUE;
    else return FALSE;
}

/** @brief This function judge if a node belongs to a queue
 * 
 * @param pQueue The node pointer
 * @param pNode The queue pointer
 * @return 
 */
bool queue_has_node(lac_queue_t *pQueue, struct queue_node_t *pNode) {
    if (queue_is_empty(pQueue)) return FALSE;
    queue_node_t *Cursor = pQueue->pFront;
    while (Cursor != NULL) {
        if (Cursor == pNode) {
            return TRUE;
        }
        queue_next(&Cursor);
    }
    return FALSE;
}

/** @brief This function pushes an element to the queue at the front
 * 
 * @param pQueue The queue pointer
 * @param pData The data pointer (void)
 * @param iNumBytes Size of data (bytes)
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @remark malloc is called and a new memory is assigned to store the data
 * @code
 * bool iRet = queue_push_front(pQueue, (void *)&Data, sizeof(Data));
 * @endcode
 */
bool queue_push_front(lac_queue_t *pQueue, const void *pData, int iNumBytes) {

    /* Allocate memory */
    queue_node_t *pNode;
    pNode = (queue_node_t *) malloc(sizeof(queue_node_t));
    if (pNode == NULL) return FALSE;
    pNode->pData = malloc((size_t) iNumBytes);
    if (pNode->pData == NULL) return FALSE;
    memcpy(pNode->pData, pData, (size_t) iNumBytes);
    pNode->iNumBytes = iNumBytes;

    /* Join pNode in the pQueue */
    pNode->pPrev = pNode;
    if (pQueue->pFront != NULL) {
        pNode->pNext = pQueue->pFront;
    } else {
        pNode->pNext = pNode;
    }

    /* Modify the pQueue */
    if (pQueue->pFront == NULL) {
        pQueue->pFront = pNode;
    } else {
        pQueue->pFront->pPrev = pNode;
    }
    pQueue->pFront = pNode;
    if (pQueue->pRear == NULL) {
        pQueue->pRear = pNode;
    }

    pQueue->iLength += 1;
    return TRUE;
}

/** @brief This function pushes an element to the queue at the back
 * 
 * @param pQueue The queue pointer
 * @param pData The data pointer (void)
 * @param iNumBytes Size of data (bytes)
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @remark malloc is called and a new memory is assigned to store the data
 * @code
 * bool iRet = queue_push_back(pQueue, (void *)&Data, sizeof(Data));
 * @endcode
 */
int queue_push_back(lac_queue_t *pQueue, void *pData, int iNumBytes) {

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

/** @brief This function pops an element in the queue at the front
 * 
 * @param pQueue The queue pointer
 * @param pData The data pointer of receiver (void)
 * @param iNumBytes Size of data (bytes)
 * @param bFreeData  If bFree == TRUE, node data is freed
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @remark This function will free the memory of queue node and node data automatically.
 *         The size of receiver must match the size of node data.
 * @code
 * bool iRet = queue_pop_front(pQueue, (void *)&Data);
 * @endcode
 */
int queue_pop_front(lac_queue_t *pQueue, void *pData, bool bFreeData) {

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

    if (bFreeData == TRUE) {
        free(pNodeTmp->pData);
    }
    /* Free the node */
    free(pNodeTmp);

    pQueue->iLength -= 1;
    return TRUE;
}


/** @brief This function pops an element in the queue at the back
 * 
 * @param pQueue The queue pointer
 * @param pData The data pointer of receiver (void)
 * @param iNumBytes Size of data (bytes)
 * @param bFreeData  If bFree == TRUE, node data is freed
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @remark This function will free the memory of queue node
 *         The size of receiver must match the size of node data.
 * @code
 * bool iRet = queue_pop_back(pQueue, (void *)&Data);
 * @endcode
 */
int queue_pop_back(lac_queue_t *pQueue, void *pData, bool bFreeData) {

    if (queue_is_empty(pQueue)) {
        return FALSE;
    }

    if (pData != NULL) {
        memcpy(pData, pQueue->pRear->pData, (size_t) pQueue->pRear->iNumBytes);
    }

    queue_node_t *pNodeTmp = pQueue->pRear;
    /* Modify the pQueue */
    if (pQueue->pRear != pQueue->pFront) {
        pQueue->pRear = pQueue->pRear->pPrev;
        pQueue->pRear->pNext = pQueue->pRear;
    } else {
        pQueue->pRear = pQueue->pFront = NULL;
    }

    /* Free the node */
    if (bFreeData == TRUE) {
        free(pNodeTmp->pData);

    }
    free(pNodeTmp);

    pQueue->iLength -= 1;
    return TRUE;
}


/** @brief This function removes a node from queue
 * 
 * @param pQueue The queue pointer
 * @param pNode The node pointer
 * @param pData The data pointer
 * @param Check Whether to check the node belongs to queue
 * @return 
 */
int queue_del(lac_queue_t *pQueue, struct queue_node_t *pNode, void *pData, bool Check, bool bFreeData) {
    if (pNode == NULL) return FALSE;
    if (pQueue == NULL) Check = FALSE; /** Disable check by setting pQueue == NULL */
    
    if (Check == TRUE) {
        if (!queue_has_node(pQueue, pNode)) return FALSE;
        if (pNode == pQueue->pFront || pNode->pPrev == pNode) {
            queue_pop_front(pQueue, pData, bFreeData);
            return TRUE;
        } else if (pNode == pQueue->pRear || pNode->pNext == pNode) {
            queue_pop_back(pQueue, pData, bFreeData);
            return TRUE;
        }
    }

    pNode->pPrev->pNext = pNode->pNext;
    pNode->pNext->pPrev = pNode->pPrev;

    if (pData != NULL) {
        memcpy(pData, pNode->pData, (size_t) pNode->iNumBytes);
    }

    if (bFreeData == TRUE) {
        free(pNode->pData);
    }
    free(pNode);
    pQueue->iLength --;
    return TRUE;
    
}


/** @brief This function destroys a lac_queue dynamically 
 * 
 * @param pQueue 
 */
void queue_destroy(lac_queue_t *pQueue) {
    while (pQueue->pFront != NULL) {
        queue_pop_front(pQueue, NULL, TRUE);
    }
    free(pQueue);
}

/** @brief This function clears a lac_queue 
 * 
 * @param pQueue 
 */
void queue_clear(lac_queue_t *pQueue) {
    while (pQueue->pFront != NULL) {
        queue_pop_front(pQueue, NULL, TRUE);
    }
    pQueue->iLength = 0;
}

/** @brief This function moves the node pointer to its next
 * 
 * @param ppNode 
 * @remark When the pointer hits the end of queue, it becomes NULL
 */
void queue_next(queue_node_t **ppNode) {
    /* This function can be used to access queue nodes 1-by-1 with out pop them */

    queue_node_t *pRet = (*ppNode)->pNext;
    if (pRet == (*ppNode)) {
        /* It is the end of the queue */
        *ppNode = NULL;
    } else {
        *ppNode = pRet;
    }
}

/** @brief This function moves the node pointer to its previous
 * 
 * @param ppNode 
 * @remark When the pointer hits the front of queue, it becomes NULL
 */
void queue_prev(queue_node_t **ppNode) {
    /* Return the pointer of previous node in the queue */
    /* This function can be used to access queue nodes 1-by-1 with out pop them */

    queue_node_t *pRet = (*ppNode)->pPrev;

    if (pRet == (*ppNode)) {
        /* It is the end of the queue */
        *ppNode = NULL;
    } else {
        *ppNode = pRet;
    }
}