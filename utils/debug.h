//
// Created by 厉宇桐 on 2020/12/1.
//

#ifndef LAC_DEBUG_H
#define LAC_DEBUG_H

#include <stdio.h>
#include "../common/btree.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/object.h"
#include "../lex/analex.h"

#define COLOR_NUMBER 34
#define COLOR_WORD 33
#define COLOR_STRING 32

void disp_annalex_res(char *psReadBuffer, lac_queue_t *pQueRes);

void disp_postfix_exp(lac_queue_t *quePostfixExp);

void disp_exp_tree(exp_btnode_t ExpTreeRoot);

void disp_symtable(hash_symtable_t *pSymTable);

void disp_vmtable(vmtable_t *pVMTable);

void disp_objects(lac_queue_t *pQueObjects);

void visualize(char *psReadBuffer, lac_queue_t *pQueRes);

#endif //LAC_DEBUG_H
