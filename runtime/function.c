//
// Created by 厉宇桐 on 2020/12/23.
//

#include "function.h"

bool concat_func_name(char psDst[], char *psScope, char *psName, int iLength) {
    int iScopeLength = (int) strlen(psScope);
    memset(psDst, 0, sizeof(char) * MAX_LEXEME_LEN);
    if ((iLength + iScopeLength - 2) > MAX_LEXEME_LEN) {
        return FALSE;
    } else {
        strcpy(psDst, psScope);
        if (iScopeLength > 0) {
            psDst[iScopeLength] = '.';
            strncpy(psDst + iScopeLength + 1, psName, (size_t) iLength);
        } else {
            strncpy(psDst, psName, (size_t) iLength);
        }

        return TRUE;
    }
}

hash_table_query_res hash_symtable_search_all(lexeme_t LexTmp, compile_stat_t CompileStat) {
    /* Search local and global symbole table */
    /* Lexeme str should be prepared */
    char LexemeStrScope[MAX_LEXEME_LEN] = {0};
    char LexemeStrModule[MAX_LEXEME_LEN] = {0};
    int iRet;
    hash_table_query_res SymbolQueryRes = {-1, NULL};

    iRet = concat_func_name(LexemeStrScope, CompileStat.sScopeName, LexTmp.pString, LexTmp.iLength);
    if (!iRet) {
        /* Overflow */
        /* The variables/functions inside other functions will have father function name added before */
        printf("\n[ Warning ] Symbol Name over flow\n");
        g_env_reset();
        return SymbolQueryRes;
    }

    iRet = concat_func_name(LexemeStrModule, CompileStat.sModuleName, LexTmp.pString, LexTmp.iLength);
    if (!iRet) {
        /* Overflow */
        /* The variables/functions inside other functions will have father function name added before */
        printf("\n[ Warning ] Symbol Name over flow\n");
        g_env_reset();
        return SymbolQueryRes;
    }

    /* Search local */
    if (CompileStat.pFuncCurr != NULL) {
        SymbolQueryRes = hash_symtable_search(&CompileStat.pFuncCurr->SymTable, LexTmp.pString, LexTmp.iLength);
        if (SymbolQueryRes.pNode == NULL) {
            /* If failed, search global */
            SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexTmp.pString, LexTmp.iLength);
            if (SymbolQueryRes.pNode == NULL) {
                /* Try this method */
                SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexemeStrScope, MAX_LEXEME_LEN);
                if (SymbolQueryRes.pNode == NULL) {
                    /* Try this method */
                    SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexemeStrModule, MAX_LEXEME_LEN);
                }
            }
        }
    } else {
        SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexTmp.pString, LexTmp.iLength);
        if (SymbolQueryRes.pNode == NULL) {
            /* Try this method */
            SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexemeStrScope, MAX_LEXEME_LEN);
            if (SymbolQueryRes.pNode == NULL) {
                /* Try this method */
                SymbolQueryRes = hash_symtable_search(&g_Env.SymTable, LexemeStrModule, MAX_LEXEME_LEN);
            }
        }
    }

    return SymbolQueryRes;
}

