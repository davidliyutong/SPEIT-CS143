//
// Created by 厉宇桐 on 2020/12/26.
//

#ifndef LAC_SYMTABLE_H
#define LAC_SYMTABLE_H

# include "../common/hash_table.h"

typedef hash_table_t hash_symtable_t;

int hash_symtable_init(hash_table_t *pSymTable);

int hash_symtable_reset(hash_table_t *pSymTable);

int hash_symtable_checkout(hash_table_t *pSymTable);

int hash_symtable_revert(hash_table_t *pSymTable);

hash_table_query_res hash_symtable_search(hash_table_t *pSymTable, const char *pKey, int iLength);

int hash_symtable_add(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, int iCFA);

int hash_symtable_get_cfa(hash_table_t *pSymTable, const char *pSymbolStr, int iLength);

int hash_symtable_set_cfa(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, int iCFAVal);

#endif //LAC_SYMTABLE_H
