//
// Created by 厉宇桐 on 2020/11/19.
//
// Code that performs calculate

#include "calculate.h"

inline exp_btnode_t *gen_exp_BTNode() {
    /* Dynamically allocate a new Binary Tree Node of expression */
    exp_btnode_t *pBTNode = (exp_btnode_t *) malloc(sizeof(exp_btnode_t));
    if (pBTNode == NULL) {
        printf("\n [ Error ] Memory error\n");
        exit(ERR_MEM);
    }
    pBTNode->lNode = NULL;
    pBTNode->rNode = NULL;
    pBTNode->type = CALC_NAN;
    pBTNode->u_Value.iNumber = 0;
    return pBTNode;
}

inline int calculate_2_op_int(int iOperand1, int iOperand2, int *ans, char cOperator) {
    /* Calculate the return value of a 2 operand expression */

    switch (cOperator) {
        case '+':
            *ans = iOperand1 + iOperand2;
            return CALC_STAT_FIN;
        case '-':
            *ans = iOperand1 - iOperand2;
            return CALC_STAT_FIN;
        case 'x':
            *ans = iOperand1 * iOperand2;
            return CALC_STAT_FIN;
        case '/':
            *ans = iOperand1 / iOperand2;
            return CALC_STAT_FIN;
        default:
            printf("\n[ Warning ] Syntax Error, operator %c not recongized\n", cOperator);
            return CALC_STAT_ERR;
    }
}

