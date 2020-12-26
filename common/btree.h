//
// Created by 厉宇桐 on 2020/11/21.
//

#ifndef LAC_BTREE_H
#define LAC_BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

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

void btree_destroy(exp_btnode_t *pTreeNode);

#endif //LAC_BTREE_H
