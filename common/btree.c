//
// Created by 厉宇桐 on 2020/11/21.
//
// Data structure: btree

#include "btree.h"

void btree_destroy(exp_btnode_t *pTreeNode) {
    /*
     * Destroy a binary tree from its root node
     * Input is the root node
     */

    if (pTreeNode == NULL) {
        return;
    } else {
        btree_destroy(pTreeNode->rNode);
        btree_destroy(pTreeNode->lNode);
        free(pTreeNode->rNode);
        free(pTreeNode->lNode);
        return;
    }
}
