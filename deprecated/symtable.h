//
// Created by 厉宇桐 on 2020/11/21.
//

#ifndef LAC_SYMTABLE_H
#define LAC_SYMTABLE_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "../common/macros.h"
#include "../common/types.h"

typedef union u_symbol {
    int i;
    char c;
} u_symbol;

typedef struct symtable_t {
    int iMaxLength; // The max length of the vm table
    int iLength; // The current used length of the sym table, also the tail of table
    /* iLength - 2 is the last symbol's CFA, NOT VALID until prepared*/
    /* iLength - 1 the the begin of the next symbol, NOT VALID*/

    int iNumSymbols;
    int iTail; // The place of last defined symbol's NfA
    int iCheckPoint; // The checkpoint
    u_symbol *Symbols;
} symtable_t;

int symtable_init(symtable_t *pSymTable);

//int symtable_destroy(symtable_t *pSymTable);

//int symtable_clear(symtable_t *pSymTable);

int symtable_search(symtable_t *pTable, u_symbol *pInput);

int symtable_checkout(symtable_t *pSymTable);

int symtable_revert(symtable_t *pSymTable);

int symtable_add(symtable_t *pSymTable, const char *pSymbolStr, int iLength);

int symtable_get_cfa_by_idx(symtable_t *pSymTable, int iSymbolIdx);

int symtable_set_cfa_by_idx(symtable_t *pSymTable, int iSymbolIdx, int iCFAVal);

int symtable_set_cfa_by_name(symtable_t *pSymTable, char *pSymbolStr, int iLength, int iCFAVal);

#endif //LAC_SYMTABLE_H
