//
// Created by 厉宇桐 on 2020/12/1.
//

#ifndef LAC_DEBUG_H
#define LAC_DEBUG_H

#include <stdio.h>
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/btree.h"
#include "../lex/analex.h"
#include "../runtime/symtable.h"
#include "../runtime/vmtable.h"

void disp_annalex_res(char *psReadBuffer, lac_queue_t *pqueRes);

void disp_postfix_exp(lac_queue_t *quePostfixExp);

void disp_exp_tree(exp_btnode_t ExpTreeRoot);

void disp_symtable(symtable_t *pSymTable);

void disp_vmtable(vmtable_t *pVMTable);

void visualize(char *psReadBuffer, lac_queue_t *pqueRes);

#endif //LAC_DEBUG_H
