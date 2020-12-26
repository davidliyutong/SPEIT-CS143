//
// Created by 厉宇桐 on 2020/12/26.
//

#include "hash_symtable.h"

int hash_symtable_init(hash_table_t *pSymTable) {
    return hash_table_init(pSymTable);
}

int hash_symtable_reset(hash_table_t *pSymTable) {
    return hash_table_reset(pSymTable);
}

int hash_symtable_checkout(hash_table_t *pSymTable, int iHashTableIdx) {
    return hash_table_checkout(pSymTable);
}

int hash_symtable_revert(hash_table_t *pSymTable) {
    return hash_table_revert(pSymTable);
}

hash_table_query_res hash_symtable_search(hash_table_t *pSymTable, char *pKey) {
    return hash_table_query(pSymTable, pKey);
}

int hash_symtable_add(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, int iCFA) {
    /* iSize is the size of pValue */

    char sKey[MAX_KEY_LEN + 1];
    strncpy(sKey, pSymbolStr, iLength);
    hash_table_query_res QueryRes;
    QueryRes = hash_table_query(pSymTable, sKey);
    int iRet;

    hash_table_entry *pEntryTmp;
    pEntryTmp = hash_table_gen_entry(sKey, &iCFA, sizeof(int));
    hash_table_entry OldEntryTmp;

    if (QueryRes.pNode == NULL) {
        /* No such item */
        queue_push_back(&pSymTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry));
        pSymTable->iNumEntry++;
        return INFO_SYMBOL_NOT_FOUND;
    } else {
        /* Duplicated item, modify */
        iRet = queue_del_no_free(&pSymTable->Data[QueryRes.idx], QueryRes.pNode, (void *) &OldEntryTmp);
        if (iRet != TRUE) exit(ERR_NOT_IMPLEMENTED);
        free(OldEntryTmp.pData);

        queue_push_back(&pSymTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry));
        return TRUE;
    }
}

int hash_symtable_get_cfa(hash_table_t *pSymTable, char *pSymbolStr, int iLength) {
    /* iSize is the size of pValue */
    hash_table_query_res QueryRes;
    hash_table_entry iEntryTmp;
    char sKey[MAX_KEY_LEN + 1];
    int iCFA;

    strncpy(sKey, pSymbolStr, iLength);

    QueryRes = hash_table_query(pSymTable, sKey);
    if (QueryRes.pNode == NULL) {
        return VM_CFA_ERROR;
    }
    iCFA = *((int *) ((hash_table_entry *) QueryRes.pNode->pData)->pData);
    return iCFA;
}

int hash_symtable_set_cfa(hash_table_t *pSymTable, char *pSymbolStr, int iLength, int iCFAVal) {
    hash_table_query_res QueryRes;
    hash_table_entry iEntryTmp;
    char sKey[MAX_KEY_LEN + 1];

    strncpy(sKey, pSymbolStr, iLength);

    QueryRes = hash_table_query(pSymTable, sKey);
    if (QueryRes.pNode == NULL) {
        return FALSE;
    }
    
    *((int *) ((hash_table_entry *) QueryRes.pNode->pData)->pData) = iCFAVal;
    return TRUE;
}