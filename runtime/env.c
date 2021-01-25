//
// Created by 厉宇桐 on 2020/12/23.
//

#include "env.h"

/** @brief @var Global environment */
vmenv_t g_Env = {0};

/** @brief Dedicated function to push data into data stack
 * 
 * @param pStack Data stack
 * @param pLACObject data object
 * @return A bool value of status code
 */
bool stack_push_data(lac_stack_t *pStack, lac_object_t *pLACObject) {
    bool iRet;
    iRet = stack_push(pStack, (void *) &pLACObject, sizeof(lac_object_t *));

    switch (iRet) {
        case TRUE:
#ifdef DEBUG
            switch (pLACObject->Type) {
                case LAC_INT:
                    printf("\n[ Debug ] Push %d into StkData\n", ((lac_int_t *) pLACObject->Child)->iValue);
                    break;
                case LAC_VEC:
                    printf("\n[ Debug ] Push string into StkData\n");
                    break;
                case LAC_VAR:
                    printf("\n[ Debug ] Push var %s into StkData\n", pLACObject->Name);
                case LAC_FUNC:
                    break;
                case LAC_CLASS:
                    printf("\n[ Debug ] Push class %s into StkData\n", pLACObject->Name);
                    break;
            }
#endif
            pLACObject->iRefCnt++;
            return TRUE;
        case FALSE:
        default:
            exit(ERR_DATA);

    }
}

/** @brief Dedicated function to pop data from data stack
 * 
 * @param pStack Data stack
 * @return lac_object, the popped data
 */
lac_object_t *stack_pop_data(lac_stack_t *pStack) {
    bool iRet;
    lac_object_t *pRes;
    iRet = stack_pop(pStack, (void *) &pRes);
    switch (iRet) {
        case TRUE:
            pRes->iRefCnt--;
            return pRes;
        case FALSE:
        default:
            /* Safe to return here, beacuse only the vm use this function, if there is no data,
            * then there is definitely an error.
            */
            pRes = env_create_lac_int(0);
            printf("\n[ Warning ] Stack is empty, 0 is poped\n");
            return pRes;
    }
}

/** @brief Dedicated function to access top of data stack
 * 
 * @param pStack Data stack
 * @return lac_object, the data at top
 */
lac_object_t *stack_top_data(lac_stack_t *pStack) {
    /* Access the top of stack without pop */
    lac_object_t *pRes;
    if (stack_is_empty(pStack)) {
        pRes = env_create_lac_int(0);
        printf("\n[ Warning ] Stack is empty, 0 is returned\n");
        return pRes;
    } else {
        return *(lac_object_t **) (pStack->pTop->pData);
    }
}
/** @brief Dedicated function to push CFA into return stack
 * 
 * @param pStack Data stack
 * @param iData CFA
 * @return A bool value of status code
 */
inline int stack_push_return(lac_stack_t *pStack, int iData) {
    /* special push method dedicated for VM(int) */
#ifdef DEBUG
    printf("\n[ Debug ] Push %d into StkReturn\n", iData);
    fflush(stdout);
#endif
    int iRet;
    iRet = stack_push(pStack, (void *) &iData, sizeof(int));
    if (iRet != TRUE) {
        exit(ERR_DATA);
    }
    return TRUE;
}

/** @brief Dedicated function to pop CFA from return stack
 * 
 * @param pStack Data stack
 * @return lac_object, the popped CFA
 */
inline int stack_pop_return(lac_stack_t *pStack) {
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
    if (stack_is_empty(pStack)) {
        printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = N/A\n", iRes);
    } else {
        printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = %d\n", iRes, stack_top_return(pStack));
    }
    fflush(stdout);
#endif
    return iRes;
}

/** @brief Dedicated function to access top of return stack
 * 
 * @param pStack Data stack
 * @return int, the CFA at top
 */
inline int stack_top_return(lac_stack_t *pStack) {
    /* Access the top of stack without pop */

    if (stack_is_empty(pStack)) {
        printf("\n[ Warning ] Stack is empty, 0 is returned\n");
        return 0;
    } else {
        return *(int *) (pStack->pTop->pData);
    }
}

/** @brief Create new lac_func object, register it to object manager
 *
 * @param FuncNameStr Name string
 * @param iNameStrLength strlen(FuncNameStr)
 * @param iCFA CFA of function
 * @param FuncType Type of function
 * @param pFunc Function pointer
 * @return 
 */
