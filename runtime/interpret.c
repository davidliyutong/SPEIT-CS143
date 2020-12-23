//
// Created by 厉宇桐 on 2020/12/2.
//
// Function used to execute LAC

# include "interpret.h"

vmenv_t g_Env = {0};

void (*g_proc_basic_func[INIT_PROC_FUNC_NUM])() = {proc_func_dup,
                                                   proc_func_drop,
                                                   proc_func_swap,
                                                   proc_func_dot,
                                                   proc_func_count,
                                                   proc_func_type,
                                                   proc_func_equal,
                                                   proc_func_mult,
                                                   proc_func_plus,
                                                   proc_func_minus,
                                                   proc_func_lit,
                                                   proc_func_fin,
                                                   proc_func_calculate,
                                                   proc_func_if,
                                                   proc_func_else,
                                                   proc_func_then,
                                                   proc_func_recurse,
                                                   proc_func_at,
                                                   proc_func_exclaim,
                                                   proc_func_while,
                                                   proc_func_loop,
                                                   proc_func_break};

char g_proc_basic_func_name[INIT_PROC_FUNC_NUM][10] = {"dup",
                                                       "drop",
                                                       "swap",
                                                       ".",
                                                       "count",
                                                       "type",
                                                       "=",
                                                       "*",
                                                       "+",
                                                       "-",
                                                       "(lit)",
                                                       "(fin)",
                                                       "calculate",
                                                       "if",
                                                       "else",
                                                       "then",
                                                       "recurse",
                                                       "@",
                                                       "!",
                                                       "while",
                                                       "loop",
                                                       "break"};

int g_proc_basic_func_type[INIT_PROC_FUNC_NUM] = {VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM, // @
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
};

inline int stack_push_vm(lac_stack_t *pStack, int iData) {
    /* special push method dedicated for VM(int) */
#ifdef DEBUG
    if (pStack == &g_Env.StkData) {
        printf("\n[ Debug ] Push %d into StkData\n", iData);
    } else if (pStack == &g_Env.StkReturn) {
        printf("\n[ Debug ] Push %d into StkReturn\n", iData);
    } else { ;
    }
    fflush(stdout);

#endif
    int iRet;
    iRet = stack_push(pStack, (void *) &iData, sizeof(int));
    if (iRet != TRUE) {
        exit(ERR_DATA);
    }
    return TRUE;
}

inline int stack_pop_vm(lac_stack_t *pStack) {
    /* special pop method dedicated for VM(int) */

    int iRet;
    int iRes = 0;
    iRet = stack_pop(pStack, (void *) &iRes);
    if (iRet != TRUE) {
        /* Safe to return here, beacuse only the vm use this function, if there is no data,
         * then there is definitely an error.
         */
        printf("\n[ Warning ] Stack is empty, 0 is poped\n");
        return 0;
    }
#ifdef DEBUG
    if (pStack == &g_Env.StkData) {
        if (stack_is_empty(pStack)) {
            printf("\n[ Debug ] Pop %d from StkData, StkData.top = N/A\n", iRes);
        } else {
            printf("\n[ Debug ] Pop %d from StkData, StkData.top = %d\n", iRes, stack_top_vm(pStack));
        }
    } else if (pStack == &g_Env.StkReturn) {
        if (stack_is_empty(pStack)) {
            printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = N/A\n", iRes);
        } else {
            printf("\n[ Debug ] Pop %d from StkReturn, StkReturn.top = %d\n", iRes, stack_top_vm(pStack));
        }
    } else { ;
    }
    fflush(stdout);
#endif
    return iRes;
}

inline int stack_top_vm(lac_stack_t *pStack) {
    /* Access the top of stack without pop */

    if (stack_is_empty(pStack)) {
        printf("\n[ Warning ] Stack is empty\n");
        exit(ERR_DATA);
    } else {
        return *(int *) (pStack->pTop->pData);
    }
}

void proc_func_dup() {
    int iOp1 = *(int *) g_Env.StkData.pTop->pData;
    stack_push_vm(&g_Env.StkData, iOp1);
}

