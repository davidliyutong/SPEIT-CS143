//
// Created by 厉宇桐 on 2020/12/28.
//
/** @file object.c
 * @brief This file contains function definition related to a object operation
 * @details There are two parts: 1. VM table operations 2. Hash symbol table operations
 * @author 厉宇桐
 * @data 2020/12/28
 */
 
#include "object.h"

/** @brief The invalid_func is an invalid lac function object used during function declaration */
lac_func_t invalid_func = {VM_FUNC_BASIC};
lac_object_t vmtable_invalid_func = {0, LAC_FUNC, "", &invalid_func};

bool vmtable_init(vmtable_t *pVMTable) {
    /* Init a vmtable defined outside of function */
    pVMTable->iMaxLength = INIT_VM_TABLE_LEN;
    pVMTable->iTail = -1;
    pVMTable->Objects = malloc(sizeof(lac_object_t *) * pVMTable->iMaxLength);

    if (pVMTable->Objects == NULL) {
        printf("\n[ Error ] Out of memory\n");
        exit(ERR_MEM);
    }

    memset(pVMTable->Objects, 0, sizeof(lac_object_t *) * pVMTable->iMaxLength);
    return TRUE;
}

bool vmtable_expand(vmtable_t *pVMTable) {
    /* If the pVMTable is not enough, then it need to be expanded */

    pVMTable->Objects = (lac_object_t **) realloc(pVMTable->Objects, sizeof(lac_object_t *) * (pVMTable->iMaxLength + INIT_VM_TABLE_LEN));

    if (pVMTable->Objects == NULL) {
        printf("\n[ Error ] Out of memory\n");
        exit(ERR_MEM);
    }

    pVMTable->iMaxLength += INIT_VM_TABLE_LEN;
    return TRUE;
}

inline bool vmtable_add(vmtable_t *pVMTable, lac_object_t *pLACObject) {
    /* Add a OpCode to the vmtable */

    if (pVMTable->iTail >= pVMTable->iMaxLength - 1) {
        vmtable_expand(pVMTable);
    }

    pLACObject->iRefCnt++;

    pVMTable->Objects[++pVMTable->iTail] = pLACObject;
    return TRUE;
};

inline bool vmtable_set(vmtable_t *pVMTable, int idx, lac_object_t *pLACObject) {
    /* Add a OpCode to the vmtable */

    pVMTable->Objects[idx] = pLACObject;
    pLACObject->iRefCnt++;
    return TRUE;
};


inline bool vmtable_del(vmtable_t *pVMTable) {
    /* Delete a OpCode from the vmtable */
    pVMTable->Objects[pVMTable->iTail]->iRefCnt--;
    pVMTable->Objects[pVMTable->iTail--] = &vmtable_invalid_func;
    return TRUE;
};

inline bool vmtable_checkout(vmtable_t *pVMTable) {
    /* Checkout the modification*/
    pVMTable->iCheckPoint = pVMTable->iTail;
    return TRUE;
};

inline bool vmtable_revert(vmtable_t *pVMTable) {
    /* Revert the modification*/
    while (pVMTable->iTail > pVMTable->iCheckPoint) {
        vmtable_del(pVMTable);
    }
    pVMTable->iCheckPoint = pVMTable->iTail;
    return TRUE;
};

inline int vmtable_strlen(lac_vec_t *pVec) {
    int iMaxLength = pVec->iLength;
    for (int iLength = 0; iLength < iMaxLength; iLength++) {
        if (pVec->pData[iLength] == 0) return iLength;
    }
    return iMaxLength;
}

bool vmtable_clear(vmtable_t *pVMTable) {
    free(pVMTable->Objects);
    vmtable_init(pVMTable);
    return TRUE;
}


bool hash_symtable_init(hash_table_t *pSymTable) {
    return hash_table_init(pSymTable);
}

bool hash_symtable_reset(hash_table_t *pSymTable) {
    return hash_table_reset(pSymTable);
}

bool hash_symtable_checkout(hash_table_t *pSymTable) {
    return hash_table_checkout(pSymTable);
}