lac_object_t *env_create_lac_func(char *FuncNameStr, const int iNameStrLength, int iCFA, e_vm_func_type FuncType, basic pFunc) {
    /* Create a new lac function */
    lac_object_t *pNewObject = calloc(1, sizeof(lac_object_t));
    if (pNewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }

    pNewObject->iRefCnt = 0;
    pNewObject->Type = LAC_FUNC;
    /* [!] */
    if (FuncNameStr != NULL) {
        if (iNameStrLength > 0) {
            strncpy(pNewObject->Name, FuncNameStr, (size_t) iNameStrLength);
        } else {
            strncpy(pNewObject->Name, FuncNameStr, MAX_LEXEME_LEN);
        }
    }

    lac_func_t *pNewFunc = (lac_func_t *) calloc(1, sizeof(lac_func_t));

    if (pNewFunc == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(pNewObject);
        exit(ERR_MEM);
    }

    pNewObject->Child = pNewFunc;
    pNewFunc->iCFA = iCFA;
    pNewFunc->Type = FuncType;
    if (pFunc != NULL && pNewFunc->Type != VM_FUNC_LAC) pNewFunc->pFunc = pFunc;
    vmtable_init(&pNewFunc->VMTable);
    hash_symtable_init(&pNewFunc->SymTable);

    /* Save to a queue of objects */
    queue_push_back(&g_Env.Objects, (void *) &pNewObject, sizeof(lac_object_t *));
    return pNewObject;
}

/** @brief Create new lac_class object, register it to object manager
 *
 * @param FuncNameStr Name string
 * @param iNameStrLength strlen(FuncNameStr)
 * @param iCFA CFA of function
 * @param FuncType Type of function
 * @param pFunc Function pointer
 * @return 
 * @remark Same as env_create_lac_func, since lac_class and lac_func are same
 */
lac_object_t *env_create_lac_class(char *FuncNameStr, const int iNameStrLength, int iCFA, e_vm_func_type FuncType, basic pFunc) {
    lac_object_t *pNewObject = env_create_lac_func(FuncNameStr, iNameStrLength, iCFA, FuncType, pFunc);
    pNewObject->Type = LAC_CLASS;
    return pNewObject;
}

/** @brief Create new lac_int object, register it to object manager
 * 
 * @param iValue INT value
 * @return New object
 */
lac_object_t *env_create_lac_int(int iValue) {
    /* Create a new lac object int */
    lac_object_t *pNewObject = calloc(1, sizeof(lac_object_t));
    if (pNewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }

    pNewObject->iRefCnt = 0;
    pNewObject->Type = LAC_INT;
    /* [!] */

    lac_int_t *pNewInt = (lac_int_t *) calloc(1, sizeof(lac_int_t));
    if (pNewInt == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(pNewObject);
        exit(ERR_MEM);
    }

    pNewObject->Child = pNewInt;
    pNewInt->iValue = iValue;

    /* Save to a queue of objects */
    queue_push_back(&g_Env.Objects, (void *) &pNewObject, sizeof(lac_object_t *));
    return pNewObject;
}

/** @brief Create new lac_vec object, register to object manager
 * 
 * @param VecNameStr Name of vector
 * @param iNameStrLength strlen(VecNameStr)
 * @param iLength Length of string
 * @param iSize Data sive
 * @param pData Content of vector
 * @return 
 */
lac_object_t *env_create_lac_vec(const char *VecNameStr, int iNameStrLength, int iLength, int iSize, void *pData) {
    /* Create a lac vector */
    lac_object_t *pNewObject = calloc(1, sizeof(lac_object_t));
    if (pNewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }

    pNewObject->iRefCnt = 0;
    pNewObject->Type = LAC_VEC;
    /* [!] */
    if (VecNameStr != NULL) {
        if (iNameStrLength > 0) {
            strncpy(pNewObject->Name, VecNameStr, (size_t) iNameStrLength);
        } else {
            strncpy(pNewObject->Name, VecNameStr, MAX_LEXEME_LEN);
        }
    }

    lac_vec_t *pNewVec = (lac_vec_t *) calloc(1, sizeof(lac_vec_t));
    if (pNewVec == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(pNewObject);
        exit(ERR_MEM);
    }

    pNewObject->Child = pNewVec;
    pNewVec->iRef = 0;
    pNewVec->iLength = iLength;
    pNewVec->iSize = iSize;

    pNewVec->pData = calloc((size_t) iLength, sizeof(int));
    if (pNewVec->pData == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(pNewVec);
        exit(ERR_MEM);
    }

    /* The input is a empty vector, we only fill it with 0*/
    if (pData == NULL) {
        for (int idx = 0; idx < pNewVec->iLength; idx++) {
            /* Char stored as int */
            pNewVec->pData[idx] = 0;
        }
    } else {
        /* The input is not a empty vector */
        switch (pNewVec->iSize) {
            case 1:
                for (int idx = 0; idx < pNewVec->iLength; idx++) {
                    /* Char stored as int */
                    pNewVec->pData[idx] = (int) ((char *) pData)[idx];
                }
                break;
            case 4:
                for (int idx = 0; idx < pNewVec->iLength; idx++) {
                    pNewVec->pData[idx] = (int) ((int *) pData)[idx];
                }
                break;
            default:
                printf("\n[ Error ] The Size of each element must be 1(char) or 4(int) Byte\n");
                exit(ERR_MEM);
        }
    }

    /* Save to a queue of objects */
    queue_push_back(&g_Env.Objects, (void *) &pNewObject, sizeof(lac_object_t *));
    return pNewObject;
}