void proc_func_drop() {
    stack_pop_vm(&g_Env.StkData);
}

void proc_func_swap() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    stack_push_vm(&g_Env.StkData, iOp1);
    stack_push_vm(&g_Env.StkData, iOp2);
}

void proc_func_dot() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    printf("%d", iOp1);
    fflush(stdout);
}

//void proc_func_count() {
//    int iAddr = stack_pop_vm(&g_Env.StkData); // address of the string in the strtable
//    int iLength = (int) strlen(&g_Env.StrTable.cStr[iAddr]);
//    stack_push_vm(&g_Env.StkData, iAddr);
//    stack_push_vm(&g_Env.StkData, iLength);
//}
void proc_func_count() {
    int iAddr = stack_pop_vm(&g_Env.StkData); // address of the string in the strtable
    int iLength = vmtable_strlen(&g_Env.VMTable, iAddr);
    stack_push_vm(&g_Env.StkData, iAddr);
    stack_push_vm(&g_Env.StkData, iLength);
}

void proc_func_type() {
    int e = stack_pop_vm(&g_Env.StkData);
    int iAddr = stack_pop_vm(&g_Env.StkData);
    for (int i = 0; i < e; i++) {
        printf("%c", (char) g_Env.VMTable.OpCodes[iAddr + i]);
    }
}

void proc_func_equal() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    if (iOp1 == iOp2) {
        stack_push_vm(&g_Env.StkData, 1);
    } else {
        stack_push_vm(&g_Env.StkData, 0);
    }
}

void proc_func_mult() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    stack_push_vm(&g_Env.StkData, iOp1 * iOp2);
}

void proc_func_plus() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    stack_push_vm(&g_Env.StkData, iOp1 + iOp2);
}

void proc_func_minus() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    stack_push_vm(&g_Env.StkData, iOp2 - iOp1);
}

void proc_func_lit() {
    int iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, iAddr + 1);
    stack_push_vm(&g_Env.StkData, g_Env.VMTable.OpCodes[iAddr + 1]);
}

void proc_func_fin() {
    stack_pop_vm(&g_Env.StkReturn);
}

void proc_func_calculate() {
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int iLength = vmtable_strlen(&g_Env.VMTable, iAddr);
    char *psCalcString = calloc(iLength + 1, sizeof(char));
    for (int idx = 0; idx < iLength; idx++) {
        psCalcString[idx] = (char) g_Env.VMTable.OpCodes[iAddr + idx];
    }

    int ans;
    calculate_exp_int(psCalcString, iLength, &ans);

    stack_push_vm(&g_Env.StkData, ans);
}

void proc_func_if() {
    int iCond = stack_pop_vm(&g_Env.StkData); // [?] Consumes the top of data stack
    int iAddr;
    if (iCond > 0) {
        iAddr = stack_pop_vm(&g_Env.StkReturn);
        stack_push_vm(&g_Env.StkReturn, iAddr + 1);
    } else {
        iAddr = stack_pop_vm(&g_Env.StkReturn);
        stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);
    }
}

void proc_func_else() {
    /* If else symbol is encountered, then the if branch must be taken */
    /* Jump to the end of conditonal branch */

    int iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);

}

void proc_func_then() {
    ;
}

void proc_func_recurse() {
    /* This function is never used, since we handle recurse in compilation mode */
    ;
}

void proc_func_at() {
    /* read variable */
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int ans = g_Env.VMTable.OpCodes[iAddr];
    stack_push_vm(&g_Env.StkData, ans);
#ifdef DEBUG
    printf("\n[ Debug ] A value %d is pushed into data stack\n", ans);
    fflush(stdout);
#endif
}

void proc_func_exclaim() {
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int iVal = stack_pop_vm(&g_Env.StkData);
    g_Env.VMTable.OpCodes[iAddr] = iVal;
#ifdef DEBUG
    printf("\n[ Debug ] A value %d is set to VMTable[%d]\n", iVal, iAddr);
    fflush(stdout);
#endif
}

