//
// Created by 厉宇桐 on 2020/12/23.
//

#include "env.h"

vmenv_t g_Env = {0};

inline int stack_push_vm(lac_stack_t *pStack, int iData) {
    /* special push method dedicated for VM(int) */
#ifdef DEBUG
    if (pStack == &g_Env.StkData) {
        printf("\n[ Debug ] Push %d into StkData\n", iData);
    } else if (pStack == &g_Env.StkReturn) {
        printf("\n[ Debug ] Push %d into StkReturn\n", iData);
    } else { ;
    }
    fflush(stdout);

#endif
    int iRet;
    iRet = stack_push(pStack, (void *) &iData, sizeof(int));
    if (iRet != TRUE) {
        exit(ERR_DATA);
    }
    return TRUE;
}

inline int stack_pop_vm(lac_stack_t *pStack) {
    /* special pop method dedicated for VM(int) */

    int iRet;
    int iRes = 0;
    iRet = stack_pop(pStack, (void *) &iRes);
    if (iRet != TRUE) {
        /* Safe to return here, beacuse only the vm use this function, if there is no data,
         * then there is definitely an error.
         */
        printf("\n[ Warning ] Stack is empty, 0 is poped\n");
        return 0;
    }
#ifdef DEBUG
    if (pStack == &g_Env.StkData) {
        if (stack_is_empty(pStack)) {
            printf("\n[ Debug ] Pop %d from StkData, StkData.top = N/A\n", iRes);
        } else {
            printf("\n[ Debug ] Pop %d from StkData, StkData.top = %d\n", iRes, stack_top_vm(pStack));
        }
    } else if (pStack == &g_Env.StkReturn) {
        if (stack_is_empty(pStack)) {
            printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = N/A\n", iRes);
        } else {
            printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = %d\n", iRes, stack_top_vm(pStack));
        }
    } else { ;
    }
    fflush(stdout);
#endif
    return iRes;
}

inline int stack_top_vm(lac_stack_t *pStack) {
    /* Access the top of stack without pop */

    if (stack_is_empty(pStack)) {
        printf("\n[ Warning ] Stack is empty, 0 is returned\n");
        return 0;
    } else {
        return *(int *) (pStack->pTop->pData);
    }
}