int decode_exp(lac_queue_t *pQueSymbols, const char *sExpression, long lNumBytes) {
    /* Decode expression */
    /* Input: mid-fix expression char; Output: queue of symbols in same order, but marked the information*/

    enum calculator_stat CalcStat = CALC_STAT_BEGIN;
    exp_btnode_t NodeTmp = {NULL, NULL, CALC_NAN, {0}, {0}}; /* Placeholder of Node */
    exp_btnode_t NodeZero = {NULL, NULL, CALC_NUMBER, {0}, {0}};
    char cTmp;
    char cNumStrTmp[MAX_NUMBER_LEN]; // Placeholder of number string
    int iNumValTmp; // Placeholder of value
    int iNumberStart;
    int iNumberEnd;
    int idx = 0;
    while (idx < lNumBytes) {
        cTmp = sExpression[idx];
        if (cTmp >= '0' && cTmp <= '9') {
            /* The number is encourtered */

            if (CalcStat == CALC_STAT_OP_PAR_RIGHT) {
                printf("\n[ Warning ] Syntax Error, <)>{<number>} not allowed\n");
                return CALC_STAT_ERR;
            }
            memset(cNumStrTmp, 0, MAX_NUMBER_LEN);
            iNumberStart = idx;

            /* Get the entire number */
            while (sExpression[idx] >= '0' && sExpression[idx] <= '9') { // Can only handle integers.
                ++idx;
            }

            iNumberEnd = idx;
            strncpy(cNumStrTmp, sExpression + iNumberStart, (size_t) iNumberEnd - iNumberStart);

            /* Translate the value to int using strtol */
            iNumValTmp = (int) strtol(cNumStrTmp, NULL, 10); // Convertion from long to int
            NodeTmp.type = CALC_NUMBER;
            NodeTmp.u_Data.iNumber = iNumValTmp; // Used for Tree presentation
            NodeTmp.u_Value.iNumber = iNumValTmp; // Used for calculation
            NodeTmp.lNode = NodeTmp.rNode = NULL;

            queue_push_back(pQueSymbols, (void *) &NodeTmp, sizeof(exp_btnode_t));
            CalcStat = CALC_STAT_NUMBER;
            continue; // We modified the idx, no need to increase idx in the while loop
        } else if (cTmp == '(') {
            /* A ( is encountered */

            /* We forbid the {<number>|<)>}{<(>} combination */
            if (CalcStat == CALC_STAT_NUMBER || CalcStat == CALC_STAT_OP_PAR_RIGHT) {
                printf("\n[ Warning ] Syntax Error, {<number>|<)>}{<(>} not allowed\n");
                return CALC_STAT_ERR;
            }

            NodeTmp.type = CALC_OP_PAR_LEFT;
            NodeTmp.u_Data.cOP = cTmp;
            NodeTmp.lNode = NodeTmp.rNode = NULL;

            queue_push_back(pQueSymbols, (void *) &NodeTmp, sizeof(exp_btnode_t));
            CalcStat = CALC_STAT_OP_PAR_LEFT;
        } else if (cTmp == ')') {
            /* A ( is encountered */

            /* We forbid the <op>{<)>} combination */
            if (CalcStat == CALC_STAT_OP) {
                printf("\n[ Warning ] Syntax Error, <op>{<)>} not allowed\n");
                return CALC_STAT_ERR;
            }

            NodeTmp.type = CALC_OP_PAR_RIGHT;
            NodeTmp.u_Data.cOP = cTmp;
            NodeTmp.lNode = NodeTmp.rNode = NULL;

            queue_push_back(pQueSymbols, (void *) &NodeTmp, sizeof(exp_btnode_t));
            CalcStat = CALC_STAT_OP_PAR_RIGHT;
        } else if (cTmp == '+' || cTmp == '-' || cTmp == 'x' || cTmp == '/') {
            /* Signed number handle: if the previous symble is '(' or operator, and a pseudo zero in the front */
            if ((CalcStat == CALC_STAT_BEGIN || CalcStat == CALC_STAT_OP || CalcStat == CALC_STAT_OP_PAR_LEFT) &&
                (cTmp == '+' || cTmp == '-')) {
                queue_push_back(pQueSymbols, (void *) &NodeZero, sizeof(exp_btnode_t)); // a signed number is encourtered, add zero
                /* The node has a higher priority */
                NodeTmp.type = CALC_OP_3;
            } else if ((CalcStat == CALC_STAT_OP || CalcStat == CALC_STAT_OP_PAR_LEFT) && (cTmp == '/' || cTmp == 'x')) {
                /* We forbid the {<op>|<(>}{<x>|</>} combination */
                printf("\n[ Warning ] Syntax Error, {<op>|<(>}{<x>|</>} not allowed\n");
                return CALC_STAT_ERR;
            } else {
                /* Normal case */
                if (cTmp == '+' || cTmp == '-') {
                    NodeTmp.type = CALC_OP_1; // 1st level operator
                } else {
                    NodeTmp.type = CALC_OP_2; // 2nd level operator
                }

            }
            /* end of signed number handle */
            NodeTmp.u_Data.cOP = cTmp;
            NodeTmp.lNode = NodeTmp.rNode = NULL;

            queue_push_back(pQueSymbols, (void *) &NodeTmp, sizeof(exp_btnode_t));
            CalcStat = CALC_STAT_OP;
        } else if (cTmp == ' ') {
            ++idx;
            continue;
        } else {
            if (cTmp == '\n') {
                break;
            }
            CalcStat = CALC_STAT_ERR;
            printf("\n[ Warning ] Syntax Error, illegal character: %c\n", cTmp);
            return CalcStat;
        }
        ++idx;
    }
    CalcStat = CALC_STAT_FIN;
    return CalcStat;
}