void proc_func_while() {
    ;
}

void proc_func_loop() {
    int iAddr;
    iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);
}

void proc_func_break() {
    int iAddr;
    iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);
}

void g_proc_env_init() {
    /* This function init the runtime environment */
    vmtable_init(&g_Env.VMTable);
    symtable_init(&g_Env.SymTable);
//    strtable_init(&g_Env.StrTable);
    stack_init(&g_Env.StkData);
    stack_init(&g_Env.StkReturn);
    g_Env.bInited = TRUE;
}

void g_proc_env_reset() {
    /* This function clear the data stack and return stack */
    stack_clear(&g_Env.StkData);
    stack_clear(&g_Env.StkReturn);
    g_Env.bInited = TRUE;
}

//void g_proc_env_clear() {
//    /* This function clear the data stacks and tables*/
//    stack_clear(&g_Env.StkData);
//    stack_clear(&g_Env.StkReturn);
//    symtable_clear(&g_Env.SymTable);
//    vmtable_clear(&g_Env.VMTable);
//    strtable_clear(&g_Env.StrTable);
//    g_Env.bInited = TRUE;
//}

void g_proc_compile() {
    /* The basic functions of processor need to be compiled in advance */
    /* The two tables (symtable and vmtable) are assumed as inited */

    /* Add the basic functions to SymTable and VMTable*/
    for (int idx = 0; idx < INIT_PROC_FUNC_NUM; ++idx) {
        /* modify the symbol table */
        symtable_add(&g_Env.SymTable, g_proc_basic_func_name[idx], (int) strlen(g_proc_basic_func_name[idx]));
        /* iLength -2 is the current  symbol's CFA (NOT VALID) */

        /* modify the vm table */
        vmtable_add(&g_Env.VMTable, g_proc_basic_func_type[idx]);
        vmtable_add(&g_Env.VMTable, idx);

        /* modify the symbol table CFA */
        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail - 1;

    }
    g_Env.bCompiled = TRUE;
}


void exec_vm(int iCFA) {
    /* This function execute the VMTable from iCFA */
    int iFuncCode;

    /* If sth goes wrong */
    if (iCFA < 0) {
        printf("\n[ Warning ] Illegal CFA: %d\n", iCFA);
        g_proc_env_reset();
        return;
    }
    /* If its a basic function or control function */
    if (g_Env.VMTable.OpCodes[iCFA] == VM_BASIC_FUNC_SYM || g_Env.VMTable.OpCodes[iCFA] == VM_CTRL_FUNC_SYM) {
        iFuncCode = g_Env.VMTable.OpCodes[iCFA + 1];
        if (iFuncCode >= INIT_PROC_FUNC_NUM || iFuncCode < 0) {
            printf("\n[ Warning ] Illegal VM Code: %c\n", iFuncCode);
            g_proc_env_reset();
            return;
        }
#ifdef DEBUG
        printf("\n[ Debug ] Execute basic function %s at VMTable[%d]\n", g_proc_basic_func_name[iFuncCode], iCFA);
        fflush(stdout);
#endif
        g_proc_basic_func[iFuncCode]();

        return;
    } else {
#ifdef DEBUG
        printf("\n[ Debug ] Execute lac function at VMTable[%d]\n", iCFA);
        fflush(stdout);
#endif
        /* Not basic function, then execute the rest of vm code, untile another function */
        int iVMIdx = iCFA;

        int iReturnStackCnt = g_Env.StkReturn.iLength; // Record the stack's length before entering lac function
        stack_push_vm(&g_Env.StkReturn, iVMIdx + 1);
        /* Recursive !, because we dont know if the next function is a basic function or lac function */
        exec_vm(g_Env.VMTable.OpCodes[iVMIdx + 1]);
        while (g_Env.StkReturn.iLength > iReturnStackCnt) {
            /* The fin will restore stack length to iReturnStackCnt, we then know the execution of a lac function is over*/
            iVMIdx = stack_pop_vm(&g_Env.StkReturn);
            stack_push_vm(&g_Env.StkReturn, iVMIdx + 1);

            /* [!] while and loop functions should be added here */

            /* Recursive ! */
#ifdef DEBUG
            printf("\n[ Debug ] Execute VMTable[%d]\n", iVMIdx + 1);
            fflush(stdout);
#endif
            exec_vm(g_Env.VMTable.OpCodes[iVMIdx + 1]);
        }
    }

}

