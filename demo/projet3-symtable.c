//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet3 demo

#include "../runtime/symtable.h"
#include "../utils/debug.h"

int main() {

    hash_symtable_t SymTable;
    hash_symtable_init(&SymTable);
    char symbols[4][6] = {"+", "swap", "swap", "swapp"};
    int iRet;
    for (int i = 0; i < 4; i++) {
        iRet = hash_symtable_add(&SymTable, symbols[i], (int) strlen(symbols[i]), 0);
        if (iRet > 0) {
            printf("\n[ Warning ] Duplicated symbol found: %s\n", symbols[i]);
        }
    }
    hash_symtable_checkout(&SymTable);
    disp_symtable(&SymTable);
    return 0;
}
