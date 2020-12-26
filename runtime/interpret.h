//
// Created by 厉宇桐 on 2020/11/26.
//

#ifndef LAC_INTERPRET_H
#define LAC_INTERPRET_H

#include <string.h>
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/types.h"
#include "../lex/analex.h"
#include "../runtime/symtable.h"
#include "../runtime/vmtable.h"
#include "../utils/calculate.h"
#include "env.h"
#include "proc.h"
#include "function.h"

typedef enum e_interpret_keyword {
    INTERPRET_DEF, // :
    INTERPRET_DEFER,
    INTERPRET_LINK,
    INTERPRET_VAR,
    INTERPRET_VEC,
    INTERPRET_IMPORT,
    INTERPRET_EXIT,
    INTERPRET_DEFAULT
} e_interpret_stat;

enum e_interpret_keyword match_keyword(char *psReadBuffer, struct lexeme_t LexTmp);

void exec_vm(int iCFA);

void interpret(char *psReadBuffer, lac_queue_t *pqueRes);

#endif //LAC_INTERPRET_H