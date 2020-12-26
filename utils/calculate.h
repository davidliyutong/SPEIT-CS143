//
// Created by 厉宇桐 on 2020/11/19.
//

#ifndef LAC_CALCULATE_H
#define LAC_CALCULATE_H

#include <stdio.h>
#include <string.h>

#include "../common/btree.h"
#include "../common/macros.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/types.h"
#include "../utils/debug.h"
#include "../utils/io.h"

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

exp_btnode_t *gen_exp_BTNode();

int calculate_2_op_int(int iOperand1, int iOperand2, int *ans, char cOperator);

int decode_exp(lac_queue_t *pqueSymbols, const char *sExpression, long lNumBytes);

int gen_postfix_exp(lac_queue_t *pquePostfixExp, lac_queue_t *pqueSymbols);

int build_exp_tree(lac_queue_t *pquePostfixExp, exp_btnode_t *pExpTreeRoot);

int calculate_exp_int(const char *sExpression, int iNumBytes, int *res);

#endif //LAC_CALCULATE_H
