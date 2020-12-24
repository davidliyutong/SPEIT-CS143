//
// Created by 厉宇桐 on 2020/11/18.
//
// Definition of custom types

#ifndef LAC_TYPES_H
#define LAC_TYPES_H

#include "macros.h"

typedef int bool;

typedef enum e_lexeme_types {
    WORD,
    STRING,
    NUMBER,
} e_lexeme_types;

typedef struct lexeme_t {
    e_lexeme_types type;
    int iStart;
    int iEnd;
} lexeme_t;

typedef struct queue_node_t {
    void *pData;
    int iNumBytes;
    struct queue_node_t *pPrev;
    struct queue_node_t *pNext;
} queue_node_t;

typedef struct lac_queue_t {
    int iLength;
    struct queue_node_t *pFront;
    struct queue_node_t *pRear;
} lac_queue_t;

typedef struct stack_node_t {
    void *pData;
    int iNumBytes;
    struct stack_node_t *pPrev;
} stack_node_t;

typedef struct lac_stack_t {
    int iLength;
    struct stack_node_t *pTop;
} lac_stack_t;

enum calculator_stat {
    /* When the calculator is running, there are possible states */
    CALC_STAT_BEGIN,
    CALC_STAT_NUMBER,
    CALC_STAT_OP,
    CALC_STAT_OP_PAR_LEFT,
    CALC_STAT_OP_PAR_RIGHT,
    CALC_STAT_FIN,
    CALC_STAT_ERR
};

typedef enum e_btnode_types {
    CALC_NAN,
    CALC_NUMBER,
    CALC_OP_PAR_LEFT,
    CALC_OP_PAR_RIGHT,
    CALC_OP_1, // - +
    CALC_OP_2, // x /
    CALC_OP_3 // - + (sign)
} e_btnode_types;

typedef struct exp_btnode_t {
    struct exp_btnode_t *lNode;
    struct exp_btnode_t *rNode;
    e_btnode_types type;
    union u_Data {
        int iNumber;
        char cOP;
    } u_Data;
    union u_Value {
        int iNumber;
    } u_Value;
} exp_btnode_t;

typedef union u_symbol {
    int i;
    char c;
} u_symbol;

typedef struct symtable_t {
    int iMaxLength; // The max length of the vm table
    int iLength; // The current used length of the sym table, also the tail of table
    /* iLength - 2 is the last symbol's CFA, NOT VALID until prepared*/
    /* iLength - 1 the the begin of the next symbol, NOT VALID*/

    int iNumSymbols;
    int iTail; // The place of last defined symbol's NfA
    int iCheckPoint; // The checkpoint
    u_symbol *Symbols;
} symtable_t;


/* The 2 table struc can be summariez to a <vector> type [!] */
typedef struct vmtable_t {
    int iMaxLength; // The max length of the vm table
    int iTail; // The place of last OpCode
    int *OpCodes;
    int iCheckPoint;
} vmtable_t;


typedef struct vmenv_t {
    lac_stack_t StkData;        // Data stack
    lac_stack_t StkReturn;      // Return stack
    symtable_t SymTable;    // Table of symbols
    vmtable_t VMTable;      // VM table
//    strtable_t StrTable;    // String table
    bool bCompiled;         // Flag
    bool bInited;           // Flag
} vmenv_t;

#endif
