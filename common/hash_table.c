//
// Created by 厉宇桐 on 2020/12/25.
//

#include "hash_table.h"

inline unsigned int hash_table_compute_hash(unsigned char *input) {
    /* Compute the unsigned int hash value of a key, using MD5 and hash33 */
    unsigned int iHashValue = str_to_hash(input);
    return iHashValue % HASH_TABLE_LEN;
}

hash_table_entry *hash_table_gen_entry(const char *sKey, void *pValue, int iSize) {
    /* pValue that have dynamically located value should be treated carefully */
    hash_table_entry *pEntry = malloc(sizeof(hash_table_entry));

    if (pEntry == NULL) {
        return pEntry;
    } else {
        pEntry->pData = malloc((size_t) iSize);
        if (pEntry->pData == NULL) {
            free(pEntry);
            return NULL;
        }
    }
    if (sKey != NULL) {
        strncpy(pEntry->Key, sKey, MAX_KEY_LEN);
    }
    if (pValue != NULL) {
        memcpy(pEntry->pData, pValue, (size_t) iSize);
    }

    return pEntry;
}

bool hash_table_reset(hash_table_t *pHashTable) {
    pHashTable->iNumEntry = 0;
    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        pHashTable->CheckPoints[idx] = 0;
    }
    hash_table_revert(pHashTable);
    return TRUE;
}

bool hash_table_init(hash_table_t *pHashTable) {
    pHashTable->iNumEntry = 0;
    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        queue_init(&pHashTable->Data[idx]);
        pHashTable->CheckPoints[idx] = 0;
    }
    return TRUE;
}

bool hash_table_revert(hash_table_t *pHashTable) {
    /* Revert hash_table */
    hash_table_entry OldEntryTmp;

    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        while (pHashTable->Data[idx].iLength > pHashTable->CheckPoints[idx]) {
            queue_pop_back_no_free(&pHashTable->Data[idx], (void *) &OldEntryTmp);
            free(OldEntryTmp.pData);
            pHashTable->iNumEntry--;
        }
    }
    return TRUE;
}

bool hash_table_checkout(hash_table_t *pHashTable) {
    /* Checkout hash_table */
    pHashTable->iNumEntry = 0;
    for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
        pHashTable->CheckPoints[idx] = pHashTable->Data[idx].iLength;
        pHashTable->iNumEntry += pHashTable->CheckPoints[idx];
    }
    return TRUE;
}

bool hash_table_entry_is_empty(hash_table_t *pHashTable, int iHashTableIdx) {
    return (pHashTable->Data[iHashTableIdx].iLength > 0) ? FALSE : TRUE;
}

hash_table_query_res hash_table_query(hash_table_t *pHashTable, char *sKey) {
    hash_table_query_res res = {-1, NULL}; // Query result
    int iCnt = 0;

    res.idx = (int) hash_table_compute_hash((unsigned char *) sKey); // get the hash table index from key
    if (hash_table_entry_is_empty(pHashTable, res.idx)) { // the hash table is empty
        return res;
    } else {
        queue_node_t *pNode = pHashTable->Data[res.idx].pFront;
        while (pNode != NULL) {
            iCnt++;
            /* Got a match, and the range is within checkpoint, specially useful when defining symbols such as X.X */
            if (strncmp(((hash_table_entry *) (pNode->pData))->Key, sKey, MAX_KEY_LEN) == 0 && iCnt <= pHashTable->CheckPoints[res.idx]) {
                res.pNode = pNode;
                return res;
            }

            queue_next(&pNode); // move to the next node
        }
        return res;
    }
}

hash_table_query_res hash_table_add(hash_table_t *pHashTable, char *sKey, void *pValue, int iSize) {
    /* iSize is the size of pValue */

    hash_table_query_res QueryRes;
    QueryRes = hash_table_query(pHashTable, sKey);
    int iRet;

    hash_table_entry *pEntryTmp;
    pEntryTmp = hash_table_gen_entry(sKey, pValue, iSize);
    hash_table_entry OldEntryTmp;

    if (QueryRes.pNode == NULL) {
        /* No such item */
        queue_push_back(&pHashTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry));
        pHashTable->iNumEntry++;
        return QueryRes;
    } else {
        /* Duplicated item, modify */
        iRet = queue_del_no_free(&pHashTable->Data[QueryRes.idx], QueryRes.pNode, (void *) &OldEntryTmp, TRUE);
        free(OldEntryTmp.pData);

        if (iRet != TRUE) exit(ERR_NOT_IMPLEMENTED);
        queue_push_back(&pHashTable->Data[QueryRes.idx], pEntryTmp, sizeof(hash_table_entry));
        return QueryRes;
    }
}