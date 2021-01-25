//
// Created by 厉宇桐 on 2020/11/15.
//
// Data structure: stack
/**@file stack.c
 * @brief This file contains definition of stack data structure
 * @details
 * @author 厉宇桐
 * @data 2020/11/15
 */
 
#include "stack.h"

/** @brief This function creates a lac_stack dynamically 
 * @return The pointer to the stack
 * @remarks This function will call malloc, must be careful with mem leak*/
lac_stack_t *stack_create() {

    lac_stack_t *pStack;
    pStack = (lac_stack_t *) malloc(sizeof(lac_stack_t));
    if (NULL == pStack) return NULL;

    pStack->pTop = NULL;
    pStack->iLength = 0;
    return pStack;
}

/** @brief This function init a lac_stack 
 * @param pStack The created stack
*/
void stack_init(lac_stack_t *pStack) {
    /* Initiate the stack that is defined outside of function */

    pStack->pTop = NULL;
    pStack->iLength = 0;
}

/** @brief This function judges if a stack is empty
 * @param pStack The stack pointer
 * @return A bool value. TRUE if it is, FALSE if not
 * @code
 * int iRet = stack_is_empty(pStack);
 * @endcode
*/
bool stack_is_empty(lac_stack_t *pStack) {
    /** @remark Function realized by judging if stack.top() == NULL */
    if (pStack->pTop == NULL) return TRUE;
    else return FALSE;
}

/** @brief This function pushes an element in the stack at the rear end
 * 
 * @param pStack The stack pointer
 * @param pData  The data pointer (void)
 * @param iNumBytes Size of data (bytes)
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @code
 * bool iRet = stack_push(pStack, (void *)&Data, sizeof(Data));
 * @endcode
 */
bool stack_push(lac_stack_t *pStack, void *pData, int iNumBytes) {

    /**1. Allocate memory of a stack node*/
    stack_node_t *pNode;
    pNode = (stack_node_t *) malloc(sizeof(stack_node_t));
    if (pNode == NULL) return FALSE;
    pNode->pData = (stack_node_t *) malloc((size_t) iNumBytes);
    if (pNode->pData == NULL) return FALSE;
    
    /** 2. Copy the data to stack node with memcpy*/
    memcpy(pNode->pData, pData, (size_t) iNumBytes);
    pNode->iNumBytes = iNumBytes;

    /** 3. Join pNode in the stack */
    pNode->pPrev = pStack->pTop;

    /** 4. Modify the stack's master node */
    pStack->pTop = pNode;

    pStack->iLength += 1; // counter plus 1
    return TRUE;
}

/** @brief This function pop an element from the stack at the rear end
 * 
 * @param pStack The stack pointer
 * @param pData The data pointer
 * @return A bool value. TRUE If the operation succeed, FALSE if not
 * @remark This function will free the memory of stack node automatically
 */
bool stack_pop(lac_stack_t *pStack, void *pData) {

    /** 1. If the stack is empty, function returns FALSE */
    if (stack_is_empty(pStack)) {
        return FALSE;
    }

    /** 2. If the pData is not null, the data will be copied. Otherwise it will be dumped*/
    if (pData != NULL) {
        memcpy(pData, pStack->pTop->pData, (size_t) pStack->pTop->iNumBytes);
    }

    stack_node_t *pNodeTmp = pStack->pTop;

    /** 3. The stack is modified, iLength decrease by 1*/
    if (pStack->pTop != NULL) {
        pStack->pTop = pStack->pTop->pPrev;
    } else {
        return FALSE;
    }
    pStack->iLength -= 1;

    /** 4. The node and node->pData are freed */
    free(pNodeTmp->pData);
    free(pNodeTmp);
    
    return TRUE;
}

/** @brief This function destroys to a dynamically created stack
 * 
 * @param pStack The stack pointer
 * @remark After execution, the stack's master node will be freed. Might be dangerous if pStack is not set to NULL
 */
void stack_destroy(lac_stack_t *pStack) {
    while (pStack->pTop != NULL) {
        stack_pop(pStack, NULL);
    }
    free(pStack);
}

/** @brief This function clears a stack
 * 
 * @param pStack The stack pointer
 */
void stack_clear(lac_stack_t *pStack) {
    while (pStack->pTop != NULL) {
        stack_pop(pStack, NULL);
    }
    pStack->iLength = 0;
}

