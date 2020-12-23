//
// Created by 厉宇桐 on 2020/11/21.
//
#include "../runtime/symtable.h"
#include "../utils/debug.h"

int main() {

    symtable_t SymTable;
    symtable_init(&SymTable);
    symtable_add(&SymTable, "+", 1);
    symtable_add(&SymTable, "swap", 4);
    symtable_add(&SymTable, "swap", 4);
    symtable_add(&SymTable, "swapp", 5);
    disp_symtable(&SymTable);

    return 0;
}
