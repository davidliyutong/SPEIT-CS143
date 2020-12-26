//
// Created by 厉宇桐 on 2020/12/23.
//

#include "function.h"

void compile_function(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This fucnction compiles lac functions */

    /* The : is already popped, hear comes the function */
    lexeme_t LexTmp;
    int iStrIdx; // tmp value
    int iCFATmp; //tmp value
    int iCFACurr; // for the key word 'recursive' only
    int iImmVal; // Immediate value
    int iJumpStart; // tmp value, if jump start from stack
    int iBreakStart;// tmp value, while break start from stack
    int iWhileStart;// tmp value, while loop start from stack
    int iRet;
    hash_table_query_res SymbolQueryRes;


    lac_stack_t StkIf; // counter that records the number of if's, use for detecting syntax error
    lac_stack_t StkElse;  // counter that records the number of else's, use for detecting syntax error
    lac_stack_t StkWhile; // counter that records the number of while, and the loop destination
    lac_stack_t StkBreak; // counter that records the number of break, and the break destination

    stack_init(&StkIf);
    stack_init(&StkElse);
    stack_init(&StkWhile);
    stack_init(&StkBreak);
    int iIfLastIdx;   // last if's idx
    int iElseLastIdx; // last if's idx

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);

    if (LexTmp.type != WORD) {
        printf("\n[ Warning ] Function not properly named, invalid name\n");
        g_proc_env_reset();
        return;
    }
    char *pSymbolStr = psReadBuffer + LexTmp.iStart;
    int iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

    vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM, OP_CODE_INST);
    iRet = hash_symtable_add(&g_Env.SymTable, pSymbolStr, iSymbolLength, g_Env.VMTable.iTail);
    iCFACurr = g_Env.VMTable.iTail;
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        g_proc_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] Define function %.*s with CFA = %d\n", LexTmp.iEnd - LexTmp.iStart, psReadBuffer + LexTmp.iStart, iCFACurr);
        fflush(stdout);
