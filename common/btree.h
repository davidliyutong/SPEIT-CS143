//
// Created by 厉宇桐 on 2020/11/21.
//
/**@file  btree.h
* @brief    Types related to binary tree operation
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/
#ifndef LAC_BTREE_H
#define LAC_BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

/**@enum e_btnode_types
 * Define the type of btnode in expression tree
 */
typedef enum e_btnode_types {
    CALC_NAN, /// N/A
    CALC_NUMBER, /// numbers
    CALC_OP_PAR_LEFT, /// (
    CALC_OP_PAR_RIGHT, /// )
    CALC_OP_1, /// - +
    CALC_OP_2, /// x /
    CALC_OP_3 /// - + (sign)
} e_btnode_types;

/**@struct exp_btnode_t
 * The btnode structure, used in project2 and calculate.c
 */
typedef struct exp_btnode_t {
    struct exp_btnode_t *lNode; /// Left node
    struct exp_btnode_t *rNode; /// Right node
    e_btnode_types type; /// Type of node
    union u_Data {  /// Value of the node, could be a int or char 
        int iNumber; /// if the node is number
        char cOP; /// if the node is an operator.
    } u_Data;
    union u_Value { /// Value of the entire subtree
        int iNumber;
    } u_Value;
} exp_btnode_t;


void btree_destroy(exp_btnode_t *pTreeNode);

#endif //LAC_BTREE_H
