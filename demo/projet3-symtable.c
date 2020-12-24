//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet3 demo

#include "../runtime/symtable.h"
#include "../utils/debug.h"

int main() {

    symtable_t SymTable;
    symtable_init(&SymTable);
    char symbols[4][6] = {"+", "swap", "swap", "swapp"};
    int iRet = 0;
    for (int i = 0; i < 4; i++) {
        iRet = symtable_add(&SymTable, symbols[i], strlen(symbols[i]));
        if (iRet > 0) {
            printf("\n[ Warning ] Duplicated symbol found: %s\n", symbols[i]);
        }
    }
    symtable_checkout(&SymTable);
    disp_symtable(&SymTable);

    return 0;
}
