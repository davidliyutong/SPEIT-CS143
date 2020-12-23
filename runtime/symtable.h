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
#include "../utils/convert.h"

int symtable_init(symtable_t *pSymTable);

int symtable_destroy(symtable_t *pSymTable);

int symtable_clear(symtable_t *pSymTable);

int symtable_cmp(u_symbol *pSymTableLoc, u_symbol *pInput);

int symtable_search(symtable_t *pTable, u_symbol *pInput);

int symtable_expand(symtable_t *pSymTable);

int symtable_add(symtable_t *pSymTable, const char *pSymbolStr, int iLength);

int symtable_get_cfa(symtable_t *pSymTable, int iSymbolIdx);

int symtable_set_cfa(symtable_t *pSymTable, int iSymbolIdx, int iCFAVal);

#endif //LAC_SYMTABLE_H
