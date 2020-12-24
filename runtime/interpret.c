//
// Created by 厉宇桐 on 2020/12/2.
//
// Function used to execute LAC

# include "interpret.h"

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
            /* This is not likely to happen in real life, but good for debug */
            printf("\n[ Warning ] Illegal VM Code: %d\n", iFuncCode);
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
#ifdef DEBUG
        printf("\n[ Debug ] Previous lac function execute VMTable[%d], VMTable[%d] = %d\n", iVMIdx + 1, iVMIdx + 1, g_Env.VMTable.OpCodes[iVMIdx + 1]);
#endif
        exec_vm(g_Env.VMTable.OpCodes[iVMIdx + 1]);
        while (g_Env.StkReturn.iLength > iReturnStackCnt) {
            /* The fin will restore stack length to iReturnStackCnt, we then know the execution of a lac function is over*/
            iVMIdx = stack_pop_vm(&g_Env.StkReturn);
            stack_push_vm(&g_Env.StkReturn, iVMIdx + 1);
#ifdef DEBUG
            printf("\n[ Debug ] Previous lac function execute VMTable[%d], VMTable[%d] = %d\n", iVMIdx + 1, iVMIdx + 1, g_Env.VMTable.OpCodes[iVMIdx + 1]);
            fflush(stdout);
#endif
            /* Recursive ! */
            exec_vm(g_Env.VMTable.OpCodes[iVMIdx + 1]);
        }
        /* The length of return stack is reduced to initial value, function return */
    }

}

void interpret(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function does LAC interpretation */

    lexeme_t LexTmp;
    int iRet;
    int iSymbolIdx; // The place to stor symbol
    int iStrIdx; //  The place to store string
    int iOpType; // Type of Operation
    int iCFA; //Code field address

    /* For import, simply run the lac file */
    char sFileNameImport[MAX_LEXEME_LEN] = {0};
    char *psReadBufferImport = NULL;
    FILE *pInputFileImport;
    int iReadCntImport;
    lac_queue_t queResImport;
    queue_init(&queResImport);

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
                /* case it is an import */
            else if (strncmp(psReadBuffer + LexTmp.iStart, "import", 6) == 0 &&
                     (LexTmp.iEnd - LexTmp.iStart) == 6) {
                queue_pop_front(pqueRes, (void *) &LexTmp);
                strncpy(sFileNameImport, psReadBuffer + LexTmp.iStart, LexTmp.iEnd - LexTmp.iStart);
                pInputFileImport = fopen(sFileNameImport, "r");
                if (pInputFileImport == NULL) {
                    printf("\n[ Warning ] Cannot find the file %s for import\n", sFileNameImport);
                    g_proc_env_reset();
                    return;
                }
                iReadCntImport = input_to_buffer(pInputFileImport, &psReadBufferImport);
                match_lac(psReadBufferImport, psReadBufferImport + iReadCntImport, &queResImport);
                interpret(psReadBufferImport, &queResImport);
#ifdef DEBUG
                printf("\n[ Debug ] Imported file %s \n", sFileNameImport);
                queue_clear(&queResImport);
#endif
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
            vmtable_checkout(&g_Env.VMTable);
        }
    }

#ifdef DEBUG
    printf("\n[ Info ] Execution finished\n");
#endif
}
