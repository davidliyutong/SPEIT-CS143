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
#include "../utils/convert.h"
#include "env.h"
#include "proc.h"
#include "function.h"

void exec_vm(int iCFA);

void interpret(char *psReadBuffer, lac_queue_t *pqueRes);

#endif //LAC_INTERPRET_H