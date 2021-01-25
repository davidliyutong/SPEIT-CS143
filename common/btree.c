//
// Created by 厉宇桐 on 2020/11/21.
//
// Data structure: btree
/**@file  btree.c
* @brief    Functions related to binary tree 
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/

#include "btree.h"

/**@brief The function destroys a binary tree.
 * @param pTreeNode The root of the tree that needs destruction.
 * @note Every tree node must be dynamically allocated using malloc().
 */
void btree_destroy(exp_btnode_t *pTreeNode) {
    if (pTreeNode == NULL) {
        return;
    } else {
        ///@remark Recursive destruction
        btree_destroy(pTreeNode->rNode); 
        btree_destroy(pTreeNode->lNode);
        free(pTreeNode->rNode);
        free(pTreeNode->lNode);
        return;
    }
}