bool hash_symtable_revert(hash_table_t *pSymTable) {
    hash_table_entry_t OldEntryTmp;
    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        while (pSymTable->Data[idx].iLength > pSymTable->CheckPoints[idx]) {
            queue_pop_back(&pSymTable->Data[idx], (void *) &OldEntryTmp, FALSE);
            (*(lac_object_t **) OldEntryTmp.pData)->iRefCnt--; // Decrease the counter
            free(OldEntryTmp.pData);
            pSymTable->iNumEntry--;
        }
    }
    return TRUE;
}

hash_table_query_res_t hash_symtable_search(hash_table_t *pSymTable, const char *pKey, int iLength) {
    /* Search a hash_table entry by its key (char) */
    char sKey[MAX_KEY_LEN + 1] = {0};
    hash_table_query_res_t res = {ERR_SYMBOL_OVERFLOW, NULL};

    if (iLength > MAX_KEY_LEN) return res;
    strncpy(sKey, pKey, (size_t) iLength);

    res = hash_table_query(pSymTable, sKey);
    return res;
}

e_symbol_status hash_symtable_add_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject) {
    /* iSize is the size of pValue */

    if (iLength > MAX_KEY_LEN) return ERR_SYMBOL_OVERFLOW;
    if (iLength == 0) iLength = (int) strlen(pSymbolStr);

    char sKey[MAX_KEY_LEN + 1] = {0};
    strncpy(sKey, pSymbolStr, (size_t) iLength);

    hash_table_query_res_t QueryRes;
    QueryRes = hash_symtable_search(pSymTable, sKey, iLength);
    int iRet;

    hash_table_entry_t *pEntryTmp;
    pEntryTmp = hash_table_gen_entry(sKey, &pLACObject, sizeof(lac_object_t *));
    hash_table_entry_t OldEntryTmp;

    if (QueryRes.pNode == NULL) {
        /* No such item */
        queue_push_back(&pSymTable->Data[QueryRes.idx], (void *) pEntryTmp, sizeof(hash_table_entry_t));
        pLACObject->iRefCnt++;
        pSymTable->iNumEntry++;
        return INFO_SYMBOL_NOT_FOUND;
    } else {
        /* Duplicated item, modify */
        iRet = queue_del(&pSymTable->Data[QueryRes.idx], QueryRes.pNode, (void *) &OldEntryTmp, TRUE, FALSE);
        if (iRet != TRUE) return (ERR_SYMBOL_OVERFLOW);
        (*(lac_object_t **) OldEntryTmp.pData)->iRefCnt--;
        free(OldEntryTmp.pData);

        queue_push_back(&pSymTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry_t));
        pLACObject->iRefCnt++;
        return SYMBOL_OK;
    }
}

lac_object_t *hash_symtable_get_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength) {
    /* iSize is the size of pValue */
    hash_table_query_res_t QueryRes;
    lac_object_t *pObj;

    QueryRes = hash_symtable_search(pSymTable, pSymbolStr, iLength);
    if (QueryRes.pNode == NULL) {
        return NULL;
    }
    pObj = *(lac_object_t **) ((hash_table_entry_t *) QueryRes.pNode->pData)->pData;
    return pObj;
}

lac_object_t *hash_symtable_get_obj_from_query(hash_table_query_res_t QueryRes) {
    /* iSize is the size of pValue */
    if (QueryRes.pNode == NULL) {
        return NULL;
    } else {
        return *(lac_object_t **) (((hash_table_entry_t *) QueryRes.pNode->pData)->pData);
    }
}

bool hash_symtable_set_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject) {
    /* This function modifies the value of a entry in symbol table */
    hash_table_query_res_t QueryRes;

    QueryRes = hash_symtable_search(pSymTable, pSymbolStr, iLength);
    if (QueryRes.pNode == NULL) {
        return FALSE;
    }
    pLACObject->iRefCnt++;
    (*(lac_object_t **) ((hash_table_entry_t *) QueryRes.pNode->pData)->pData)->iRefCnt--;
    *(lac_object_t **) ((hash_table_entry_t *) QueryRes.pNode->pData)->pData = pLACObject;
    return TRUE;
}