int gen_postfix_exp(lac_queue_t *pQuePostfixExp, lac_queue_t *pQueSymbols) {
    /* Generate post-fix expression from a mid-fix expression (symbols) */
    /* Use stack to achieve the goal */
    /* The symbol queue will be cleared */

    enum calculator_stat CalcStat;
    lac_stack_t stkOp;
    stack_init(&stkOp);
    exp_btnode_t SymbolTmp;
    exp_btnode_t OpTmp;
    while (!queue_is_empty(pQueSymbols)) {
        queue_pop_front(pQueSymbols, (void *) &SymbolTmp, TRUE); // scan the queue, pick a symbol,

        if (SymbolTmp.type == CALC_NUMBER) {
            /* If it is a number, push it in the postfix expression */
            queue_push_back(pQuePostfixExp, (void *) &SymbolTmp, sizeof(exp_btnode_t)); // If it is a number, enqueue
        } else if (SymbolTmp.type == CALC_OP_PAR_LEFT) {
            /* If it is a '(', push it in to the operator stack */
            stack_push(&stkOp, (void *) &SymbolTmp, sizeof(exp_btnode_t));
        } else if (SymbolTmp.type == CALC_OP_PAR_RIGHT) {
            /* If it is a ')', pop all the operators in the operator stack, until '(' */
            while (!stack_is_empty(&stkOp)) {
                stack_pop(&stkOp, (void *) &OpTmp);
                if (OpTmp.type == CALC_OP_PAR_LEFT) {
                    /* Drop the '(' */
                    break;
                } else {
                    queue_push_back(pQuePostfixExp, (void *) &OpTmp, sizeof(exp_btnode_t));
                }
                if (stack_is_empty(&stkOp)) {
                    /* parenthese not match, this is an error */
                    printf("\n[ Warning ] Unclosed parentheses )\n");
                    return CALC_STAT_ERR;
                }
            }
        } else if (SymbolTmp.type == CALC_OP_1 || SymbolTmp.type == CALC_OP_2 || SymbolTmp.type == CALC_OP_3) {
            /* If it is a operator */

            if (stack_is_empty(&stkOp)) {
                /* If the operator stack is empty, push it in */
                stack_push(&stkOp, (void *) &SymbolTmp, sizeof(exp_btnode_t));
            } else {
                /* If not, pop all the operators higher or equal to it, push it in the stack */
                while (!stack_is_empty(&stkOp)) {
                    stack_pop(&stkOp, (void *) &OpTmp);
                    if (OpTmp.type >= SymbolTmp.type) {
                        queue_push_back(pQuePostfixExp, (void *) &OpTmp, sizeof(exp_btnode_t));
                    } else {
                        stack_push(&stkOp, (void *) &OpTmp, sizeof(exp_btnode_t));
                        break;
                    }
                }
                stack_push(&stkOp, (void *) &SymbolTmp, sizeof(exp_btnode_t));
            }
        }

    }
    /* Pop whatever left in the operator stack */
    while (!stack_is_empty(&stkOp)) {
        stack_pop(&stkOp, (void *) &OpTmp);
        /* If there is ( left , then we know there is a parentheses mismatch */
        if (OpTmp.type == CALC_OP_PAR_LEFT) {
            printf("\n[ Warning ] Unclosed parenthese (\n");
            return CALC_STAT_ERR;
        }
        queue_push_back(pQuePostfixExp, (void *) &OpTmp, sizeof(exp_btnode_t));
    }

    stack_clear(&stkOp); /* Free up memory */
    CalcStat = CALC_STAT_FIN;
    return CalcStat;
}