bool compile_function(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    /* This fucnction compiles lac functions recursively*/

    /* The : is already popped, hear comes the function */
    lexeme_t LexTmp;
    int iImmVal; // Immediate value
    int iRet;
    bool bFuncCompileRes;
    hash_table_query_res SymbolQueryRes;
    e_interpret_stat InterpretType;

    lac_object_t *pLACObjectTmp;
    lac_func_t *pLACFuncTmp;
    char LexemeStrTmp[MAX_LEXEME_LEN];
    compile_stat_t NewCompileStat;

    /* Get the name of symbol */

    if (!CompileStat.bIf && !CompileStat.bWhile) { // compiling function or class with out if / while
        queue_pop_front(pQueRes, (void *) &LexTmp);
        if (LexTmp.type != WORD) {
            printf("\n[ Warning ] Function not properly named, invalid name\n");
            g_env_reset();
            return FALSE;
        } else {
            iRet = concat_func_name(LexemeStrTmp, CompileStat.sScopeName, LexTmp.pString, LexTmp.iLength);
            if (!iRet) {
                /* Overflow */
                /* The variables/functions inside other functions will have father function name added before */
                printf("\n[ Warning ] Symbol Name over flow\n");
                g_env_reset();
                return FALSE;
            }
            switch (CompileStat.bIsClass) {
                case TRUE:
                    pLACObjectTmp = env_create_lac_class(LexemeStrTmp, 0, VM_CFA_LAC, VM_FUNC_LAC, NULL);
                    break;
                case FALSE:
                    pLACObjectTmp = env_create_lac_func(LexemeStrTmp, 0, VM_CFA_LAC, VM_FUNC_LAC, NULL);
                    break;
            }
            iRet = hash_symtable_add_obj(&g_Env.SymTable, LexemeStrTmp, (int) strlen(LexemeStrTmp), pLACObjectTmp);
            CompileStat.pFuncCurr = (lac_func_t *) pLACObjectTmp->Child;
            if (iRet == ERR_SYMBOL_OVERFLOW) {
                printf("\n[ Warning ] Symbol Name overflow\n");
                g_env_reset();
                return FALSE;
            } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
                printf("\n[ Debug ] Define function %s \n", LexemeStrTmp);
                fflush(stdout);
#endif
            } else {
                printf("\n[ Info ] Semantic risk, Duplicate definition, previous definition will be replaced\n");
            }

            strcpy(CompileStat.sScopeName, LexemeStrTmp); // Modify the status.scope
        }
    }


    /* The : and the name is popped */
    while (TRUE) {
        if (queue_is_empty(pQueRes) == TRUE) {
            printf("\n[ Waring ] Syntax Error, function is not defined properly\n");
            g_env_reset();
            return FALSE;
        }
        if (CompileStat.pFuncCurr == NULL) {
            printf("\n[ Waring ] Syntax Error, try to process function definition that is not started\n");
            g_env_reset();/*[!] Could move this statement to outside */
            return FALSE;
        }

        queue_pop_front(pQueRes, (void *) &LexTmp);

        /* match keywords */
        InterpretType = match_keyword(LexTmp);
        switch (InterpretType) {
            case INTERPRET_DEF:
                bFuncCompileRes = compile_function(pQueRes, CompileStat);
                if (bFuncCompileRes == FALSE) {
                    return FALSE;
                }
                break;
            case INTERPRET_ENDDEF:
                if (!CompileStat.bWhile && !CompileStat.bIf) {
                    /* The end of definition */
                    /* Add a fin. [!] this can be done by directly add the opcode fin */
                    vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_FIN]); // le CFA de (fin)
                    /* Save the status of VMTable and SymTable */
                    hash_symtable_checkout(&CompileStat.pFuncCurr->SymTable);
                    hash_symtable_checkout(&g_Env.SymTable);
                    vmtable_checkout(&CompileStat.pFuncCurr->VMTable);
                    return TRUE;
                } else {
                    printf("\n[ Waring ] Syntax Error, function is not defined properly\n");
                    g_env_reset();/*[!] Could move this statement to outside */
                    return FALSE;
                }
            case INTERPRET_VAR:
                declare_var(pQueRes, CompileStat);
                continue;
            case INTERPRET_VEC:
                declare_vec(pQueRes, CompileStat);
                continue;
            case INTERPRET_DEFER:
                link_declaration(pQueRes, CompileStat);
                continue;
            case INTERPRET_CLASS:
                printf("\n[ Warning ] Defining class is not allowed here\n");
            case INTERPRET_IMPORT:
            case INTERPRET_EXIT: /*[!] actually is allowed here */
            case INTERPRET_LINK:
                printf("\n[ Waring ] Syntax Error, keyword not allowed here\n");
                g_env_reset();
                return FALSE;
            case INTERPRET_DEFAULT:
            default:
                break;
        }

        /* Not the end of definition */
        switch (LexTmp.type) {
            case WORD:
                /* A word is encountered */
                iRet = concat_func_name(LexemeStrTmp, CompileStat.sScopeName, LexTmp.pString, LexTmp.iLength);
                if (!iRet) {
                    /* Overflow */
                    /* The variables/functions inside other functions will have father function name added before */
                    printf("\n[ Warning ] Symbol Name over flow\n");
                    g_env_reset();
                    return FALSE;
                }

                SymbolQueryRes = hash_symtable_search_all(LexTmp, CompileStat);
                if (SymbolQueryRes.pNode == NULL) {
                    printf("\n[ Warning ] Semantic Error, Undefined symbol: %.*s\n", LexTmp.iLength, LexTmp.pString);
                    g_env_reset();
                    return FALSE;
                } else {
                    pLACObjectTmp = hash_symtable_get_obj_from_query(SymbolQueryRes);
                    switch (pLACObjectTmp->Type) {
                        case LAC_INT:
                        case LAC_VEC:
                        case LAC_VAR:
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);
                            continue;
                        case LAC_FUNC:
                            break;
                        case LAC_CLASS:
                            printf("\n[ Warning ] Semantic Error,trying to instance a class: %.*s inside function\n", LexTmp.iLength, LexTmp.pString);
                            continue;
                    }
                    /* if -> 26 else -> 28 then -> 30 */
                    pLACFuncTmp = (lac_func_t *) pLACObjectTmp->Child;
                    switch (pLACFuncTmp->iCFA) {
                        case VM_CFA_ERROR:
                            printf("\n[ Warning ] Unknown error\n");
                            g_env_reset();
                            return FALSE;

                        case VM_CFA_WHILE:
                            /* [ while(JR) | x+3 | JR  | LOOP_END | ... | break(JR) | x+1 | ... |   loop   | x+3(WHILE_START) |
                             * [    x      | x+1 | x+2 |   x+3    | ... |           |     | ... |          |     LOOP_END     |*/
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_WHILE]);

                            NewCompileStat = CompileStat;
                            NewCompileStat.bWhile = TRUE;
                            NewCompileStat.iWhileStart = CompileStat.pFuncCurr->VMTable.iTail; // x 

                            pLACObjectTmp = env_create_lac_int(CompileStat.pFuncCurr->VMTable.iTail + 3); // x + 3
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);

                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_JR]);
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, &vmtable_invalid_func);
                            iRet = compile_function(pQueRes, NewCompileStat);
                            if (iRet == FALSE) {
                                printf("\n[ Waring ] Syntax Error, invalid while loop\n");
                                g_env_reset();
                                return FALSE;
                            }
                            break;

                        case VM_CFA_BREAK:
                            if (CompileStat.bWhile != TRUE) {
                                printf("\n[ Waring ] Syntax Error, break not included in while block\n");
                                g_env_reset();
                                return FALSE;
                            }
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_BREAK]);
                            pLACObjectTmp = env_create_lac_int(CompileStat.iWhileStart + 1); // x + 1
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);
                            break;

                        case VM_CFA_LOOP:
                            if (CompileStat.bWhile != TRUE) {
                                printf("\n[ Waring ] Syntax Error, loop defined with out while\n");
                                g_env_reset();
                                return FALSE;
                            } else {
                                vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_LOOP]);
                                pLACObjectTmp = env_create_lac_int(CompileStat.iWhileStart + 3); // x + 3
                                vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);

                                pLACObjectTmp = env_create_lac_int(CompileStat.pFuncCurr->VMTable.iTail);
                                vmtable_set(&CompileStat.pFuncCurr->VMTable, CompileStat.iWhileStart + 3, pLACObjectTmp);
                                return TRUE;
                            }

                        case VM_CFA_IF:
                            /* [ IF | ELSE_END/COND_END | ... | ELSE(JR) | COND_END |   THEN   |
                             * [  x |        x+1        | ... |   ...    | ELSE_END | COND_END | */
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_IF]);
                            pLACObjectTmp = env_create_lac_int(-1);
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);

                            NewCompileStat = CompileStat;
                            NewCompileStat.bIf = TRUE;
                            NewCompileStat.iIfStart = CompileStat.pFuncCurr->VMTable.iTail; // x + 1
                            NewCompileStat.iElseStart = -1; // invalid
                            iRet = compile_function(pQueRes, NewCompileStat);
                            if (iRet == FALSE) {
                                printf("\n[ Waring ] Syntax Error, invalid while loop\n");
                                g_env_reset();
                                return FALSE;
                            }
                            break;

                        case VM_CFA_ELSE:
                            if (CompileStat.bIf != TRUE) {
                                printf("\n[ Waring ] Syntax Error, else defined with out if\n");
                                g_env_reset();
                                return FALSE;
                            }
                            if (CompileStat.iElseStart > 0) {
                                printf("\n[ Waring ] Syntax Error, more than one else in a condition\n");
                                g_env_reset();
                                return FALSE;
                            }
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_ELSE]);
                            pLACObjectTmp = env_create_lac_int(-1);
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);

                            CompileStat.iElseStart = CompileStat.pFuncCurr->VMTable.iTail;
                            break;

                        case VM_CFA_THEN:
                            if (CompileStat.bIf != TRUE) {
                                printf("\n[ Waring ] Syntax Error, else defined with out if\n");
                                g_env_reset();
                                return FALSE;
                            } else {
                                vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_THEN]);
                                if (CompileStat.iElseStart > 0) {
                                    /* If ... Else ... Then */
                                    ((lac_int_t *) CompileStat.pFuncCurr->VMTable.Objects[CompileStat.iIfStart]->Child)->iValue = CompileStat.iElseStart;
                                    ((lac_int_t *) CompileStat.pFuncCurr->VMTable.Objects[CompileStat.iElseStart]->Child)->iValue = CompileStat.pFuncCurr->VMTable.iTail;

                                } else {
                                    /* If ... Then */
                                    ((lac_int_t *) CompileStat.pFuncCurr->VMTable.Objects[CompileStat.iIfStart]->Child)->iValue = CompileStat.pFuncCurr->VMTable.iTail;
                                }
                                return TRUE;
                            }
                        case VM_CFA_RECURSE:
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_RECURSE]);
                            break;

                        default:
                            /* Recursive function: Execute the current function */
                            vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);

                    }
                }
                break;
            case STRING:
                /* If a string is encountered */
                pLACObjectTmp = env_create_lac_vec(NULL, 0, LexTmp.iLength, sizeof(char), LexTmp.pString);

                /* Then store the string */
                /* vmtable_add(&g_Env.VMTable, g_Env.VMTable.OpCodes[VM_CFA_LIT]); */
                vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);
                break;
            case NUMBER:
                /* Add a lit. This is no longer necessary*/
                /* vmtable_add(&g_Env.VMTable, g_Env.VMTable.OpCodes[VM_CFA_LIT]); */
                iImmVal = (int) strtol(LexTmp.pString, NULL, 10);
                pLACObjectTmp = env_create_lac_int(iImmVal);
                vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);
                break;
        }

    }
}