void compile_function(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This fucnction compiles lac functions */

    /* The : is already popped, hear comes the function */
    lexeme_t LexTmp;
    u_symbol SymbolInput[MAX_WORD_LEN];
    int iSymbolIdx;
    int iStrIdx; // tmp value
    int iCFATmp; //tmp value
    int iCFACurr; // for the key word 'recursive' only
    int iImmVal; // Immediate value
    int iJumpStart; // tmp value, if jump start from stack
    int iBreakStart;// tmp value, while break start from stack
    int iWhileStart;// tmp value, while loop start from stack

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
    int iRet = lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }
    iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);
    if (iSymbolIdx < 0) {
        symtable_add(&g_Env.SymTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart);
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail;
        iCFACurr = g_Env.VMTable.iTail;
#ifdef DEBUG
        printf("\n[ Debug ] Define function %.*s with CFA = %d\n", LexTmp.iEnd - LexTmp.iStart,
               psReadBuffer + LexTmp.iStart, iCFACurr);
        fflush(stdout);
#endif
    } else {
        printf("\n[ Info ] Semantic risk, Duplicate definition, previous definition will be replaced\n");
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        symtable_set_cfa(&g_Env.SymTable, iSymbolIdx, g_Env.VMTable.iTail);
        iCFACurr = symtable_get_cfa(&g_Env.SymTable, iSymbolIdx);
        /* [!] this can be done better */
    }


    /* The : and the name is popped */
    while (TRUE) {
        if (queue_is_empty(pqueRes) == TRUE) {
            printf("\n[ Waring ] Syntax error, function is not defined properly\n");
            g_proc_env_reset();
            return;
        }
        queue_pop_front(pqueRes, (void *) &LexTmp);

        /* In function variables, note they are in fact global variables*/
        if (strncmp(psReadBuffer + LexTmp.iStart, "variable", 8) == 0 &&
            (LexTmp.iEnd - LexTmp.iStart) == 8) {
            /* The definition of variable will interrupt the function, use 'else' to skip it */
            vmtable_add(&g_Env.VMTable, VM_CFA_ELSE);
            vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 6);
            declare_var(psReadBuffer, pqueRes);
            continue;
        }

        /* In function vectors, note they are in fact global vectors*/
        if (strncmp(psReadBuffer + LexTmp.iStart, "vec", 3) == 0 &&
            (LexTmp.iEnd - LexTmp.iStart) == 3) {

            /* The definition of variable will interrupt the function, use 'else' as unconditional jump to skip it */
            vmtable_add(&g_Env.VMTable, VM_CFA_ELSE);

            int iVecStart = g_Env.VMTable.iTail + 1; // start of 'vec' jump
            vmtable_add(&g_Env.VMTable, 0); // pseudo value

            declare_vec(psReadBuffer, pqueRes); // declare the vec
            g_Env.VMTable.OpCodes[iVecStart] = g_Env.VMTable.iTail; // set the pre-defined pseudo value to real jump destination
            continue;
        }

        if (strncmp(psReadBuffer + LexTmp.iStart, ";", 1) == 0) {
            /* The end of definition */
            break;
        }
        if (strncmp(psReadBuffer + LexTmp.iStart, ":", 1) == 0) {
            /* The end of definition */
            printf("\n[ Waring ] Syntax error, function is not defined properly, missing ;\n");
            g_proc_env_reset();
            return;
        }

        /* Not the end of definition */

        if (LexTmp.type == WORD) {
            /* A word is encountered */
            iRet = lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
            if (iRet != TRUE) {
                g_proc_env_reset();
                return;
            }
            iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);
            if (iSymbolIdx < 0) {
                printf("\n[ Warning ] Semantic error, Undefined symbol: %.*s\n", LexTmp.iEnd - LexTmp.iStart,
                       psReadBuffer + LexTmp.iStart);
                g_proc_env_reset();
                return;
            } else {
                iCFATmp = symtable_get_cfa(&g_Env.SymTable, iSymbolIdx);
                /* if -> 26 else -> 28 then -> 30 */
                switch (iCFATmp) {
                    case VM_CFA_WHILE: {
                        stack_push_vm(&StkWhile, g_Env.VMTable.iTail + 1);
                        vmtable_add(&g_Env.VMTable, iCFATmp);
                        stack_push_vm(&StkBreak, -1); // Add an invalid CFA to break stack, mark the begin of loop
                        break;
                    }
                    case VM_CFA_BREAK: {
                        stack_push_vm(&StkBreak, g_Env.VMTable.iTail + 2);
                        vmtable_add(&g_Env.VMTable, iCFATmp);
                        vmtable_add(&g_Env.VMTable, -1); // loop end destination, now its an invalid value.
                        break;
                    }
                    case VM_CFA_LOOP: {
                        while (stack_is_empty(&StkBreak) != TRUE) {
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
                        vmtable_add(&g_Env.VMTable, iCFATmp);
                        vmtable_add(&g_Env.VMTable, iWhileStart);
                        break;
                    }
                    case VM_CFA_IF: {
                        stack_push_vm(&StkIf, g_Env.VMTable.iTail +
                                              2); // where if's jump position is stored, need to be filled after else/then is encountered
                        vmtable_add(&g_Env.VMTable, iCFATmp);
                        vmtable_add(&g_Env.VMTable, -1); // jump destination, now its an invalid value.
                        break;
                    }
                    case VM_CFA_ELSE: {
                        iJumpStart = stack_pop_vm(&StkIf); // if's jump position
                        g_Env.VMTable.OpCodes[iJumpStart] =
                                g_Env.VMTable.iTail + 2; // set the if's jump destination to this else

                        stack_push_vm(&StkElse, g_Env.VMTable.iTail +
                                                2); // where else's jump position is stored, need to be filled after else/then is encountered
                        vmtable_add(&g_Env.VMTable, iCFATmp);
                        vmtable_add(&g_Env.VMTable, -1); // jump destination, now its an invalid value.
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
                            vmtable_add(&g_Env.VMTable, iCFATmp);
                        } else {
                            /* Recursive function: Execute the current function */
                            vmtable_add(&g_Env.VMTable, iCFACurr);
                        }


                }
            }
        } else if (LexTmp.type == NUMBER) {
            /* Add a lit. [!] this can be done by directly add the opcode lit */
            vmtable_add(&g_Env.VMTable, VM_CFA_LIT); // 20 is the CFA of (lit) [!]
            iImmVal = (int) strtol(psReadBuffer + LexTmp.iStart, NULL, 10);
            vmtable_add(&g_Env.VMTable, iImmVal);
        } else if (LexTmp.type == STRING) {
            /* If a string is encountered */
            /* We store the string on the VMTable, so it must be skipped during execution */
            /* 'Else' function is the unconditional jump function, it is used to skip the string*/

            /* First add a unconditional jump to skip */
            int iStrLen = LexTmp.iEnd - LexTmp.iStart;
            vmtable_add(&g_Env.VMTable, VM_CFA_ELSE);
            vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + iStrLen + 2);

            /* Then store the string to VMTable */
            iStrIdx = g_Env.VMTable.iTail + 1; // Get the string's idx
            vmtable_add_vec(&g_Env.VMTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart, sizeof(char));

            /* The string is saved to iStrIdx, we save this value to vmtable*/
            /* Do the same as the situation of numbers (lit) + adddr */
            vmtable_add(&g_Env.VMTable, VM_CFA_LIT);
            vmtable_add(&g_Env.VMTable, iStrIdx);
        }

    }

    /* If-else syntax check */
    if (stack_is_empty(&StkIf) == FALSE || stack_is_empty(&StkElse) == FALSE) {
        printf("\n[ Warning ] Syntax error, mismatched 'If', 'else' and 'then'\n");
        g_proc_env_reset();
    }

    /* Add a fin. [!] this can be done by directly add the opcode fin */
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN); // 11 est le CFA de (fin)
    stack_clear(&StkIf);
    stack_clear(&StkElse);
    stack_clear(&StkWhile);
    stack_clear(&StkBreak);
}