int build_exp_tree(lac_queue_t *pQuePostfixExp, exp_btnode_t *pExpTreeRoot) {
    /* The postfix expression queue will not be modified */

    enum calculator_stat CalcStat = CALC_STAT_BEGIN;
    lac_stack_t stkExpTree;
    stack_init(&stkExpTree);

    exp_btnode_t *pPostfixSymbolTmp;
    /* Two pointers helps to construct the tree structure */
    exp_btnode_t *pExpTreeNodeTmpLeft;
    exp_btnode_t *pExpTreeNodeTmpRight;
    exp_btnode_t ExpTreeNodeTmpRoot; // We reuse this piece of memory
    const exp_btnode_t *pExpTreeNodeTmpRoot = &ExpTreeNodeTmpRoot;

    bool bFlag = FALSE; // used to determine the end of queue

    /* Use a while loop to read the queue */
    queue_node_t *pPostfixNodeTmp = pQuePostfixExp->pFront;
    while (TRUE) {
        pPostfixSymbolTmp = (exp_btnode_t *) pPostfixNodeTmp->pData; // The pointer points to a place in the queue (Data)
        switch (pPostfixSymbolTmp->type) {
            case CALC_NUMBER: {
                stack_push(&stkExpTree, (void *) pPostfixSymbolTmp, sizeof(struct exp_btnode_t));
                break;
            }
            case CALC_OP_1:
            case CALC_OP_2:
            case CALC_OP_3: {
                pExpTreeNodeTmpLeft = gen_exp_BTNode(); // allocate a piece of memory
                pExpTreeNodeTmpRight = gen_exp_BTNode(); // allocate a piece of memory

                stack_pop(&stkExpTree, (void *) pExpTreeNodeTmpRight);
                stack_pop(&stkExpTree, (void *) pExpTreeNodeTmpLeft);
                ExpTreeNodeTmpRoot = *pPostfixSymbolTmp;

                /* Link Nodes */
                ExpTreeNodeTmpRoot.lNode = pExpTreeNodeTmpLeft;
                ExpTreeNodeTmpRoot.rNode = pExpTreeNodeTmpRight;

                /* Calculation comes with linking */
                int iRet = calculate_2_op_int(pExpTreeNodeTmpLeft->u_Value.iNumber,
                                              pExpTreeNodeTmpRight->u_Value.iNumber,
                                              &ExpTreeNodeTmpRoot.u_Value.iNumber,
                                              ExpTreeNodeTmpRoot.u_Data.cOP);
                if (iRet == CALC_STAT_ERR) {
                    CalcStat = CALC_STAT_ERR;
                    bFlag = TRUE;
                    break;
                }
                /* Push the root back */
                stack_push(&stkExpTree, (void *) pExpTreeNodeTmpRoot, sizeof(exp_btnode_t));
                break;
            }
            default: {
                CalcStat = CALC_STAT_ERR;
                bFlag = TRUE;
                break;
            }
        }
        pPostfixNodeTmp = pPostfixNodeTmp->pNext;
        if (bFlag == TRUE) break;
        if (pPostfixNodeTmp == pPostfixNodeTmp->pNext) {
            bFlag = TRUE;
        }
    }

    /* Finally we get the result and free up memory */
    stack_pop(&stkExpTree, (void *) pExpTreeRoot);
    stack_clear(&stkExpTree);

    if (CalcStat != CALC_STAT_ERR) {
        CalcStat = CALC_STAT_FIN;
    }

    return CalcStat;

}

int calculate_exp_int(const char *sExpression, int iNumBytes, int *res) {
    /* This function calculate the value of a expression (sExpresion -> sExpression + iNumBytes) */
    /* The output is writen to res, disp controls if the process is visualized */

    int ans;
    int iRet;
#ifdef DEBUG
    printf("\nCalculate expression: %.*s\n", iNumBytes, sExpression);
#endif

    lac_queue_t queSymbols;
    queue_init(&queSymbols);
    lac_queue_t quePostfixExp;
    queue_init(&quePostfixExp);
    exp_btnode_t ExpTreeRoot;

    /* Decode expression to symbols */
    iRet = decode_exp(&queSymbols, sExpression, iNumBytes);
    if (iRet != CALC_STAT_FIN) {
        queue_clear(&queSymbols);
        queue_clear(&quePostfixExp);
        btree_destroy(&ExpTreeRoot);
        return CALC_STAT_ERR;
    }

    /* Generate post-fix expression */

    iRet = gen_postfix_exp(&quePostfixExp, &queSymbols);
    if (iRet != CALC_STAT_FIN) {
        queue_clear(&queSymbols);
        queue_clear(&quePostfixExp);
        btree_destroy(&ExpTreeRoot);
        return CALC_STAT_ERR;
    }

    /* Build expression tree */

    iRet = build_exp_tree(&quePostfixExp, &ExpTreeRoot);
    if (iRet != CALC_STAT_FIN) {
        queue_clear(&queSymbols);
        queue_clear(&quePostfixExp);
        btree_destroy(&ExpTreeRoot);
        return CALC_STAT_ERR;
    }

    ans = ExpTreeRoot.u_Value.iNumber;

#ifdef DEBUG
    disp_postfix_exp(&quePostfixExp);
    fflush(stdout);
    /* Visualisation of the tree */
    disp_exp_tree(ExpTreeRoot);
    /* The value of expression is stored at the root node */
    printf("\n[ info ] Expresion value: %d \n", ans);
    fflush(stdout);
#endif

    /* Free up the memory to avoid memory leakage */
    queue_clear(&queSymbols);
    queue_clear(&quePostfixExp);
    btree_destroy(&ExpTreeRoot);

    *res = ans;
    return CALC_STAT_FIN;
}
