//
// Created by 厉宇桐 on 2020/12/26.
//

#include "symtable.h"

int hash_symtable_init(hash_table_t *pSymTable) {
    return hash_table_init(pSymTable);
}

int hash_symtable_reset(hash_table_t *pSymTable) {
    return hash_table_reset(pSymTable);
}

int hash_symtable_checkout(hash_table_t *pSymTable) {
    return hash_table_checkout(pSymTable);
}

int hash_symtable_revert(hash_table_t *pSymTable) {
    return hash_table_revert(pSymTable);
}

hash_table_query_res hash_symtable_search(hash_table_t *pSymTable, const char *pKey, int iLength) {
    char sKey[MAX_KEY_LEN + 1] = {0};
    hash_table_query_res res = {ERR_SYMBOL_OVERFLOW, NULL};

    if (iLength > MAX_KEY_LEN) return res;
    strncpy(sKey, pKey, iLength);

    res = hash_table_query(pSymTable, sKey);
    return res;
}

int hash_symtable_add(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, int iCFA) {
    /* iSize is the size of pValue */

    if (iLength > MAX_KEY_LEN) return ERR_SYMBOL_OVERFLOW;
    char sKey[MAX_KEY_LEN + 1] = {0};
    strncpy(sKey, pSymbolStr, iLength);

    hash_table_query_res QueryRes;
    QueryRes = hash_symtable_search(pSymTable, sKey, iLength);
    int iRet;

    hash_table_entry *pEntryTmp;
    pEntryTmp = hash_table_gen_entry(sKey, &iCFA, sizeof(int));
    hash_table_entry OldEntryTmp;

    if (QueryRes.pNode == NULL) {
        /* No such item */
        queue_push_back(&pSymTable->Data[QueryRes.idx], (void *) pEntryTmp, sizeof(hash_table_entry));
        pSymTable->iNumEntry++;
        return INFO_SYMBOL_NOT_FOUND;
    } else {
        /* Duplicated item, modify */
        iRet = queue_del_no_free(&pSymTable->Data[QueryRes.idx], QueryRes.pNode, (void *) &OldEntryTmp);
        if (iRet != TRUE) exit(ERR_SYMBOL_OVERFLOW);
        free(OldEntryTmp.pData);

        queue_push_back(&pSymTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry));
        return TRUE;
    }
}

int hash_symtable_get_cfa(hash_table_t *pSymTable, const char *pSymbolStr, int iLength) {
    /* iSize is the size of pValue */
    hash_table_query_res QueryRes;
    int iCFA;

    QueryRes = hash_symtable_search(pSymTable, pSymbolStr, iLength);
    if (QueryRes.pNode == NULL) {
        return VM_CFA_ERROR - 1;
    }
    iCFA = *((int *) ((hash_table_entry *) QueryRes.pNode->pData)->pData);
    return iCFA;
}

int hash_symtable_set_cfa(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, int iCFAVal) {
    hash_table_query_res QueryRes;

    QueryRes = hash_symtable_search(pSymTable, pSymbolStr, iLength);
    if (QueryRes.pNode == NULL) {
        return FALSE;
    }

    *((int *) ((hash_table_entry *) QueryRes.pNode->pData)->pData) = iCFAVal;
    return TRUE;
}