void declare_function(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function declares a lac functions */

    /* The defer is already popped, hear comes the function(declare) */
    lexeme_t LexTmp;
    u_symbol SymbolInput[MAX_WORD_LEN];
    int iSymbolIdx;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);
    int iRet = lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }
    iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);
    if (iSymbolIdx < 0) {
        symtable_add(&g_Env.SymTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart);
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail;
#ifdef DEBUG
        printf("\n[ Debug ] Declare function %.*s with CFA = %d\n", LexTmp.iEnd - LexTmp.iStart,
               psReadBuffer + LexTmp.iStart, g_Env.VMTable.iTail);
        fflush(stdout);
#endif
    } else {
        printf("\n[ Warning ] Semantic error, Duplicate declaration of: %.*s\n", LexTmp.iEnd - LexTmp.iStart,
               psReadBuffer + LexTmp.iStart);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    }
    /* Add an error function and a (fin) */
    vmtable_add(&g_Env.VMTable, VM_CFA_ERROR);
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN);
}

void link_declaration(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function links function to declaration: Src -> Dst declares a lac functions */

    /* The ' is already popped, hear comes the function */
    /* ' Dst is Src */
    lexeme_t LexDst;
    lexeme_t LexSrc;
    lexeme_t LexIs;


    u_symbol SymbolInput[MAX_WORD_LEN];
    int iDstSymbolIdx;
    int iSrcSymbolIdx;
    int iDstCFA;
    int iSrcCFA;

    /* Get the name of declaration and function */
    queue_pop_front(pqueRes, (void *) &LexDst);
    queue_pop_front(pqueRes, (void *) &LexIs);
    queue_pop_front(pqueRes, (void *) &LexSrc);

    /* Syntax check */
    if (!(strncmp(psReadBuffer + LexIs.iStart, "is", 2) == 0 && (LexIs.iEnd - LexIs.iStart) == 2)) {
        printf("\n[ Warning ] Syntax error, expect 'is' after ' at %d\n", LexIs.iStart);
        g_proc_env_reset();
        return;
    }

    /* Get CFA of Dst (the well-defined function) */
    int iRet = lex_to_symbol(psReadBuffer, LexDst, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }
    iDstSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);
    if (iDstSymbolIdx < 0) {
        printf("\n[ Warning ] Semantic error, link Dst (%.*s) is invalid\n", LexDst.iEnd - LexDst.iStart,
               psReadBuffer + LexDst.iStart);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    } else {
        iDstCFA = symtable_get_cfa(&g_Env.SymTable, iDstSymbolIdx);
    }

    /* Get CFA of Src (the function declaration) */
    iRet = lex_to_symbol(psReadBuffer, LexSrc, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }

    iSrcSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);
    if (iSrcSymbolIdx < 0) {
        printf("\n[ Warning ] Semantic error, link Src (%.*s) is invalid\n", LexSrc.iEnd - LexSrc.iStart,
               psReadBuffer + LexSrc.iStart);
        g_proc_env_reset();
        return;
        /* [!] this can be done better */
    } else {
        iSrcCFA = symtable_get_cfa(&g_Env.SymTable, iSrcSymbolIdx);
    }

    /* Link */
    g_Env.VMTable.OpCodes[iSrcCFA + 1] = iDstCFA;
