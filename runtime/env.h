//
// Created by 厉宇桐 on 2020/12/23.
//

#ifndef LAC_ENV_H
#define LAC_ENV_H

#include <stdio.h>
#include <stdlib.h>
#include "../common/stack.h"
#include "symtable.h"
#include "vmtable.h"

typedef struct vmenv_t {
    lac_stack_t StkData;        // Data stack
    lac_stack_t StkReturn;      // Return stack
    hash_symtable_t SymTable;    // Table of symbols
    vmtable_t VMTable;      // VM table
    bool bCompiled;         // Flag
    bool bInited;           // Flag
} vmenv_t;

extern vmenv_t g_Env;

int stack_push_vm(lac_stack_t *pStack, int iData);

int stack_pop_vm(lac_stack_t *pStack);

int stack_top_vm(lac_stack_t *pStack);

#endif //LAC_ENV_H
