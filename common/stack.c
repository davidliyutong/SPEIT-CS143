//
// Created by 厉宇桐 on 2020/11/15.
//
// Data structure: stack

#include "stack.h"

lac_stack_t *stack_create() {
    /* Create a stack dynamically, the pointer to the stack will be returned
     * This function will call malloc, must be careful with mem leak*/


    lac_stack_t *pStack;
    pStack = (lac_stack_t *) malloc(sizeof(lac_stack_t));
    if (NULL == pStack) return NULL;

    pStack->pTop = NULL;
    pStack->iLength = 0;
    return pStack;
}

void stack_init(lac_stack_t *pStack) {
    /* Initiate the stack that is defined outside of function */

    pStack->pTop = NULL;
    pStack->iLength = 0;
}

int stack_is_empty(lac_stack_t *pStack) {
    /* Tell if a stack is empty, return TRUE if it is, FALSE if not */

    if (pStack->pTop == NULL) return TRUE;
    else return FALSE;
}

/* [!] */
//int queue_push_front(lac_queue_t *queue, const void *pData) {
//    /* This function enqueue pData at the front end */
//    return ERR_NOT_IMPLEMENTED;
//}

int stack_push(lac_stack_t *pStack, void *pData, int iNumBytes) {
    /* This function push pData at the rear end */

    /* Allocate memory */
    stack_node_t *pNode;
    pNode = (stack_node_t *) malloc(sizeof(stack_node_t));
    if (pNode == NULL) return FALSE;
    pNode->pData = (stack_node_t *) malloc((size_t) iNumBytes);
    if (pNode->pData == NULL) return FALSE;
    memcpy(pNode->pData, pData, (size_t) iNumBytes);
    pNode->iNumBytes = iNumBytes;

    /* Join pNode in the stack */
    pNode->pPrev = pStack->pTop;

    /* Modify the stack */
    pStack->pTop = pNode;

    pStack->iLength += 1; // counter plus 1
    return TRUE;
}

int stack_pop(lac_stack_t *pStack, void **pData) {
    /* This function pop the data(ptr) at the top end
     * This function will free the memory of stack node automatically
     */

    if (stack_is_empty(pStack)) {
        return FALSE;
    }

    if (pData != NULL) {
        memcpy(pData, pStack->pTop->pData, (size_t) pStack->pTop->iNumBytes);
    }

    stack_node_t *pNodeTmp = pStack->pTop;

    /* Modify the stack */
    if (pStack->pTop != NULL) {
        pStack->pTop = pStack->pTop->pPrev;
    } else {
        return FALSE;
    }

    free(pNodeTmp->pData);
    free(pNodeTmp);

    pStack->iLength -= 1; // counter plus 1
    return TRUE;
}

void stack_destroy(lac_stack_t *pStack) {
    /* This function only applies to a dynamically created queue */
    while (pStack->pTop != NULL) {
        stack_pop(pStack, NULL);
    }
    free(pStack->pTop);
    free(pStack);
}

void stack_clear(lac_stack_t *pStack) {
    /* This function applies to a defined created queue or defined queue */
    while (pStack->pTop != NULL) {
        stack_pop(pStack, NULL);
    }
    pStack->iLength = 0;
}