#ifdef DEBUG
    printf("\n[ Debug ] Link function %.*s to %.*s\n", LexSrc.iEnd - LexSrc.iStart, psReadBuffer + LexSrc.iStart,
           LexDst.iEnd - LexDst.iStart, psReadBuffer + LexDst.iStart);
    fflush(stdout);
#endif

}

void declare_var(char *psReadBuffer, lac_queue_t *pqueRes) {
    lexeme_t LexTmp;
    u_symbol SymbolInput[MAX_WORD_LEN];
    int iSymbolIdx;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);
    int iRet = lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }

    iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);

    if (iSymbolIdx < 0) {
        symtable_add(&g_Env.SymTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart);
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail;
#ifdef DEBUG
        printf("\n[ Debug ] A variable %.*s is defined at VMTable[%d]\n", LexTmp.iEnd - LexTmp.iStart,
               psReadBuffer + LexTmp.iStart, g_Env.VMTable.iTail);
#endif
    } else {
        printf("\n[ Info ] Semantic risk of (var), Duplicate definition, previous definition will be replaced\n");
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        symtable_set_cfa(&g_Env.SymTable, iSymbolIdx, g_Env.VMTable.iTail);
        /* [!] this can be done better */
    }
    vmtable_add(&g_Env.VMTable, VM_CFA_LIT);
    vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 3); // address of pseudo value
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN);
    vmtable_add(&g_Env.VMTable, 0); // pseudo value


}

