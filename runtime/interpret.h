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
#include "../utils/calculate.h"
#include "env.h"
#include "proc.h"
#include "function.h"
#include "../common/cfa.h"

void exec_vm(lac_object_t *pLACFunctionObject);

void interpret(lac_queue_t *pQueRes, compile_stat_t CompileStat);

#endif //LAC_INTERPRET_H