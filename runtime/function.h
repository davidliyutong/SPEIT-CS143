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
#include "../runtime/symtable.h"
#include "../runtime/vmtable.h"
#include "../utils/calculate.h"
#include "../utils/convert.h"
#include "env.h"
#include "interpret.h"
#include "proc.h"

void compile_function(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_function(char *psReadBuffer, lac_queue_t *pqueRes);

void link_declaration(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_var(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_vec(char *psReadBuffer, lac_queue_t *pqueRes);

#endif //LAC_FUNCTION_H
