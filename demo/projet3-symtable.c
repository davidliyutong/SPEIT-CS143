//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet3 demo

#include "../common/object.h"
#include "../runtime/env.h"

int main() {
    g_env_init();
    hash_symtable_t SymTable;
    hash_symtable_init(&SymTable);
    char symbols[4][6] = {"+", "swap", "swap", "swapp"};
    lac_object_t *pLACObjectTmp;
    int iRet;
    for (int i = 0; i < 4; i++) {
        pLACObjectTmp = env_create_lac_func(symbols[i], strlen(symbols[i]), 0, VM_FUNC_BASIC, NULL);
        iRet = hash_symtable_add_obj(&SymTable, symbols[i], (int) strlen(symbols[i]), pLACObjectTmp);
        if (iRet > 0) {
            printf("\n[ Warning ] Duplicated symbol found: %s\n", symbols[i]);
        }
    }
    hash_symtable_checkout(&SymTable);
    disp_symtable(&SymTable);
    return 0;
}