/** Create lac_var object, register to object manager
 * 
 * @param VarNameStr Variable name
 * @param iNameStrLength strlen(VarNameStr)
 * @param pLACObject The object that variable points to
 * @return 
 */
lac_object_t *env_create_lac_var(const char *VarNameStr, int iNameStrLength, lac_object_t *pLACObject) {
    /* Create a lac variable */
    lac_object_t *pNewObject = calloc(1, sizeof(lac_object_t));
    if (pNewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }

    pNewObject->iRefCnt = 0;
    pNewObject->Type = LAC_VAR;
    /* [!] */
    if (VarNameStr != NULL) {
        if (iNameStrLength > 0) {
            strncpy(pNewObject->Name, VarNameStr, (size_t) iNameStrLength);
        } else {
            strncpy(pNewObject->Name, VarNameStr, MAX_LEXEME_LEN);
        }

    }

    lac_var_t *pNewVar = (lac_var_t *) calloc(1, sizeof(lac_var_t));
    if (pNewVar == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(pNewObject);
        exit(ERR_MEM);
    }

    pNewObject->Child = pNewVar;
    if (pLACObject != NULL) {
        pNewVar->Object = pLACObject;
        pNewVar->Type = pLACObject->Type;
        pNewVar->bValid = TRUE;
    } else {
        pNewVar->Object = pNewObject;
        pNewVar->Type = LAC_VAR;
        pNewVar->bValid = FALSE;
    }

    /* Save to a queue of objects */
    queue_push_back(&g_Env.Objects, (void *) &pNewObject, sizeof(lac_object_t *));
    return pNewObject;
}

/** Set the created var's contend(reference)
 * 
 * @param pLACVAR The variable
 * @param pLACObject New value
 * @return 
 * #remark A bool value, status code
 */
bool env_set_lac_var(lac_object_t *pLACVAR, lac_object_t *pLACObject) {
    if (pLACVAR->Type != LAC_VAR) return FALSE;

    if (((lac_var_t *) pLACVAR->Child)->Object != NULL && ((lac_var_t *) pLACVAR->Child)->bValid) {
        ((lac_var_t *) pLACVAR->Child)->Object->iRefCnt--;
    }

    ((lac_var_t *) pLACVAR->Child)->Object = pLACObject;
    if (pLACObject != NULL) {
        ((lac_var_t *) pLACVAR->Child)->Type = pLACObject->Type;
        ((lac_var_t *) pLACVAR->Child)->bValid = TRUE;
        pLACObject->iRefCnt++;
    } else {
        ((lac_var_t *) pLACVAR->Child)->Object = pLACVAR;
        ((lac_var_t *) pLACVAR->Child)->bValid = FALSE;
    }
    return TRUE;
}

/** @brief Gnerate lac instance from class
 * 
 * @param pLACClass The class
 * @param pLACInstance The instance
 * @return  A bool value, status code
 * @remark The instance must be created in advance
 */
