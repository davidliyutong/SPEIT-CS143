//
// Created by 厉宇桐 on 2020/11/19.
//
// Code to realise the VM table


#ifndef LAC_VMTABLE_H
#define LAC_VMTABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "../common/macros.h"
#include "../common/stack.h"
#include "../common/types.h"

typedef enum e_opcode_type {
    OP_CODE_DATA, // Data, Vector, Variable.
    OP_CODE_INST, // Function codes.
    OP_CODE_NULL, // Default
} e_opcode_type;

/* The 2 table struc can be summariez to a <vector> type [!] */
typedef struct vmtable_t {
    int iMaxLength; // The max length of the vm table
    int iTail; // The place of last OpCode
    int *OpCodes;
    e_opcode_type *OpCodeTypes; // Permission of OpCodes
    int iCheckPoint;
} vmtable_t;

int vmtable_init(vmtable_t *pVMTable);

int vmtable_expand(vmtable_t *pVMTable);

int vmtable_del(vmtable_t *pVMTable);

int vmtable_checkout(vmtable_t *pVMTable);

int vmtable_revert(vmtable_t *pVMTable);

int vmtable_add(vmtable_t *pVMTable, int iOpCode, e_opcode_type Type);

int vmtable_add_vec(vmtable_t *pVMTable, void *pVec, int iLen, int iSize);

int vmtable_strlen(vmtable_t *pVMTable, int iAddr);

//int vmtable_clear(vmtable_t *pVMTable);

#endif //LAC_VMTABLE_H