#endif
    } else {
        printf("\n[ Info ] Semantic risk, Duplicate definition, previous definition will be replaced\n");
    }

    /* The : and the name is popped */
    while (TRUE) {
        if (queue_is_empty(pqueRes) == TRUE) {
            printf("\n[ Waring ] Syntax error, function is not defined properly\n");
            g_proc_env_reset();
            return;
        }
        queue_pop_front(pqueRes, (void *) &LexTmp);
        pSymbolStr = psReadBuffer + LexTmp.iStart;
        iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

        /* In function variables, note they are in fact global variables*/
        if (strncmp(psReadBuffer + LexTmp.iStart, "variable", 8) == 0 &&
            (LexTmp.iEnd - LexTmp.iStart) == 8) {
            /* The definition of variable will interrupt the function, use 'else' to skip it */
            vmtable_add(&g_Env.VMTable, VM_CFA_JR, OP_CODE_INST);
            vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 6, OP_CODE_DATA);
            declare_var(psReadBuffer, pqueRes);
            continue;
        }

        /* In function vectors, note they are in fact global vectors*/
        if (strncmp(psReadBuffer + LexTmp.iStart, "vec", 3) == 0 &&
            (LexTmp.iEnd - LexTmp.iStart) == 3) {

            /* The definition of variable will interrupt the function, use 'else' as unconditional jump to skip it */
            vmtable_add(&g_Env.VMTable, VM_CFA_JR, OP_CODE_INST);

            int iVecStart = g_Env.VMTable.iTail + 1; // start of 'vec' jump
            vmtable_add(&g_Env.VMTable, 0, OP_CODE_DATA); // pseudo value

            declare_vec(psReadBuffer, pqueRes); // declare the vec
            g_Env.VMTable.OpCodes[iVecStart] = g_Env.VMTable.iTail; // set the pre-defined pseudo value to real jump destination
            continue;
        }

        if (strncmp(psReadBuffer + LexTmp.iStart, ";", 1) == 0) {
            /* The end of definition */
            break;
        }
        if (strncmp(psReadBuffer + LexTmp.iStart, ":", 1) == 0) {
            /* begin of another function？ No!*/
            printf("\n[ Waring ] Syntax error, function is not defined properly, missing ;\n");
            g_proc_env_reset();
            return;
        }

        /* Not the end of definition */

        if (LexTmp.type == WORD) {
            /* A word is encountered */
            SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, pSymbolStr, iSymbolLength);
            if (SymbolQueryRes.idx < 0) {
                printf("\n[ Warning ] Semantic error, Undefined symbol: %.*s\n", LexTmp.iEnd - LexTmp.iStart,
                       psReadBuffer + LexTmp.iStart);
                g_proc_env_reset();
                return;
            } else {
                iCFATmp = hash_symtable_get_cfa(&g_Env.SymTable, pSymbolStr, iSymbolLength);
                /* if -> 26 else -> 28 then -> 30 */
                switch (iCFATmp) {
                    case VM_CFA_WHILE: {
                        stack_push_vm(&StkWhile, g_Env.VMTable.iTail + 1);
                        vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        stack_push_vm(&StkBreak, -1); // Add an invalid CFA to break stack, mark the begin of loop
                        break;
                    }
                    case VM_CFA_BREAK: {
                        stack_push_vm(&StkBreak, g_Env.VMTable.iTail + 2);
                        vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        vmtable_add(&g_Env.VMTable, -1, OP_CODE_INST); // loop end destination, now its an invalid value.
                        break;
                    }
                    case VM_CFA_LOOP: {
                        while (stack_is_empty(&StkBreak) != TRUE) {
                            /* A while loop may have more thant one breaks */
                            iBreakStart = stack_pop_vm(&StkBreak);
                            if (iBreakStart >= 0) {
                                g_Env.VMTable.OpCodes[iBreakStart] = g_Env.VMTable.iTail + 2;
                            } else {
                                break;
                            }
                        }
                        if (stack_is_empty(&StkWhile) == TRUE) {
                            printf("\n[ Warning ] Syntax error, mismatched 'while' and 'loop'\n");
                            g_proc_env_reset();
                            return;
                        } else {
                            iWhileStart = stack_pop_vm(&StkWhile);
                        }
                        vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        vmtable_add(&g_Env.VMTable, iWhileStart, OP_CODE_DATA);
                        break;
                    }
                    case VM_CFA_IF: {
                        stack_push_vm(&StkIf, g_Env.VMTable.iTail +
                                              2); // where if's jump position is stored, need to be filled after else/then is encountered
                        vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        vmtable_add(&g_Env.VMTable, -1, OP_CODE_DATA); // jump destination, now its an invalid value.
                        break;
                    }
                    case VM_CFA_ELSE: {
                        iJumpStart = stack_pop_vm(&StkIf); // if's jump position
                        g_Env.VMTable.OpCodes[iJumpStart] =
                                g_Env.VMTable.iTail + 2; // set the if's jump destination to this else

                        stack_push_vm(&StkElse, g_Env.VMTable.iTail +
                                                2); // where else's jump position is stored, need to be filled after else/then is encountered
                        vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        vmtable_add(&g_Env.VMTable, -1, OP_CODE_DATA); // jump destination, now its an invalid value.
                        break;
                    }
                    case VM_CFA_THEN: {
                        if (stack_is_empty(&StkIf)) iIfLastIdx = -1;
                        else iIfLastIdx = stack_top_vm(&StkIf);

                        if (stack_is_empty(&StkElse)) iElseLastIdx = -1;
                        else iElseLastIdx = stack_top_vm(&StkElse);

                        if (iElseLastIdx == -1 && iIfLastIdx == -1) {
                            printf("\n[ Warning ] Syntax error, mismatched 'If', 'else' and 'then'\n");
                            g_proc_env_reset();
                            return;
                        }

                        if (iIfLastIdx < iElseLastIdx) {
                            /* The closest symbol is else */
                            iJumpStart = stack_pop_vm(&StkElse);
                            g_Env.VMTable.OpCodes[iJumpStart] = g_Env.VMTable.iTail; // set the else's jump destination
                        } else {
                            /* The closest symbol is if */
                            iJumpStart = stack_pop_vm(&StkIf);
                            g_Env.VMTable.OpCodes[iJumpStart] = g_Env.VMTable.iTail; // set the if's jump destination
                        }
                        break;
                    }

                    default:
                        if (iCFATmp != VM_CFA_RECURSE) {
                            vmtable_add(&g_Env.VMTable, iCFATmp, OP_CODE_INST);
                        } else {
                            /* Recursive function: Execute the current function */
                            vmtable_add(&g_Env.VMTable, iCFACurr, OP_CODE_INST);
                        }


                }
            }
        } else if (LexTmp.type == NUMBER) {
            /* Add a lit. [!] this can be done by directly add the opcode lit */
            vmtable_add(&g_Env.VMTable, VM_CFA_LIT, OP_CODE_INST); // 20 is the CFA of (lit) [!]
            iImmVal = (int) strtol(psReadBuffer + LexTmp.iStart, NULL, 10);
            vmtable_add(&g_Env.VMTable, iImmVal, OP_CODE_INST);
        } else if (LexTmp.type == STRING) {
            /* If a string is encountered */
            /* We store the string on the VMTable, so it must be skipped during execution */
            /* 'Else' function is the unconditional jump function, it is used to skip the string*/

            /* First add a unconditional jump to skip */
            int iStrLen = LexTmp.iEnd - LexTmp.iStart;
            vmtable_add(&g_Env.VMTable, VM_CFA_JR, OP_CODE_INST);
            vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + iStrLen + 2, OP_CODE_DATA);

            /* Then store the string to VMTable */
            iStrIdx = g_Env.VMTable.iTail + 1; // Get the string's idx
            vmtable_add_vec(&g_Env.VMTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart, sizeof(char));

            /* The string is saved to iStrIdx, we save this value to vmtable*/
            /* Do the same as the situation of numbers (lit) + adddr */
            vmtable_add(&g_Env.VMTable, VM_CFA_LIT, OP_CODE_INST);
            vmtable_add(&g_Env.VMTable, iStrIdx, OP_CODE_INST);
        }

    }

    /* If-else syntax check */
    if (stack_is_empty(&StkIf) == FALSE || stack_is_empty(&StkElse) == FALSE) {
        printf("\n[ Warning ] Syntax error, mismatched 'If', 'else' and 'then'\n");
        g_proc_env_reset();
    }

    /* Add a fin. [!] this can be done by directly add the opcode fin */
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN, OP_CODE_INST); // 11 est le CFA de (fin)
    stack_clear(&StkIf);
    stack_clear(&StkElse);
    stack_clear(&StkWhile);
    stack_clear(&StkBreak);
    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}

