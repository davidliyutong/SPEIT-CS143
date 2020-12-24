//
// Created by 厉宇桐 on 2020/11/19.
//

#include "vmtable.h"

int vmtable_init(vmtable_t *pVMTable) {
    /* Init a vmtable defined outside of function */

    pVMTable->iMaxLength = INIT_VM_TABLE_LEN;
    pVMTable->iTail = -1;
    pVMTable->OpCodes = malloc(sizeof(int) * pVMTable->iMaxLength);
    memset(pVMTable->OpCodes, VM_CTRL_FUNC_SYM, sizeof(int) * pVMTable->iMaxLength);

    if (pVMTable->OpCodes == NULL) {
        printf("\n[ Error ] Out of memory\n");
        exit(ERR_MEM);
    }

    return TRUE;
}

int vmtable_expand(vmtable_t *pVMTable) {
    /* If the pVMTable is not enough, then it need to be expanded */

    pVMTable->OpCodes = (int *) realloc(pVMTable->OpCodes, sizeof(int) * (pVMTable->iMaxLength + INIT_VM_TABLE_LEN));
    if (pVMTable->OpCodes == NULL) {
        printf("\n[ Error ] Out of memory\n");
        exit(ERR_MEM);
    }
    pVMTable->iMaxLength += INIT_VM_TABLE_LEN;
    return TRUE;
}

inline int vmtable_add(vmtable_t *pVMTable, int iOpCode) {
    /* Add a OpCode to the vmtable */

    if (pVMTable->iTail >= pVMTable->iMaxLength - 1) {
        vmtable_expand(pVMTable);
    }
    pVMTable->OpCodes[++pVMTable->iTail] = iOpCode;
    return TRUE;
};

inline int vmtable_del(vmtable_t *pVMTable) {
    /* Delete a OpCode from the vmtable */
    pVMTable->OpCodes[pVMTable->iTail--] = VM_CTRL_FUNC_SYM;
    return TRUE;
};

inline int vmtable_checkout(vmtable_t *pVMTable) {
    /* Checkout the modification*/
    pVMTable->iCheckPoint = pVMTable->iTail;
    return TRUE;
};

inline int vmtable_revert(vmtable_t *pVMTable) {
    /* Revert the modification*/
    while (pVMTable->iTail > pVMTable->iCheckPoint) {
        vmtable_del(pVMTable);
    }
    pVMTable->iCheckPoint = pVMTable->iTail;
    return TRUE;
};

inline int vmtable_add_vec(vmtable_t *pVMTable, void *pVec, int iLen, int iSize) {
    /* Add a vector or char string to the VMtable, if it is a string, an 0 will be append to the end */

    /* The input is a empty vector, we only fill it with 0*/
    if (pVec == NULL) {
        for (int idx = 0; idx < iLen; idx++) {
            /* Char stored as int */
            vmtable_add(pVMTable, 0);
        }
    } else {
        /* The input is not a empty vector */
        switch (iSize) {
            case 1:
                for (int idx = 0; idx < iLen; idx++) {
                    /* Char stored as int */
                    vmtable_add(pVMTable, (int) ((char *) pVec)[idx]);
                }
                /* Add zero */
                vmtable_add(pVMTable, 0);
                break;
            case 4:
                for (int idx = 0; idx < iLen; idx++) {
                    vmtable_add(pVMTable, ((int *) pVec)[idx]);
                }
                break;
            default:
                printf("\n[ Error ] The Size of each element must be 1(char) or 4(int) Byte\n");
                exit(ERR_MEM);
        }
    }

    return TRUE;
}

inline int vmtable_strlen(vmtable_t *pVMTable, int iAddr) {
    int iLength = 0;
    while (pVMTable->OpCodes[iAddr++] != 0) {
        iLength++;
    }
    return iLength;
}

int vmtable_clear(vmtable_t *pVMTable) {
    /* Clear the VM table */

    pVMTable->iMaxLength = INIT_VM_TABLE_LEN;
    pVMTable->iTail = -1;
    free(pVMTable->OpCodes);
    pVMTable->OpCodes = NULL;
    pVMTable->OpCodes = malloc(sizeof(int) * pVMTable->iMaxLength);
    memset(pVMTable->OpCodes, -3, sizeof(int) * pVMTable->iMaxLength);

    if (pVMTable->OpCodes == NULL) {
        printf("\n[ Error ] Out of memory\n");
        exit(ERR_MEM);
    }

    return TRUE;
}
