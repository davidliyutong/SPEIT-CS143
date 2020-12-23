//
// Created by 厉宇桐 on 2020/11/21.
//
// Code to realise the symbol table

#include "symtable.h"

int symtable_init(symtable_t *pSymTable) {
    pSymTable->iMaxLength = INIT_SYM_TABLE_LEN;
    pSymTable->iLength = 0;
    pSymTable->iTail = 0;
    pSymTable->iNumSymbols = 0;
    pSymTable->Symbols = malloc(sizeof(struct symtable_t) * pSymTable->iMaxLength);

    if (pSymTable->Symbols == NULL) {
        printf("\n[ Warning ] Out of memory\n");
        exit(ERR_MEM);
    } else {
        pSymTable->Symbols[0].i = 0;
        pSymTable->iLength = 1;
        return TRUE;
    }
}

int symtable_destroy(symtable_t *pSymTable) {
    pSymTable->iMaxLength = -1; // A invalid number
    pSymTable->iLength = 0;
    pSymTable->iTail = 0;
    pSymTable->iNumSymbols = 0;
    free(pSymTable->Symbols);
    pSymTable->Symbols = NULL;
    return TRUE;
}

int symtable_clear(symtable_t *pSymTable) {
    symtable_destroy(pSymTable);
    return symtable_init(pSymTable);
}

int symtable_cmp(u_symbol *pSymTableLoc, u_symbol *pInput) {
    /* Compare the symbol with an extraction of symbol */
    /* The pInput points to a new symbol under the format: [l|{NAME}] */
    int iLength = pInput[0].i;
    for (int i = 0; i <= iLength; ++i) {
        if (pSymTableLoc[i].i != pInput[i].i || pSymTableLoc[i].c != pInput[i].c) {
            return FALSE;
        }
    }
    return TRUE;
}


int symtable_search(symtable_t *pTable, u_symbol *pInput) {
    /* Search the symbol in the table */
    int pTableLocIdx = pTable->iTail; // A pointer to the tail
    if (pTableLocIdx == 0) return FALSE; // The table is empty;
    while (TRUE) {
        /* Got a match */
        if (symtable_cmp(pTable->Symbols + pTableLocIdx, pInput)) {
            /* The return value is the place of symbol's length l */
            return pTableLocIdx;
        } else {
            /* Compare the previous symbol */
            pTableLocIdx = pTable->Symbols[pTableLocIdx - 1].i;
        }
        /* Reach the begining of table */
        if (pTableLocIdx <= 0) {
            return -1;
        }
    }
}

int symtable_expand(symtable_t *pSymTable) {
    /* If the pSymTable is not enough, then it need to be expanded */
    pSymTable->Symbols = (u_symbol *) realloc(pSymTable->Symbols,
                                              sizeof(u_symbol) * (pSymTable->iMaxLength + INIT_VM_TABLE_LEN));
    if (pSymTable->Symbols == NULL) {
        printf("\n[ Warning ] Out of memory\n");
        exit(ERR_MEM);
    }
    pSymTable->iMaxLength += INIT_VM_TABLE_LEN;
    return TRUE;
}

int symtable_add(symtable_t *pSymTable, const char *pSymbolStr, int iLength) {

    u_symbol SymbolInput[MAX_LEXEME_LEN];
    int iRet;
    iRet = str_to_symbol(pSymbolStr, iLength, SymbolInput);
    if (iRet != TRUE) {
        return FALSE;
    }

    /*
    iRet = symtable_search(pSymTable, SymbolInput);
    if (iRet > 0) { // Duplicate symbol;
        return FALSE;
    }
     */

    // The symbol is new;
    int iLengthTmp = pSymTable->iLength;
    if ((iLengthTmp + iLength + 4) >= pSymTable->iMaxLength) symtable_expand(pSymTable); // need to expand the table

    pSymTable->Symbols[iLengthTmp] = SymbolInput[0];
    for (int i = 1; i <= iLength; ++i) pSymTable->Symbols[iLengthTmp + i] = SymbolInput[i]; //input the symbol

    pSymTable->Symbols[iLengthTmp + 2 + iLength].i = pSymTable->iTail = iLengthTmp; // NFA
    pSymTable->iLength = iLengthTmp + 3 + iLength;
    pSymTable->iNumSymbols += 1;

    return TRUE;
}

int symtable_get_cfa(symtable_t *pSymTable, int iSymbolIdx) {
    /* This function returns the CFA value of a symbol in the symtable */
    /* The index of the symbol should be provided (l) */
    int iOffset = pSymTable->Symbols[iSymbolIdx].i;
    int iCFAVal = pSymTable->Symbols[iSymbolIdx + iOffset + 1].i;
    return iCFAVal;

}

int symtable_set_cfa(symtable_t *pSymTable, int iSymbolIdx, int iCFAVal) {
    /* This function set the CFA value of a symbol in the symtable */
    /* The index of the symbol should be provided (l) */
    int iOffset = pSymTable->Symbols[iSymbolIdx].i;
    pSymTable->Symbols[iSymbolIdx + iOffset + 1].i = iCFAVal;
    return TRUE;
}