void declare_function(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    /* This function declares a lac functions */

    /* The defer is already popped, hear comes the function(declare) */
    lexeme_t FuncName;
    int iRet;
    lac_object_t *pLACObjectTmp;
    char LexemeStrTmp[MAX_LEXEME_LEN];

    /* Get the name of symbol */
    queue_pop_front(pQueRes, (void *) &FuncName);

    if (FuncName.type != WORD) {
        printf("\n[ Warning ] Function not properly named, invalid name\n");
        g_env_reset();
        return;
    }

    iRet = concat_func_name(LexemeStrTmp, CompileStat.sScopeName, FuncName.pString, FuncName.iLength);
    if (!iRet) {
        /* Overflow */
        /* The variables/functions inside other functions will have father function name added before */
        printf("\n[ Warning ] Symbol Name over flow\n");
        g_env_reset();
        return;
    }

    /* This is an invalid function */
    int iLength = (int) strlen(LexemeStrTmp);
    pLACObjectTmp = env_create_lac_func(LexemeStrTmp, iLength, VM_CFA_ERROR, VM_FUNC_LAC, NULL);

    iRet = hash_symtable_add_obj(&g_Env.SymTable, LexemeStrTmp, iLength, pLACObjectTmp);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        printf("\n[ Warning ] Symbol Name overflow\n");
        g_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] Declare function %.*s\n", iLength, LexemeStrTmp);
        fflush(stdout);