bool env_instance_class(lac_object_t *pLACClass, lac_object_t *pLACInstance) {
    /* Create a new lac function from class*/
    if (pLACClass == NULL || pLACInstance == NULL) {
        return FALSE;
    }
    if (pLACClass->Type != LAC_CLASS || pLACInstance->Type != LAC_FUNC) {
        return FALSE;
    }
    lac_class_t *pClass = (lac_class_t *) pLACClass->Child;
    lac_func_t *pFunc = (lac_func_t *) pLACInstance->Child;

    /* Init the new instance */
    pFunc->iCFA = VM_CFA_LAC;
    pFunc->Type = VM_FUNC_LAC;

    /* Reset the instance */
    vmtable_clear(&pFunc->VMTable);
    hash_symtable_reset(&pFunc->SymTable);

    /* Copy the field of class to instance */
    queue_node_t *pCursor;
    hash_table_entry_t *pOldEntryTmp;
    lac_object_t *pCurrObject;
    lac_object_t *pLACObjectTmp;
    hash_table_entry_t *pEntryTmp;

    for (int idx = 0; idx <= pClass->VMTable.iTail; idx++) {
        vmtable_add(&pFunc->VMTable, pClass->VMTable.Objects[idx]);
    }
    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        pCursor = pClass->SymTable.Data[idx].pFront;
        pFunc->SymTable.CheckPoints[idx] = pClass->SymTable.CheckPoints[idx];
        pFunc->SymTable.iNumEntry = pClass->SymTable.iNumEntry;
        while (pCursor != NULL) {
            pOldEntryTmp = (hash_table_entry_t *) pCursor->pData;
            pCurrObject = (*(lac_object_t **) pOldEntryTmp->pData);
            pCurrObject->iRefCnt++; // Decrease the counter
            switch (pCurrObject->Type) {
                case LAC_FUNC:
                case LAC_INT:
                case LAC_CLASS:
                    queue_push_back(&pFunc->SymTable.Data[idx], (void *) pCursor->pData, pCursor->iNumBytes);
                    break;
                case LAC_VEC:
                    pLACObjectTmp = env_create_lac_vec(pCurrObject->Name, (int) strlen(pCurrObject->Name), ((lac_vec_t *) pCurrObject->Child)->iLength, ((lac_vec_t *) pCurrObject->Child)->iSize,
                                                       ((lac_vec_t *) pCurrObject->Child)->pData);
                    pEntryTmp = hash_table_gen_entry(pOldEntryTmp->Key, &pLACObjectTmp, sizeof(lac_object_t *));
                    queue_push_back(&pFunc->SymTable.Data[idx], (void *) pEntryTmp, sizeof(hash_table_entry_t));
                case LAC_VAR:
                    pLACObjectTmp = env_create_lac_var(pCurrObject->Name, (int) strlen(pCurrObject->Name), ((lac_var_t *) pCurrObject->Child)->Object);
                    pEntryTmp = hash_table_gen_entry(pOldEntryTmp->Key, &pLACObjectTmp, sizeof(lac_object_t *));
                    queue_push_back(&pFunc->SymTable.Data[idx], (void *) pEntryTmp, sizeof(hash_table_entry_t));
                    break;
            }
            queue_next(&pCursor);
        }
    }

    return TRUE;

}

/** @brief This function destroys a lac object
 * 
 * @param pLACObject Object
 * @return A bool vavlue, status code
 */
int destroy_lac_obj(lac_object_t *pLACObject) {
    /* Destroy a lac object */
    e_vm_func_type FuncTypeTmp;
    switch (pLACObject->Type) {
        case LAC_VEC:
            free(((lac_vec_t *) pLACObject->Child)->pData);
            break;
        case LAC_VAR:
            ((lac_var_t *) pLACObject->Child)->Object->iRefCnt--;
            break;
        case LAC_FUNC:
            FuncTypeTmp = ((lac_func_t *) pLACObject->Child)->Type;
            switch (FuncTypeTmp) {

                case VM_FUNC_CTRL:
                case VM_FUNC_BASIC:
                    /* Trying to destroy a basic function is not allowed */
                    return FALSE;
                case VM_FUNC_LAC:
                    /* Haven't decide how to handle */
                default:
                    break;
            }
        default:
            break;
    }
    free(pLACObject->Child);
    free(pLACObject);
    return TRUE;
}

//int reset_lac_obj(lac_object_t *pLACObject){
//    /* This function reset a object, free its child,  */
//    switch (pLACObject->Type) {
//        case LAC_VEC:
//            free(((lac_vec_t *) pLACObject->Child)->pData);
//            break;
//        case LAC_VAR:
//            ((lac_var_t *) pLACObject->Child)->Object->iRefCnt --;
//            break;
//        default:
//            break;
//    }
//    free(pLACObject->Child);
//    pLACObject->Child = NULL;
//    memset(pLACObject->Name, 0, MAX_LEXEME_LEN);
//    pLACObject->Type = LAC_INT;
//    return TRUE;
//}