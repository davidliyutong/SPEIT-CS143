//
// Created by 厉宇桐 on 2020/12/28.
//

#ifndef LAC_OBJECT_H
#define LAC_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "macros.h"
#include "types.h"
#include "hash_table.h"
#include "cfa.h"

typedef hash_table_t hash_symtable_t;

struct lac_object_t;
struct vmtable_t;

typedef struct vmtable_t {
    int iMaxLength; // The max length of the vm table
    int iTail; // The place of last OpCode
    struct lac_object_t **Objects;
    int iCheckPoint;
} vmtable_t;


typedef enum e_lac_object_type {
    /* All types of lac objects */
    LAC_FUNC,
    LAC_INT,
    LAC_VEC,
    LAC_VAR,
    LAC_CLASS
} e_lac_object_type;

typedef enum e_vm_func_type {
    /* There are three types of functions */
    VM_FUNC_BASIC = -1,
    VM_FUNC_LAC = -2,
    VM_FUNC_CTRL = -3,
} e_vm_func_type;

typedef struct lac_object_t {
    /* Base class */
    int iRefCnt;
    e_lac_object_type Type;
    char Name[MAX_LEXEME_LEN];
    void *Child;
} lac_object_t;

typedef struct lac_func_t {
    e_vm_func_type FuncType;
    basic pFunc;                // only basic function has pFunc
    int iCFA;                   // only basic function has iCFA
    struct vmtable_t VMTable;
    hash_symtable_t SymTable;
} lac_func_t, lac_class_t;

typedef struct lac_int_t {
    /* lac int */
    int iValue;
} lac_int_t;

typedef struct lac_var_t {
    /* lac variable */
    bool bValid;            /* if the variable is valid */
    e_lac_object_type Type; /* type of variable's content */
    lac_object_t *Object;   /* pointer to the real content */
} lac_var_t;

typedef struct lac_vec_t {
    /* lac vector */
    int iRef; /* reference of this vector */
    int iLength;
    int iSize;
    int *pData;
} lac_vec_t;

typedef enum symbol_status {
    INFO_SYMBOL_NOT_FOUND = -1,
    ERR_SYMBOL_OVERFLOW = -2,
    SYMBOL_OK = TRUE
} symbol_status;

/* An instance of Invalid function, frequently used */
extern lac_func_t invalid_func;
extern lac_object_t vmtable_invalid_func;

bool vmtable_init(vmtable_t *pVMTable);

bool vmtable_expand(vmtable_t *pVMTable);

bool vmtable_del(vmtable_t *pVMTable);

bool vmtable_checkout(vmtable_t *pVMTable);

bool vmtable_revert(vmtable_t *pVMTable);

bool vmtable_add(vmtable_t *pVMTable, lac_object_t *pLACObject);

bool vmtable_set(vmtable_t *pVMTable, int idx, lac_object_t *pLACObject);

int vmtable_strlen(lac_vec_t *pVec);

bool vmtable_clear(vmtable_t *pVMTable);


bool hash_symtable_init(hash_table_t *pSymTable);

bool hash_symtable_reset(hash_table_t *pSymTable);

bool hash_symtable_checkout(hash_table_t *pSymTable);

bool hash_symtable_revert(hash_table_t *pSymTable);

hash_table_query_res hash_symtable_search(hash_table_t *pSymTable, const char *pKey, int iLength);

symbol_status hash_symtable_add_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject);

lac_object_t *hash_symtable_get_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength);

lac_object_t *hash_symtable_get_obj_from_query(hash_table_query_res QueryRes);

bool hash_symtable_set_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject);

#endif //LAC_OBJECT_H
