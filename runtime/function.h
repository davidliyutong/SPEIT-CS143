//
// Created by 厉宇桐 on 2020/12/23.
//

#ifndef LAC_FUNCTION_H
#define LAC_FUNCTION_H

#include <string.h>
#include "../common/macros.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/types.h"
#include "../utils/calculate.h"
#include "env.h"
#include "proc.h"
#include "../common/cfa.h"
#include "../common/object.h"


typedef struct compile_stat_t {
    /* Compile mode */
    bool bIsClass;
    bool bIf;
    bool bWhile;
    int iIfStart;
    int iElseStart;
    int iWhileStart;
    /* Interpret mode */
    lac_func_t *pFuncCurr;
    char sScopeName[MAX_LEXEME_LEN];
    char sModuleName[MAX_LEXEME_LEN]; // for import

    bool bInterpretIfTaken;
    bool bInterpretWhileTaken;
} compile_stat_t;

bool compile_function(lac_queue_t *pQueRes, compile_stat_t CompileStat);

hash_table_query_res_t hash_symtable_search_all(lexeme_t LexTmp, compile_stat_t CompileStat);

void declare_function(lac_queue_t *pQueRes, compile_stat_t CompileStat);

void link_declaration(lac_queue_t *pQueRes, compile_stat_t CompileStat);

void declare_var(lac_queue_t *pQueRes, compile_stat_t CompileStat);

void declare_vec(lac_queue_t *pQueRes, compile_stat_t CompileStat);

#endif //LAC_FUNCTION_H
