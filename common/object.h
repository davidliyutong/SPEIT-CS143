//
// Created by 厉宇桐 on 2020/12/28.
//
/**@file  object.h
* @brief    Types related to lac objects
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/
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

/** @enum struct vmtable_t
 * @brief 
 * @details VMTable is lac basic design(like bytecode in java) LAC functions are compiled as VMTable codes before execution.
 * @todo Develop a lac Syntax Tree structure, since the common workflow is LAC script -> Syntax Tree -> Bytecode -> Execution.
 */
typedef struct vmtable_t {
    int iMaxLength; /// The max length of the vm table
    int iTail; /// The place of last OpCode
    struct lac_object_t **Objects; /// Data 
    int iCheckPoint; /// Checkpoint marks the end of valid Objects
} vmtable_t;

/** @enum e_lac_object_type
 * @brief lac object function types
 * @details There are five types:
 * - LAC_FUNC: lac function, not only function but more like object. Variables defined inside function can be accessed as attribute
 * - LAC_INT: lac int@todo Develop lac float type, short int, long int
 * - LAC_VEC: lac vector@todo Develoop lac list(dynamic vector), dict(dynamic hash table)
 * - LAC_VAR: lac variable, can store vector or int @todo This can be replaced by a special LAC_FUNC that stores nothing but a vector or int
 * - LAC_CLASS: lac class, specially marked function template
 */
typedef enum e_lac_object_type {
    /* All types of lac objects */
    LAC_FUNC,
    LAC_INT,
    LAC_VEC,
    LAC_VAR,
    LAC_CLASS
} e_lac_object_type;

/** @enum e_vm_func_type
 * @brief VM function types
 * @details There are three types:
 * - VM_FUNC_BASIC -1 dup, drop, swap... basic functions
 * - VM_FUNC_LAC -2 lac functions
 * - VM_FUNC_CTRL -3 if, else, while... functions allowed in compile mode but not interpret mode
 */
typedef enum e_vm_func_type {
    /* There are three types of functions */
    VM_FUNC_BASIC = -1,
    VM_FUNC_LAC = -2,
    VM_FUNC_CTRL = -3,
} e_vm_func_type;

/** @struct lac_object_t
 * @brief lac object type
 * @details General lac object type, can represent any lac object
*/
typedef struct lac_object_t {
    int iRefCnt;                /// Count the reference of the object, if iRefCnt <= 0, the object is not accessible thus can be freed
    e_lac_object_type Type;     /// Type of object @related e_lac_object_type
    char Name[MAX_LEXEME_LEN];  /// Object name, for functions and classes, the name appears in the global / local symbol table.
                                /// @bug lac vectors and lac variables may also have a name, this is incoherent.
    void *Child;                /// Child, the data point of object
} lac_object_t;

/** @struct lac_func_t
 * @related lac_object
 * @brief lac function
 * @remark lac function has own VM Table and Symbol Table
 */
typedef struct lac_func_t {
    e_vm_func_type Type;    /// Type of function @related e_vm_func_type
    basic pFunc;                /// only basic function has pFunc
    int iCFA;                   /// only basic function has iCFA
    struct vmtable_t VMTable;
    hash_symtable_t SymTable;
} lac_func_t, lac_class_t;      /// lac_class type is synonym of lac_func_t

/** @struct lac_int_t
 * @related lac_object
 * @brief lac integer
 */
typedef struct lac_int_t {
    int iValue; /// value of integer
} lac_int_t;

/** @struct lac_var_t
 * @related lac_object
 * @brief lac variable
 */
typedef struct lac_var_t {
    bool bValid;            /// if the variable is valid
    e_lac_object_type Type; /// type of variable's content
    lac_object_t *Object;   /// pointer to the real content
} lac_var_t;

/** @struct lac_vec_t
 * @related lac_object
 * @brief lac vector
 * @todo Develop vector that can store more types of data
 */
typedef struct lac_vec_t {
    int iRef; /// reference of this vector
    int iLength; /// length of this vector
    int iSize;   /// size of vector element
    int *pData; /// vector only stores element smaller than int(4Byte)
} lac_vec_t;

/** @enum symbol_status
 * @brief The three status when adding a symbol to symbol table:
 * - INFO_SYMBOL_NOT_FOUND the symbol does not exist and is added
 * - ERR_SYMBOL_OVERFLOW the symbol is too long thus is not processed
 * - SYMBOL_OK the symbol exist, and is replaced
 */
typedef enum e_symbol_status {
    INFO_SYMBOL_NOT_FOUND = -1,
    ERR_SYMBOL_OVERFLOW = -2,
    SYMBOL_OK = TRUE
} e_symbol_status;

/**@var invalid_func
 * An instance of Invalid function, frequently used 
 * */
extern lac_func_t invalid_func;

/**@var vmtable_invalid_func
 * An instance of Invalid function, frequently used 
 * */
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

hash_table_query_res_t hash_symtable_search(hash_table_t *pSymTable, const char *pKey, int iLength);

e_symbol_status hash_symtable_add_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject);

lac_object_t *hash_symtable_get_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength);

lac_object_t *hash_symtable_get_obj_from_query(hash_table_query_res_t QueryRes);

bool hash_symtable_set_obj(hash_table_t *pSymTable, const char *pSymbolStr, int iLength, lac_object_t *pLACObject);

#endif //LAC_OBJECT_H
