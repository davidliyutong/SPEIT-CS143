//
// Created by 厉宇桐 on 2020/12/2.
//
// Function used to execute LAC

# include "interpret.h"

void exec_vm(lac_object_t *pLACFunctionObject) {
    /* This function execute the VMTable from iCFA */
    int CFACurr;
    int iVMIdx;
    e_vm_func_type FuncTypeCurr;

    /* If sth goes wrong */
    if (pLACFunctionObject == NULL) {
        printf("\n[ Warning ] Empty Function\n");
        g_env_reset();
        return;
    }
    lac_func_t *pFunction;

    switch (pLACFunctionObject->Type) {
        case LAC_VAR:
        case LAC_VEC:
        case LAC_CLASS:
        case LAC_INT:
            stack_push_data(&g_Env.StkData, pLACFunctionObject);
            return;
        case LAC_FUNC:
            pFunction = (lac_func_t *) pLACFunctionObject->Child;
            CFACurr = pFunction->iCFA;
            FuncTypeCurr = pFunction->FuncType;
            switch (FuncTypeCurr) {
                case VM_FUNC_BASIC:
                case VM_FUNC_CTRL:
                    if (CFACurr < 0) {
                        printf("\n[ Warning ] Invalid CFA:%d", CFACurr);
                        g_env_reset();
                        return;
                    }
#ifdef DEBUG
                    printf("\n[ Debug ] Execute basic function %s at VMTable[%d]\n", g_proc_basic_func_name[CFACurr], CFACurr);
                    fflush(stdout);
#endif
                    g_proc_basic_func[CFACurr]();
                    return;
                case VM_FUNC_LAC:
#ifdef DEBUG
                    printf("\n[ Debug ] Execute lac function %s\n", pLACFunctionObject->Name);
                    fflush(stdout);
#endif
                    /* Not basic function, then execute the rest of vm code, until another function */
                    iVMIdx = 0;

                    int iReturnStackCnt = g_Env.StkReturn.iLength; // Record the stack's length before entering lac function
                    stack_push_return(&g_Env.StkReturn, iVMIdx);
                    /* Recursive !, because we dont know if the next function is a basic function or lac function */
#ifdef DEBUG
                    printf("\n[ Debug ] Previous lac function execute VMTable[%d]\n", iVMIdx);
                    fflush(stdout);
#endif
                    g_Env.pLACVMTable = &pFunction->VMTable;
                    exec_vm(pFunction->VMTable.Objects[0]);
                    while (g_Env.StkReturn.iLength > iReturnStackCnt) {
                        /* The fin will restore stack length to iReturnStackCnt, we then know the execution of a lac function is over*/
                        iVMIdx = stack_pop_return(&g_Env.StkReturn);
                        stack_push_return(&g_Env.StkReturn, iVMIdx + 1);
#ifdef DEBUG
                        printf("\n[ Debug ] Previous lac function execute VMTable[%d]\n", iVMIdx + 1);
                        fflush(stdout);
#endif
                        /* Recursive ! */
                        g_Env.pLACVMTable = &pFunction->VMTable;
                        exec_vm(pFunction->VMTable.Objects[iVMIdx + 1]);
                    }
                    break;
                    /* The length of return stack is reduced to initial value, function return */
                default:
                    printf("\n[ Warning ] Not a function object: VMTable[%d], type: %d\n", CFACurr, FuncTypeCurr);
                    g_env_reset();
                    return;
            }
    }

}

