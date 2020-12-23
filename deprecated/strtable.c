//
// Created by 厉宇桐 on 2020/12/1.
//
// Functions used to realize a string table

#include "strtable.h"

int strtable_init(strtable_t *pStrTable) {
    /* Init a strtable defined outside of function */

    pStrTable->iMaxLength = INIT_STR_TABLE_LEN;
    pStrTable->iTail = -1;
    pStrTable->cStr = malloc(sizeof(char) * pStrTable->iMaxLength);
    memset(pStrTable->cStr, -3, sizeof(char) * pStrTable->iMaxLength);

    if (pStrTable->cStr == NULL) {
        printf("\n[ Warning ] Out of memory\n");
        exit(ERR_MEM);
    }

    return TRUE;
}

int strtable_expand(strtable_t *pStrTable) {
    /* If the pStrTable is not enough, then it need to be expanded */

    pStrTable->cStr = (char *) realloc(pStrTable->cStr, sizeof(char) * (pStrTable->iMaxLength + INIT_VM_TABLE_LEN));
    if (pStrTable->cStr == NULL) {
        printf("\n[ Warning ] Out of memory\n");
        exit(ERR_MEM);
    }
    pStrTable->iMaxLength += INIT_VM_TABLE_LEN;
    return TRUE;
}

int strtable_add(strtable_t *pStrTable, const char *pStr, int iLength) {
    /* This function put a string in to the strtable */
    /* iLength: the strlen() result of pStr */
#ifdef DEBUG
    printf("\n[ Debug ] Put %.*s into StrTable[%d]\n", iLength, pStr, pStrTable->iTail + 1);
#endif
    for (int i = 0; i < iLength; ++i) {
        if (pStrTable->iTail >= pStrTable->iMaxLength - 2) {
            strtable_expand(pStrTable);
        }
        pStrTable->cStr[++pStrTable->iTail] = pStr[i];
        /* cann use memset instead [!] */
    }
    /* Put a 0 at the end of string */
    pStrTable->cStr[++pStrTable->iTail] = 0;

    return TRUE;
}

int strtable_clear(strtable_t *pStrTable) {
    /* Clear the strtable */

    pStrTable->iMaxLength = INIT_VM_TABLE_LEN;
    pStrTable->iTail = -1;
    free(pStrTable->cStr);
    pStrTable->cStr = NULL;
    pStrTable->cStr = malloc(sizeof(char) * pStrTable->iMaxLength);
    memset(pStrTable->cStr, -3, sizeof(char) * pStrTable->iMaxLength);

    if (pStrTable->cStr == NULL) {
        printf("\n[ Warning ] Out of memory\n");
        exit(ERR_MEM);
    }

    return TRUE;
}