#endif
        if (CompileStat.pFuncCurr != NULL) {
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, LexemeStrTmp, iLength, pLACObjectTmp);
            hash_symtable_checkout(&CompileStat.pFuncCurr->SymTable);
        }

    } else {
        printf("\n[ Info ] Semantic risk of (defer), Duplicate declearation, previous definition will be replaced\n");
        /* Local variable in local symbol table */
        if (CompileStat.pFuncCurr != NULL) {
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, LexemeStrTmp, iLength, pLACObjectTmp);
            hash_symtable_checkout(&CompileStat.pFuncCurr->SymTable);
        }

    }

    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
}

void link_declaration(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    /* This function links function to declaration: Src -> Dst declares a lac functions */

    /* The ' is already popped, hear comes the function */
    /* ' Dst is Src */
    lexeme_t LexDst, LexSrc, LexIs;
    lac_object_t *pDstFunction;
    lac_object_t *pSrcFunction;
    hash_table_query_res DstSymbolQueryRes, SrcSymbolQueryRes;

    /* Get the name of declaration and function */
    queue_pop_front(pQueRes, (void *) &LexSrc);
    queue_pop_front(pQueRes, (void *) &LexIs);
    queue_pop_front(pQueRes, (void *) &LexDst);

    /* Syntax check */
    int iLexIsLength = LexIs.iEnd - LexIs.iStart;
    if (!(strncmp(LexIs.pString, "is", 2) == 0 && iLexIsLength == 2)) {
        printf("\n[ Warning ] Syntax Error, expect 'is' after ' at %d\n", LexIs.iStart);
        g_env_reset();
        return;
    }
    if (LexDst.iLength > MAX_LEXEME_LEN || LexSrc.iLength > MAX_LEXEME_LEN) {
        printf("\n[ Warning ] Symbol overflow\n");
        g_env_reset();
        return;
    }

    /* Get CFA of Dst (the well-defined function) */
    DstSymbolQueryRes = hash_symtable_search_all(LexDst, CompileStat);
    if (SrcSymbolQueryRes.pNode == NULL) {
        printf("\n[ Warning ] Semantic Error, link Dst (%.*s) is invalid\n", LexDst.iLength, LexDst.pString);
        g_env_reset();
        return;
    } else {
        pDstFunction = hash_symtable_get_obj(&g_Env.SymTable, LexDst.pString, LexDst.iLength);
    }

    /* Get CFA of Src (the function declaration) */
    SrcSymbolQueryRes = hash_symtable_search_all(LexSrc, CompileStat);
    if (SrcSymbolQueryRes.pNode == NULL) {
        printf("\n[ Warning ] Semantic Error, link Src (%.*s) is invalid\n", LexSrc.iLength, LexSrc.pString);
        g_env_reset();
        return;
        /* [!] this can be done better */
    } else {
        pSrcFunction = hash_symtable_get_obj(&g_Env.SymTable, LexSrc.pString, LexSrc.iLength);
    }

    /* Judge if the two are functions */
    if (pSrcFunction->Type == LAC_FUNC && pDstFunction->Type == LAC_FUNC) {
        /* Link */
        vmtable_clear(&((lac_func_t *) pSrcFunction->Child)->VMTable);
        vmtable_add(&((lac_func_t *) pSrcFunction->Child)->VMTable, pDstFunction);
        vmtable_add(&((lac_func_t *) pSrcFunction->Child)->VMTable, g_Env.BasicFuncTable.Objects[VM_CFA_FIN]);
        ((lac_func_t *) pSrcFunction->Child)->iCFA = VM_CFA_LAC;
#ifdef DEBUG
        printf("\n[ Debug ] Link function %.*s to %.*s\n", LexSrc.iLength, LexSrc.pString, LexDst.iLength, LexDst.pString);
        fflush(stdout);
#endif
    } else if (pSrcFunction->Type == LAC_FUNC && pDstFunction->Type == LAC_CLASS) {
        lac_func_t *pFunc = (lac_func_t *) pSrcFunction->Child;
        queue_node_t *pCursor;
        char LexemeStrTmp[MAX_LEXEME_LEN] = {0};
        char InstanceName[MAX_LEXEME_LEN] = {0};
        hash_table_entry *pOldEntryTmp;
        lac_object_t *pLACObjectTmp;
        int iRet;

        /* Modify the DstFunction to match SrcClass */
        env_instance_class(pDstFunction, pSrcFunction);
        /* Name of instance */
        strcpy(InstanceName, pSrcFunction->Name);

        for (int idx = 0; idx < HASH_TABLE_LEN; idx++) {
            pCursor = pFunc->SymTable.Data[idx].pFront;
            while (pCursor != NULL) {
                pOldEntryTmp = (hash_table_entry *) pCursor->pData;
                pLACObjectTmp = *(lac_object_t **) pOldEntryTmp->pData;
                iRet = concat_func_name(LexemeStrTmp, InstanceName, pOldEntryTmp->Key, (int) strlen(pOldEntryTmp->Key));
                if (!iRet) {
                    /* Overflow */
                    /* The variables/functions inside other functions will have father function name added before */
                    printf("\n[ Warning ] Symbol Name over flow\n");
                    g_env_reset();
                    return;
                }
                /* Register */
                hash_symtable_add_obj(&g_Env.SymTable, LexemeStrTmp, (int) strlen(LexemeStrTmp), pLACObjectTmp);
                queue_next(&pCursor);
            }
        }
    } else {
        printf("\n[ Warning ] Syntax Error, trying to link two non-functions\n");
        g_env_reset();
        return;
    }

    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
}