void exec_symbol(lac_queue_t *pQueRes, lexeme_t LexTmp, compile_stat_t CompileStat) {
    hash_table_query_res SymbolQueryRes;
    lac_object_t *pLACObjectTmp; // Immediate var/vec
    lac_func_t *pLACFuncTmp;

    /* Translate the Lexeme to symbol */
    SymbolQueryRes = hash_symtable_search_all(LexTmp, CompileStat);
    if (SymbolQueryRes.pNode == NULL) {
        /* Clear stacks if the symbol is not defined */
        printf("\n[ Warning ] Symbol not defined: %.*s at %d\n", LexTmp.iLength, LexTmp.pString, LexTmp.iStart);
        g_env_reset();
        return;
    } else {
        /* Verify the CFA is valid */
        pLACObjectTmp = hash_symtable_get_obj(&g_Env.SymTable, LexTmp.pString, LexTmp.iLength);
        switch (pLACObjectTmp->Type) {
            case LAC_VAR:
                printf("\n[ Debug ] Variable\n");
            case LAC_VEC:
            case LAC_INT:
                stack_push_data(&g_Env.StkData, pLACObjectTmp);
                return;
            case LAC_FUNC:
                pLACFuncTmp = (lac_func_t *) pLACObjectTmp->Child;
                switch (pLACFuncTmp->FuncType) {
                    case VM_FUNC_BASIC:
                    case VM_FUNC_LAC:
                        exec_vm(pLACObjectTmp);
                        break;
                    case VM_FUNC_CTRL:
                        switch (pLACFuncTmp->iCFA) {
                            case VM_CFA_RECURSE:
                            case VM_CFA_IF:
                            case VM_CFA_ELSE:
                            case VM_CFA_THEN:
                            case VM_CFA_WHILE:
                            case VM_CFA_LOOP:
                            case VM_CFA_BREAK:
                            default:
                                /* [ could be done better */
                                printf("\n[ Warning ] Syntax Error, invalid symbol in interpret mode, iCFA:%d\n", pLACFuncTmp->iCFA);
                                g_env_reset();
                                break;
                        }
                        break;
                    default:
                        printf("\n[ Warning ] Syntax Error, invalid symbol:%s, iCFA:%d\n", pLACObjectTmp->Name, pLACFuncTmp->iCFA);
                        g_env_reset();
                        return;
                }
                break;
            case LAC_CLASS:
                printf("\n[ Debug ] Class\n");
                break;
        }
    }
}

void import_extern_lib(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    char sFileNameImport[MAX_LEXEME_LEN] = {0};
    char *psReadBufferImport = NULL;
    FILE *pInputFileImport;
    int iReadCntImport;
    lexeme_t FileName;
    lexeme_t ModuleName;
    lexeme_t AsTmp;
    queue_pop_front(pQueRes, (void *) &FileName);
    if (queue_is_empty(pQueRes) == TRUE) {
        ModuleName = FileName;
    } else {
        queue_pop_front(pQueRes, (void *) &AsTmp);
        /* import ... as ... or import ... */
        if (strncmp(AsTmp.pString, "as", 2) == 0 && AsTmp.iLength == 2) {
            queue_pop_front(pQueRes, (void *) &ModuleName);
        } else {
            queue_push_front(pQueRes, (void *) &AsTmp, sizeof(lexeme_t));
            ModuleName = FileName;
        }
    }


    hash_table_query_res SymbolQueryRes;
    SymbolQueryRes = hash_symtable_search_all(ModuleName, CompileStat);
    if (SymbolQueryRes.pNode != NULL) {
        /* The file is already imported */
#ifdef DEBUG
        printf("\n[ Debug ] Duplicate import\n");
#endif
        return;
    }

    /* Save the old symbol queue */
    lac_queue_t *pOldQueRes = g_Env.pQueRes;
    /* create a new queue for import to allow recursive import*/
    g_Env.pQueRes = queue_create();
    queue_init(g_Env.pQueRes);

    strncpy(sFileNameImport, FileName.pString, (size_t) FileName.iLength);
    pInputFileImport = fopen(sFileNameImport, "r");
    if (pInputFileImport == NULL) {
        printf("\n[ Warning ] Cannot find the file %s for import\n", sFileNameImport);
        /* restore the symbol queue */
        queue_destroy(g_Env.pQueRes);
        g_Env.pQueRes = pOldQueRes;
        g_env_reset();
        return;
    }

    lac_object_t *pLACNewModule = env_create_lac_func(ModuleName.pString, ModuleName.iLength, VM_CFA_LAC, VM_FUNC_LAC, NULL);
    hash_symtable_add_obj(&g_Env.SymTable, sFileNameImport, MAX_LEXEME_LEN, pLACNewModule);
    compile_stat_t NewCompileStat = CompileStat;
    strncpy(NewCompileStat.sScopeName, ModuleName.pString, (size_t) ModuleName.iLength);
    strncpy(NewCompileStat.sModuleName, ModuleName.pString, (size_t) ModuleName.iLength); // Module Name
    NewCompileStat.pFuncCurr = (lac_func_t *) pLACNewModule->Child;

    iReadCntImport = input_to_buffer(pInputFileImport, &psReadBufferImport);
    match_lac(psReadBufferImport, psReadBufferImport + iReadCntImport, g_Env.pQueRes);

    /* import, simply run the lac file, under a module, this is not hierarchical */
    interpret(g_Env.pQueRes, NewCompileStat);
#ifdef DEBUG
    printf("\n[ Debug ] Imported file %s \n", sFileNameImport);
#endif
    free(psReadBufferImport);

    /* restore the symbol queue */
    queue_destroy(g_Env.pQueRes);
    g_Env.pQueRes = pOldQueRes;

}