void declare_function(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function declares a lac functions */

    /* The defer is already popped, hear comes the function(declare) */
    lexeme_t LexTmp;
    int iRet;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);

    if (LexTmp.type != WORD) {
        printf("\n[ Warning ] Function not properly named, invalid name\n");
        g_proc_env_reset();
        return;
    }

    char *pSymbolStr = psReadBuffer + LexTmp.iStart;
    int iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

    vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM, OP_CODE_INST);
    iRet = hash_symtable_add(&g_Env.SymTable, pSymbolStr, iSymbolLength, g_Env.VMTable.iTail);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        g_proc_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] Declare function %.*s with CFA = %d\n", iSymbolLength,
               pSymbolStr, g_Env.VMTable.iTail);
        fflush(stdout);
#endif
    } else {
        printf("\n[ Warning ] Semantic error, Duplicate declaration of: %.*s\n", iSymbolLength,
               pSymbolStr);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    }
    /* Add an pseudo(wrong) function and a (fin) */
    vmtable_add(&g_Env.VMTable, VM_CFA_ERROR, OP_CODE_INST);
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN, OP_CODE_INST);
    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}

void link_declaration(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function links function to declaration: Src -> Dst declares a lac functions */

    /* The ' is already popped, hear comes the function */
    /* ' Dst is Src */
    lexeme_t LexDst;
    lexeme_t LexSrc;
    lexeme_t LexIs;

    int iDstCFA;
    int iSrcCFA;
    hash_table_query_res DstSymbolQueryRes;
    hash_table_query_res SrcSymbolQueryRes;

    /* Get the name of declaration and function */
    queue_pop_front(pqueRes, (void *) &LexDst);
    queue_pop_front(pqueRes, (void *) &LexIs);
    queue_pop_front(pqueRes, (void *) &LexSrc);

    /* Syntax check */
    int iLexIsLength = LexIs.iEnd - LexIs.iStart;
    char *pLexIsStr = psReadBuffer + LexIs.iStart;

    if (!(strncmp(pLexIsStr, "is", 2) == 0 && iLexIsLength == 2)) {
        printf("\n[ Warning ] Syntax error, expect 'is' after ' at %d\n", LexIs.iStart);
        g_proc_env_reset();
        return;
    }

    /* Get CFA of Dst (the well-defined function) */
    char *pLexDstStr = psReadBuffer + LexDst.iStart;
    int iLexDstLength = LexDst.iEnd - LexDst.iStart;
    DstSymbolQueryRes = hash_symtable_search(&g_Env.SymTable, pLexDstStr, iLexDstLength);

    if (DstSymbolQueryRes.idx < 0) {
        printf("\n[ Warning ] Semantic error, link Dst (%.*s) is invalid\n", iLexDstLength,
               pLexDstStr);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    } else {
        iDstCFA = hash_symtable_get_cfa(&g_Env.SymTable, pLexDstStr, iLexDstLength);
    }

    /* Get CFA of Src (the function declaration) */
    char *pLexSrcStr = psReadBuffer + LexSrc.iStart;
    int iLexSrcLength = LexSrc.iEnd - LexSrc.iStart;
    SrcSymbolQueryRes = hash_symtable_search(&g_Env.SymTable, pLexSrcStr, iLexSrcLength);


    if (SrcSymbolQueryRes.idx < 0) {
        printf("\n[ Warning ] Semantic error, link Src (%.*s) is invalid\n", iLexSrcLength,
               pLexSrcStr);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    } else {
        iSrcCFA = hash_symtable_get_cfa(&g_Env.SymTable, pLexSrcStr, iLexSrcLength);
    }

    /* Link */
    g_Env.VMTable.OpCodes[iSrcCFA + 1] = iDstCFA;
#ifdef DEBUG
    printf("\n[ Debug ] Link function %.*s to %.*s\n", iLexSrcLength, pLexSrcStr,
           iLexDstLength, pLexDstStr);
    fflush(stdout);
#endif
    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}