void declare_vec(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* Declare a vector */

    lexeme_t LexTmp;
    u_symbol SymbolInput[MAX_WORD_LEN];
    int iSymbolIdx;

    /* Get the name of symbol */
    queue_pop_front(pqueRes, (void *) &LexTmp);
    int iRet = lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
    if (iRet != TRUE) {
        g_proc_env_reset();
        return;
    }

    iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput);

    if (iSymbolIdx < 0) {
        symtable_add(&g_Env.SymTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart);
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail;
#ifdef DEBUG
        printf("\n[ Debug ] A vector %.*s is defined at VMTable[%d]\n", LexTmp.iEnd - LexTmp.iStart,
               psReadBuffer + LexTmp.iStart, g_Env.VMTable.iTail);
#endif
    } else {
        printf("\n[ Info ] Semantic risk of (vec), Duplicate definition, previous definition will be replaced\n");
        vmtable_add(&g_Env.VMTable, VM_LAC_FUNC_SYM);
        symtable_set_cfa(&g_Env.SymTable, iSymbolIdx, g_Env.VMTable.iTail);
        /* [!] this can be done better */
    }

    /* Get the length of vector */
    queue_pop_front(pqueRes, (void *) &LexTmp);
    int iLength = (int) strtol(psReadBuffer + LexTmp.iStart, NULL, 10);

    /* First add the unconditional jump to skip the data area */
    vmtable_add(&g_Env.VMTable, VM_CFA_ELSE);
    vmtable_add(&g_Env.VMTable, g_Env.VMTable.iTail + 1 + iLength);

    int iVecIdx = g_Env.VMTable.iTail + 1; // address for the beginning of vector
    vmtable_add_vec(&g_Env.VMTable, NULL, iLength, sizeof(int)); // add iLength 0s to the VMTable
    vmtable_add(&g_Env.VMTable, VM_CFA_LIT);
    vmtable_add(&g_Env.VMTable, iVecIdx); // address of pseudo value
    vmtable_add(&g_Env.VMTable, VM_CFA_FIN);
}