void declare_var(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    lexeme_t VarName;
    int iRet;
    lac_object_t *pLACObjectTmp;
    char LexemeStrTmp[MAX_LEXEME_LEN];


    /* Get the name of symbol */
    queue_pop_front(pQueRes, (void *) &VarName);
    if (VarName.type != WORD) {
        printf("\n[ Warning ] Variable not properly named, invalid name\n");
        g_env_reset();
        return;
    }

    iRet = concat_func_name(LexemeStrTmp, CompileStat.sScopeName, VarName.pString, VarName.iLength);
    if (!iRet) {
        /* Overflow */
        /* The variables/functions inside other functions will have father function name added before */
        printf("\n[ Warning ] Symbol Name over flow\n");
        g_env_reset();
        return;
    }

    pLACObjectTmp = env_create_lac_var(LexemeStrTmp, (int) strlen(LexemeStrTmp), NULL);
//    if (CompileStat.pFuncCurr != NULL) {
//        /* Not in mode interpret */
//        vmtable_add(&CompileStat.pFuncCurr->VMTable, pLACObjectTmp);
//        vmtable_checkout(&CompileStat.pFuncCurr->VMTable);
//
//    }

    iRet = hash_symtable_add_obj(&g_Env.SymTable, LexemeStrTmp, (int) strlen(LexemeStrTmp), pLACObjectTmp);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        printf("\n[ Warning ] Symbol Name overflow\n");
        g_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] A variable %s is defined]\n", LexemeStrTmp);