void declare_var(char *psReadBuffer, lac_queue_t *pqueRes) {
    lexeme_t LexTmp;
    int iRet;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);

    if (LexTmp.type != WORD) {
        printf("\n[ Warning ] Variable not properly named, invalid name\n");
        g_proc_env_reset();
        return;
    }

    char *pSymboleStr = psReadBuffer + LexTmp.iStart;
    int iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

    vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM, OP_CODE_INST);
    iRet = hash_symtable_add(&g_Env.SymTable, pSymboleStr, iSymbolLength, g_Env.VMTable.iTail);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        g_proc_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] A variable %.*s is defined at VMTable[%d]\n", iSymbolLength,
               pSymboleStr, g_Env.VMTable.iTail);
#endif
    } else {
        printf("\n[ Info ] Semantic risk of (var), Duplicate definition, previous definition will be replaced\n");
    }

    vmtable_add(&g_Env.VMTable, VM_CFA_LIT, OP_CODE_INST);
    vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 3, OP_CODE_DATA); // address of pseudo value
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN, OP_CODE_INST);
    vmtable_add(&g_Env.VMTable, 0, OP_CODE_DATA); // pseudo value
    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}

void declare_vec(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* Declare a vector */

    lexeme_t LexTmp;
    int iRet;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);

    if (LexTmp.type != WORD) {
        printf("\n[ Warning ] Vec not properly named, invalid name\n");
        g_proc_env_reset();
        return;
    }

    char *pSymbolStr = psReadBuffer + LexTmp.iStart;
    int iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

    vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM, OP_CODE_INST);
    iRet = hash_symtable_add(&g_Env.SymTable, pSymbolStr, iSymbolLength, g_Env.VMTable.iTail);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        g_proc_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] A vector %.*s is defined at VMTable[%d]\n", iSymbolLength,
               pSymbolStr, g_Env.VMTable.iTail);
#endif
    } else {
        printf("\n[ Info ] Semantic risk of (vec), Duplicate definition, previous definition will be replaced\n");
    }

    /* Get the length of vector */
    queue_pop_front(pqueRes, (void *) &LexTmp);
    pSymbolStr = psReadBuffer + LexTmp.iStart;
    iSymbolLength = LexTmp.iEnd - LexTmp.iStart;

    int iVecLen = (int) strtol(pSymbolStr, NULL, 10);
    if (iVecLen <= 0) {
        /* There is a syntax error */
        printf("\n[ Warning ] Vec not properly defined, invalid length\n");
        g_proc_env_reset();
        return;
    }
#ifdef DEBUG
    printf("\n[ Debug ] A vector %.*s of length %d is defined at VMTable[%d]\n", iSymbolLength,
           pSymbolStr, iVecLen, g_Env.VMTable.iTail);
#endif

    /* First add the unconditional jump to skip the data area */
    vmtable_add(&g_Env.VMTable, VM_CFA_JR, OP_CODE_INST);
    vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 1 + iVecLen, OP_CODE_DATA);

    int iVecIdx = g_Env.VMTable.iTail + 1; // address for the beginning of vector
    vmtable_add_vec(&g_Env.VMTable, NULL, iVecLen, sizeof(int)); // add iVecLen 0s to the VMTable
    vmtable_add(&g_Env.VMTable, VM_CFA_LIT, OP_CODE_INST);
    vmtable_add(&g_Env.VMTable, iVecIdx, OP_CODE_DATA); // address of pseudo value
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN, OP_CODE_INST);

    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}