void interpret(lac_queue_t *pQueRes, compile_stat_t CompileStat) {
    /* This function does LAC interpretation */

    lexeme_t LexTmp;
    int iRet;
    compile_stat_t NewCompileStat;

    e_interpret_stat InterpretType;
    lac_object_t *pLACObjectTmp; // Immediate var/vec

#ifdef DEBUG
    printf("\n[ Info ] %d lexemes to process\n", pQueRes->iLength);
#endif

    while (queue_is_empty(pQueRes) != TRUE) {
        queue_pop_front(pQueRes, (void *) &LexTmp);
        /* Case it is a number */

        switch (LexTmp.type) {
            case NUMBER:
                iRet = (int) strtol(LexTmp.pString, NULL, 10);
                pLACObjectTmp = env_create_lac_int(iRet);
#ifdef DEBUG
                printf("\n[ Info ] A number %d is pushed into to StkData\n", iRet);
#endif
                stack_push_data(&g_Env.StkData, pLACObjectTmp);
                break; // end of case NUMBER
            case STRING:
                /* If a string is encountered */

                /* First store the string to VMTable */
                /*[!] create a vector that has the same name as string */
                pLACObjectTmp = env_create_lac_vec(NULL, 0, LexTmp.iLength, sizeof(char), LexTmp.pString);
#ifdef DEBUG
                printf("\n[ Info ] A string %.*s is pushed into to StkData\n", LexTmp.iLength, LexTmp.pString);
#endif
                /* The string is saved to iStrIdx, we push this value to data stack*/
                stack_push_data(&g_Env.StkData, pLACObjectTmp);
                break; // end of case STRING
            case WORD:
                /* Case it is a word */
                InterpretType = match_keyword(LexTmp);
                switch (InterpretType) {
                    case INTERPRET_DEF:
                        /* case it is a :, which signify function definition */
                        compile_function(pQueRes, CompileStat);
                        break;
                    case INTERPRET_CLASS:
                        NewCompileStat = CompileStat;
                        NewCompileStat.bIsClass = TRUE;
                        compile_function(pQueRes, NewCompileStat);
                        break;
                    case INTERPRET_DEFER:
                        /* case it is a defer */
                        declare_function(pQueRes, CompileStat);
                        break;
                    case INTERPRET_LINK:
                        /* case it is a ' */
                        link_declaration(pQueRes, CompileStat);
                        break;
                    case INTERPRET_VAR:
                        /* case it is a variable */
                        declare_var(pQueRes, CompileStat);
                        break;
                    case INTERPRET_VEC:
                        /* case it is a vector */
                        declare_vec(pQueRes, CompileStat);
                        break;
                    case INTERPRET_IMPORT:
                        /* case it is an import */
                        import_extern_lib(pQueRes, CompileStat);
                        break;
                    case INTERPRET_EXIT:
                        exit(0);
                    case INTERPRET_DEFAULT:
                    default:
                        exec_symbol(pQueRes, LexTmp, CompileStat);
                }
                break; // end of case WORD
            default:
                printf("\n[ Warning ] Syntax Error, invalid symbol:%.*s\n", LexTmp.iLength, LexTmp.pString);
                g_env_reset();
                return;
        }
    }

#ifdef DEBUG
    printf("\n[ Info ] Execution finished\n");
#endif
}
