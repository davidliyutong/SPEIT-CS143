//
// Created by 厉宇桐 on 2020/12/23.
//
/**@file  env.h
* @brief    LAC environment 
* @details  
* @author      厉宇桐
* @date        2020/12/23
*/
#ifndef LAC_ENV_H
#define LAC_ENV_H

#include <stdio.h>
#include <stdlib.h>
#include "../common/macros.h"
#include "../common/stack.h"
#include "../common/object.h"
#include "proc.h"

/** @struct vmenv_t
 * @brief This struct stores the LAC execution environment
 */
typedef struct vmenv_t {
    lac_stack_t StkData;        /// Data stack
    lac_stack_t StkReturn;      /// Return stack
    hash_symtable_t SymTable;   /// Table of symbols, global
    vmtable_t *pLACVMTable;     /// pointer to the VM table of certain lac function
    vmtable_t BasicFuncTable;   /// VM table of basic functions
    bool bInited;               /// Flag. TRUE if the environment is inited
    lac_queue_t *pQueRes;       /// Queue to store lexical analyse results
    lac_queue_t Objects;        /// Queue stores all dynamically created objects
} vmenv_t;

extern vmenv_t g_Env;


lac_object_t *env_create_lac_func(char *FuncNameStr, int iNameStrLength, int iCFA, e_vm_func_type FuncType, basic pFunc);

lac_object_t *env_create_lac_class(char *FuncNameStr, const int iNameStrLength, int iCFA, e_vm_func_type FuncType, basic pFunc);

lac_object_t *env_create_lac_int(int iValue);

lac_object_t *env_create_lac_var(const char *VarNameStr, int iNameStrLength, lac_object_t *pLACObject);

lac_object_t *env_create_lac_vec(const char *VecNameStr, int iNameStrLength, int iLength, int iSize, void *pData);

bool env_set_lac_var(lac_object_t *pLACVAR, lac_object_t *pLACObject);

bool env_instance_class(lac_object_t *pLACClass, lac_object_t *pLACInstance);

int destroy_lac_obj(lac_object_t *pLACObject);

//int reset_lac_obj(lac_object_t *pLACObject);

bool stack_push_data(lac_stack_t *pStack, lac_object_t *pLACObject);

lac_object_t *stack_pop_data(lac_stack_t *pStack);

lac_object_t *stack_top_data(lac_stack_t *pStack);

int stack_push_return(lac_stack_t *pStack, int iData);

int stack_pop_return(lac_stack_t *pStack);

int stack_top_return(lac_stack_t *pStack);

#endif