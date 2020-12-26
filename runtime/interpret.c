//
// Created by 厉宇桐 on 2020/12/2.
//
// Function used to execute LAC

# include "interpret.h"

inline enum e_interpret_keyword match_keyword(char *psReadBuffer, struct lexeme_t LexTmp) {
    int iLength = LexTmp.iEnd - LexTmp.iStart;
    if (strncmp(psReadBuffer + LexTmp.iStart, ":", 1) == 0 && (iLength) == 1) {
        return INTERPRET_DEF;
    }

    /* case it is a defer */
    if (strncmp(psReadBuffer + LexTmp.iStart, "defer", 5) == 0 && (iLength) == 5) {
        return INTERPRET_DEFER;
    }

    /* case it is a ' */
    if (strncmp(psReadBuffer + LexTmp.iStart, "'", 1) == 0 && (iLength) == 1) {
        return INTERPRET_LINK;
    }

    /* case it is a variable */
    if (strncmp(psReadBuffer + LexTmp.iStart, "variable", 8) == 0 && (iLength) == 8) {
        return INTERPRET_VAR;
    }
    /* case it is a vector */
    if (strncmp(psReadBuffer + LexTmp.iStart, "vec", 3) == 0 && (iLength) == 3) {
        return INTERPRET_VEC;
    }

    if (strncmp(psReadBuffer + LexTmp.iStart, "exit", 4) == 0 && (iLength) == 4) {
        return INTERPRET_EXIT;
    }
    /* case it is an import */
    if (strncmp(psReadBuffer + LexTmp.iStart, "import", 6) == 0 &&
        (LexTmp.iEnd - LexTmp.iStart) == 6) {
        return INTERPRET_IMPORT;

    }

    return INTERPRET_DEFAULT;
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
#ifdef DEBUG
        if (iFuncCode >= INIT_PROC_FUNC_NUM || iFuncCode < 0) {
            /* This is not likely to happen in real life, but good for debug */
            printf("\n[ Warning ] Illegal VM Code: %d\n", iFuncCode);
            g_proc_env_reset();
            return;
        }
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

void import_extern_lib(const char *psReadBuffer, lac_queue_t *pqueRes, lexeme_t *LexTmp) {
    char sFileNameImport[MAX_LEXEME_LEN] = {0};
    char *psReadBufferImport = NULL;
    FILE *pInputFileImport;
    int iReadCntImport;
    lac_queue_t queResImport;
    queue_init(&queResImport);

    queue_pop_front(pqueRes, (void *) LexTmp);
    strncpy(sFileNameImport, psReadBuffer + (*LexTmp).iStart, (size_t) ((*LexTmp).iEnd - (*LexTmp).iStart));
    pInputFileImport = fopen(sFileNameImport, "r");
    if (pInputFileImport == NULL) {
        printf("\n[ Warning ] Cannot find the file %s for import\n", sFileNameImport);
        g_proc_env_reset();
        return;
    }

    iReadCntImport = input_to_buffer(pInputFileImport, &psReadBufferImport);
    match_lac(psReadBufferImport, psReadBufferImport + iReadCntImport, &queResImport);

    /*  import, simply run the lac file */
    interpret(psReadBufferImport, &queResImport);
#ifdef DEBUG
    printf("\n[ Debug ] Imported file %s \n", sFileNameImport);
    queue_clear(&queResImport);
#endif
    free(psReadBufferImport);
}

void interpret(char *psReadBuffer, lac_queue_t *pqueRes) {
    /* This function does LAC interpretation */

    lexeme_t LexTmp;
    int iRet;
    int iSymbolIdx; // The place to store symbol
    int iStrIdx; //  The place to store string
    int iOpType; // Type of Operation
    enum e_interpret_keyword InterpretType;
    int iCFA = VM_CFA_ERROR; //Code field address

    u_symbol SymbolInput[MAX_LEXEME_LEN];

#ifdef DEBUG
    printf("\n[ Info ] %d lexemes to process\n", pqueRes->iLength);
#endif

    while (queue_is_empty(pqueRes) != TRUE) {
        queue_pop_front(pqueRes, (void *) &LexTmp);
        /* Case it is a number */
        switch (LexTmp.type) {
            case NUMBER:
                iRet = (int) strtol(psReadBuffer + LexTmp.iStart, NULL, 10);
                stack_push_vm(&g_Env.StkData, iRet);
                break; // end of case NUMBER
            case WORD:
                /* Case it is a word */
                InterpretType = match_keyword(psReadBuffer, LexTmp);
                switch (InterpretType) {
                    case INTERPRET_DEF:
                        /* case it is a :, which signify function definition */
                        compile_function(psReadBuffer, pqueRes);
                        break;
                    case INTERPRET_DEFER:
                        /* case it is a defer */
                        declare_function(psReadBuffer, pqueRes);
                        break;
                    case INTERPRET_LINK:
                        /* case it is a ' */
                        link_declaration(psReadBuffer, pqueRes);
                        break;
                    case INTERPRET_VAR:
                        /* case it is a variable */
                        declare_var(psReadBuffer, pqueRes);
                        break;
                    case INTERPRET_VEC:
                        /* case it is a vector */
                        declare_vec(psReadBuffer, pqueRes);
                        break;
                    case INTERPRET_IMPORT:
                        /* case it is an import */
                        import_extern_lib(psReadBuffer, pqueRes, &LexTmp);
                        break;
                    case INTERPRET_EXIT:
                        exit(0);
                    case INTERPRET_DEFAULT:
                    default:
                        /* Translate the Lexeme to symbol */
                        lex_to_symbol(psReadBuffer, LexTmp, SymbolInput);
                        iSymbolIdx = symtable_search(&g_Env.SymTable, SymbolInput); // The place of the symbol in symtable.
                        if (iSymbolIdx < 0) {
                            /* Clear stacks if the symbol is not defined */
                            printf("\n[ Warning ] Symbol not defined: %.*s at %d\n", LexTmp.iEnd - LexTmp.iStart, psReadBuffer + LexTmp.iStart, LexTmp.iStart);
                            g_proc_env_reset();
                            return;
                        } else {
                            /* Verify the CFA is valid, (-1 for LAC or -2 for VM, not -3) */
                            iCFA = symtable_get_cfa_by_idx(&g_Env.SymTable, iSymbolIdx);
                            iOpType = g_Env.VMTable.OpCodes[iCFA];
                            switch (iOpType) {
                                case VM_BASIC_FUNC_SYM:
                                case VM_LAC_FUNC_SYM:
                                    exec_vm(iCFA);
                                    break;
                                case VM_CTRL_FUNC_SYM:
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
                                    break;
                                default:
                                    printf("\n[ Warning ] Syntax error, invalid symbol, iCFA:%d\n", iCFA);
                                    g_proc_env_reset();
                                    return;
                            }
                        }
                }
                break; // end of case WORD
            case STRING:
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
                break; // end of case STRING
            default:
                printf("\n[ Warning ] Syntax error, invalid symbol, iCFA:%d\n", iCFA);
                g_proc_env_reset();
                return;
        }
    }

#ifdef DEBUG
    printf("\n[ Info ] Execution finished\n");
#endif
}