void interpret(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function does LAC interpretation */

    lexeme_t LexTmp;
    int iRet;
    int iSymbolIdx;
    int iStrIdx;
    int iOpType; // Type of Operation
    int iCFA;

    u_symbol SymbolInput[MAX_WORD_LEN];

    if (g_Env.bInited == FALSE || g_Env.bCompiled == FALSE) {
        printf("\n[ Error ] VM Launch failed\n");
    }
#ifdef DEBUG
    printf("\n[ Info ] %d lexemes to process\n", pqueRes->iLength);
#endif

    while (queue_is_empty(pqueRes) != TRUE) {
        queue_pop_front(pqueRes, (void *) &LexTmp);
        /* Case it is a number */
        if (LexTmp.type == NUMBER) {
            iRet = (int) strtol(psReadBuffer + LexTmp.iStart, NULL, 10);
            stack_push_vm(&g_Env.StkData, iRet);
        }
            /* Case it is a word */
        else if (LexTmp.type == WORD) {

            /* case it is a :, which signify function definition */
            if (strncmp(psReadBuffer + LexTmp.iStart, ":", 1) == 0 && (LexTmp.iEnd - LexTmp.iStart) == 1) {
                compile_function(psReadBuffer, pqueRes);
            }

                /* case it is a defer */
            else if (strncmp(psReadBuffer + LexTmp.iStart, "defer", 5) == 0 && (LexTmp.iEnd - LexTmp.iStart) == 5) {
                declare_function(psReadBuffer, pqueRes);
            }

                /* case it is a ' */
            else if (strncmp(psReadBuffer + LexTmp.iStart, "'", 1) == 0 && (LexTmp.iEnd - LexTmp.iStart) == 1) {
                link_declaration(psReadBuffer, pqueRes);


            }
                /* case it is a variable */
            else if (strncmp(psReadBuffer + LexTmp.iStart, "variable", 8) == 0 &&
                     (LexTmp.iEnd - LexTmp.iStart) == 8) {
                declare_var(psReadBuffer, pqueRes);
            }
                /* case it is a vector */
            else if (strncmp(psReadBuffer + LexTmp.iStart, "vec", 3) == 0 &&
                     (LexTmp.iEnd - LexTmp.iStart) == 3) {
                declare_vec(psReadBuffer, pqueRes);
            }

                /* case it is a normal symbol */
            else {
                /* Translate the Lexeme to symbol */
                lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
                iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput); // The place of the symbol in symtable.
                if (iSymbolIdx < 0) {
                    /* Clear stacks if the symbol is not defined */
                    printf("\n[ Warning ] Symbol not defined: %.*s at %d\n", LexTmp.iEnd - LexTmp.iStart,
                           psReadBuffer + LexTmp.iStart, LexTmp.iStart);
                    g_proc_env_reset();
                    return;
                } else {
                    /* Verify the CFA is valid, (-1 for LAC or -2 for VM, not -3) */
                    iCFA = symtable_get_cfa(&g_Env.SymTable, iSymbolIdx);
                    iOpType = g_Env.VMTable.OpCodes[iCFA];
                    if (iOpType == VM_BASIC_FUNC_SYM || iOpType == VM_LAC_FUNC_SYM) {
                        /* Execute the VM */
                        exec_vm(iCFA);
                    } else if (iOpType == VM_CTRL_FUNC_SYM) {
                        switch (iCFA) {
                            case VM_CFA_RECURSE:
                            case VM_CFA_IF:
                            case VM_CFA_ELSE:
                            case VM_CFA_THEN:
                            case VM_CFA_WHILE:
                            case VM_CFA_LOOP:
                            case VM_CFA_BREAK:
                                printf("\n[ Warning ] Syntax error, invalid symbol in interpret mode, iCFA:%d\n", iCFA);
                                g_proc_env_reset();
                                break;
                            default:
                                exec_vm(iCFA);
                        }

                    } else {
                        printf("\n[ Warning ] Syntax error, invalid symbol, iCFA:%d\n", iCFA);
                        g_proc_env_reset();
                        return;
                        /* [!] */
                    }

                }
            }
        } else if (LexTmp.type == STRING) {
            /* If a string is encountered */

            /* First store the string to VMTable */
            iStrIdx = g_Env.VMTable.iTail + 1; // Get the idx(address) where the string will be stored
            vmtable_add_vec(&g_Env.VMTable, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart, sizeof(char));
#ifdef DEBUG
            printf("\n[ Info ] A string %.*s is added to VMTable [%d]\n", LexTmp.iEnd - LexTmp.iStart,
                   psReadBuffer + LexTmp.iStart, iStrIdx);
#endif
            /* The string is saved to iStrIdx, we push this value to data stack*/
            stack_push_vm(&g_Env.StkData, iStrIdx);
        }
    }

#ifdef DEBUG
    printf("\n[ Info ] Execution finished\n");
#endif
}