#endif
        if (CompileStat.pFuncCurr != NULL) {
            /* Not in mode interpret */
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, VarName.pString, VarName.iLength, pLACObjectTmp);
            hash_symtable_checkout(&CompileStat.pFuncCurr->SymTable);
        }

    } else {
        printf("\n[ Info ] Semantic risk of (var), Duplicate definition, previous definition will be replaced\n");
        /* Local variable in local symbol table */
        if (CompileStat.pFuncCurr != NULL) {
            /* Not in mode interpret */
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, VarName.pString, VarName.iLength, pLACObjectTmp);
            hash_symtable_checkout(&CompileStat.pFuncCurr->SymTable);
        }
    }

    /* Save the status of SymbolTable */
    hash_symtable_checkout(&g_Env.SymTable);
}


void declare_vec(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    /* Declare a vector */

    lexeme_t VecName;
    lexeme_t VecLength;

    int iRet;
    lac_object_t *pLACObjectTmp;
    char LexemeStrTmp[MAX_LEXEME_LEN];

    /* Get the name of symbol */
    queue_pop_front(pQueRes, (void *) &VecName);

    if (VecName.type != WORD) {
        printf("\n[ Warning ] Vec not properly named, invalid name\n");
        g_env_reset();
        return;
    }

    iRet = concat_func_name(LexemeStrTmp, CompileStat.sScopeName, VecName.pString, VecName.iLength);
    if (!iRet) {
        /* Overflow */
        /* The variables/functions inside other functions will have father function name added before */
        printf("\n[ Warning ] Symbol Name over flow\n");
        g_env_reset();
        return;
    }

    /* Get the length of vector */
    queue_pop_front(pQueRes, (void *) &VecLength);
    int iVecLen = (int) strtol(VecLength.pString, NULL, 10);
    if (iVecLen <= 0) {
        /* There is a syntax error */
        printf("\n[ Warning ] Vec not properly defined, invalid length\n");
        g_env_reset();
        return;
    }

    pLACObjectTmp = env_create_lac_vec(VecName.pString, VecName.iLength, iVecLen, 4, NULL);

    iRet = hash_symtable_add_obj(&g_Env.SymTable, LexemeStrTmp, (int) strlen(LexemeStrTmp), pLACObjectTmp);
    if (iRet == ERR_SYMBOL_OVERFLOW) {
        printf("\n[ Warning ] Symbol Name overflow\n");
        g_env_reset();
        return;
    } else if (iRet == INFO_SYMBOL_NOT_FOUND) {
#ifdef DEBUG
        printf("\n[ Debug ] A vector %.*s is defined\n", VecName.iLength, VecName.pString);
#endif
        if (CompileStat.pFuncCurr != NULL) {
            /* Not in mode interpret */
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, VecName.pString, VecName.iLength, pLACObjectTmp);
            hash_symtable_checkout(&g_Env.SymTable);
        }

    } else {
        printf("\n[ Info ] Semantic risk of (vec), Duplicate definition, previous definition will be replaced\n");
        if (CompileStat.pFuncCurr != NULL) {
            /* Not in mode interpret */
            hash_symtable_add_obj(&CompileStat.pFuncCurr->SymTable, VecName.pString, VecName.iLength, pLACObjectTmp);
            hash_symtable_checkout(&g_Env.SymTable);
        }
    }
#ifdef DEBUG
    printf("\n[ Debug ] A vector %.*s of length %d is defined\n", VecName.iLength, VecName.pString, iVecLen);
#endif

    /* Save the status of SymbolTable */
    hash_symtable_checkout(&g_Env.SymTable);
}