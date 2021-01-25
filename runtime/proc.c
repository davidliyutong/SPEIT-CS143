//
// Created by 厉宇桐 on 2020/12/23.
//

#include "proc.h"

/*
    dup (n – n n)       : duplique le sommet de la pile de données.
    drop (n – )         : supprime le sommet de la pile de données.
    swap (n1 n2 – n2 n1): échange les deux éléments sur le haut de la pile de données.
    . (e – )            : imprime l’entier qui est sur le sommet de la pile de données et le supprime.
    count (ad – ad’ e)  : prend l’adresse de chaîne en haut de la pile, et la remplace par l’adresse du début de la chaîne sans la longueur et met la longueur sur le sommet.
    type (ad e – )      : imprime e caractères dont les codes sont à partir de ad.
    = (e1 e2 – b)       : teste si e1 = e2 et empile un booléen.
    >                   :
    <                   :
    *                   :
    +                   :
    -                   :
    /                   :
    (lit)               :
    (fin)               :
    (lac)               :
    calculate           :
    if                  :
    else                :
    then                :
    recurse             :
    @                   :
    !                   :
    while               :
    loop                :
    break               :
    jr                  :
    

*/

basic g_proc_basic_func[INIT_PROC_FUNC_NUM] = {proc_func_dup,
                                               proc_func_drop,
                                               proc_func_swap,
                                               proc_func_dot,
                                               proc_func_count,
                                               proc_func_type,
                                               proc_func_equal,
                                               proc_func_gt,
                                               proc_func_lt,
                                               proc_func_mult,
                                               proc_func_plus,
                                               proc_func_minus,
                                               proc_func_div,
                                               proc_func_lit,
                                               proc_func_fin,
                                               proc_func_lac,
                                               proc_func_calculate,
                                               proc_func_if,
                                               proc_func_else,
                                               proc_func_then,
                                               proc_func_recurse,
                                               proc_func_at,
                                               proc_func_exclaim,
                                               proc_func_while,
                                               proc_func_loop,
                                               proc_func_break,
                                               proc_func_jr};

char g_proc_basic_func_name[INIT_PROC_FUNC_NUM][10] = {"dup",
                                                       "drop",
                                                       "swap",
                                                       ".",
                                                       "count",
                                                       "type",
                                                       "=",
                                                       ">",
                                                       "<",
                                                       "*",
                                                       "+",
                                                       "-",
                                                       "/",
                                                       "(lit)",
                                                       "(fin)",
                                                       "(lac)",
                                                       "calculate",
                                                       "if",
                                                       "else",
                                                       "then",
                                                       "recurse",
                                                       "@",
                                                       "!",
                                                       "while",
                                                       "loop",
                                                       "break",
                                                       "jr"};

e_vm_func_type g_proc_basic_func_type[INIT_PROC_FUNC_NUM] = {VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC, // div
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL, // recurse
                                                             VM_FUNC_BASIC, // @
                                                             VM_FUNC_BASIC,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL,
                                                             VM_FUNC_CTRL
};


void proc_func_dup() {
    lac_object_t *Op = stack_top_data(&g_Env.StkData);
    stack_push_data(&g_Env.StkData, Op);
}

void proc_func_drop() {
    stack_pop_data(&g_Env.StkData);
}

void proc_func_swap() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    stack_push_data(&g_Env.StkData, Op1);
    stack_push_data(&g_Env.StkData, Op2);
}

void proc_func_dot() {
    lac_object_t *Op = stack_pop_data(&g_Env.StkData);
    if (Op->Type == LAC_VAR) Op = ((lac_var_t *) Op->Child)->Object;
    int idx;

    switch (Op->Type) {
        case LAC_CLASS:
            printf("%s", Op->Name);
            break;
        case LAC_FUNC:
            printf("%s:%d", Op->Name, ((lac_func_t *) Op->Child)->iCFA);
            break;
        case LAC_INT:
            printf("%d", ((lac_int_t *) Op->Child)->iValue);
            break;
        case LAC_VEC:
            idx = 0;
            printf("[");
            while (idx < ((lac_vec_t *) Op->Child)->iLength) {
                printf("%d", ((lac_vec_t *) Op->Child)->pData[idx]);
                if (idx < ((lac_vec_t *) Op->Child)->iLength - 1) {
                    printf(", ");
                }
                ++idx;
            }
            printf("]");
            break;
        case LAC_VAR:
            printf("\n[ Warning ] Syntax Error, trying to dot a var(var)\n");
            g_env_reset();
            return;
    }
    fflush(stdout);
}

void proc_func_count() {
    lac_object_t *Op = stack_top_data(&g_Env.StkData); // address of the string in the strtable
    lac_object_t *Res;
    int iCnt;
    if (Op->Type == LAC_VAR) Op = ((lac_var_t *) Op->Child)->Object;
    switch (Op->Type) {
        case LAC_VEC:
            iCnt = vmtable_strlen(Op->Child);
            Res = env_create_lac_int(iCnt);
            stack_push_data(&g_Env.StkData, Res);
            break;
        case LAC_FUNC:
        case LAC_VAR:
        case LAC_INT:
        default:
            printf("\n[ Warning ] Syntax Error, trying to count a function / var, type:%d\n", Op->Type);
            g_env_reset();
            return;
    }
}

void proc_func_type() {
    lac_object_t *Count = stack_pop_data(&g_Env.StkData);
    lac_object_t *Str = stack_pop_data(&g_Env.StkData);
    if (Count->Type == LAC_VAR) Count = ((lac_var_t *) Count->Child)->Object;
    if (Str->Type == LAC_VAR) Str = ((lac_var_t *) Str->Child)->Object;

    if (Count->Type == LAC_INT && Str->Type == LAC_VEC) {
        for (int idx = 0; idx < ((lac_int_t *) Count->Child)->iValue; idx++) {
            printf("%c", (char) (((lac_vec_t *) Str->Child)->pData[idx]));
        }
    } else {
        printf("\n[ Warning ] Syntax Error, trying to type a function / var or the count is incorrect\n");
        g_env_reset();
        return;
    }
}

void proc_func_equal() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    bool bFlag;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        if (((lac_int_t *) Op1->Child)->iValue == ((lac_int_t *) Op2->Child)->iValue) {
            bFlag = TRUE;
        } else {
            bFlag = FALSE;
        }
    } else if (Op1->Type == LAC_VEC && Op2->Type == LAC_VEC) {
        lac_vec_t *Vec1 = ((lac_vec_t *) Op1->Child);
        lac_vec_t *Vec2 = ((lac_vec_t *) Op2->Child);
        if (Vec1->iLength == Vec2->iLength && Vec1->iRef == Vec2->iRef && Vec1->iSize == Vec2->iSize) {
            if (memcmp(Vec1->pData, Vec2->pData, Vec1->iSize * Vec1->iLength)) {
                bFlag = TRUE;
            } else {
                bFlag = FALSE;
            }
        } else {
            bFlag = FALSE;
        }

    } else {
        printf("\n[ Warning ] Syntax Error, trying to evaluate two incomparable types\n");
        g_env_reset();
        return;
    }

    if (bFlag == TRUE) {
        Res = env_create_lac_int(1);
        stack_push_data(&g_Env.StkData, Res);
    } else {
        Res = env_create_lac_int(0);
        stack_push_data(&g_Env.StkData, Res);
    }
    return;
}

void proc_func_gt() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        if (((lac_int_t *) Op2->Child)->iValue > ((lac_int_t *) Op1->Child)->iValue) {
            Res = env_create_lac_int(1);
            stack_push_data(&g_Env.StkData, Res);
        } else {
            Res = env_create_lac_int(0);
            stack_push_data(&g_Env.StkData, Res);

        }
    } else {
        printf("\n[ Warning ] Syntax Error, trying to compare two non-var types\n");
        g_env_reset();
        return;
    }

}

void proc_func_lt() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        if (((lac_int_t *) Op2->Child)->iValue < ((lac_int_t *) Op1->Child)->iValue) {
            Res = env_create_lac_int(1);
            stack_push_data(&g_Env.StkData, Res);
        } else {
            Res = env_create_lac_int(0);
            stack_push_data(&g_Env.StkData, Res);

        }
    } else {
        printf("\n[ Warning ] Syntax Error, trying to compare two non-var types\n");
        g_env_reset();
        return;
    }
}

void proc_func_mult() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        Res = env_create_lac_int(0);
        ((lac_int_t *) Res->Child)->iValue = ((lac_int_t *) Op1->Child)->iValue * ((lac_int_t *) Op2->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else {
        printf("\n[ Warning ] Syntax Error, trying to compare two non-var types\n");
        g_env_reset();
        return;
    }
}

void proc_func_plus() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        Res = env_create_lac_int(0);
        ((lac_int_t *) Res->Child)->iValue = ((lac_int_t *) Op1->Child)->iValue + ((lac_int_t *) Op2->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else if (Op1->Type == LAC_INT && Op2->Type == LAC_VEC) {
        Res = env_create_lac_vec(NULL, 0, ((lac_vec_t *) Op2->Child)->iLength, ((lac_vec_t *) Op2->Child)->iSize, NULL);
        ((lac_vec_t *) Res->Child)->pData = ((lac_vec_t *) Op2->Child)->pData;
        ((lac_vec_t *) Res->Child)->iRef = ((lac_vec_t *) Op1->Child)->iRef + ((lac_int_t *) Op2->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else if (Op2->Type == LAC_INT && Op1->Type == LAC_VEC) {
        Res = env_create_lac_vec(NULL, 0, ((lac_vec_t *) Op1->Child)->iLength, ((lac_vec_t *) Op1->Child)->iSize, NULL);
        ((lac_vec_t *) Res->Child)->pData = ((lac_vec_t *) Op1->Child)->pData;
        ((lac_vec_t *) Res->Child)->iRef = ((lac_vec_t *) Op2->Child)->iRef + ((lac_int_t *) Op1->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else {
        printf("\n[ Warning ] Syntax Error, trying to add two wrong types\n");
        g_env_reset();
        return;
    }

}

void proc_func_minus() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        Res = env_create_lac_int(0);
        ((lac_int_t *) Res->Child)->iValue = ((lac_int_t *) Op2->Child)->iValue - ((lac_int_t *) Op1->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else if (Op1->Type == LAC_INT && Op2->Type == LAC_VEC) {
        Res = env_create_lac_vec(NULL, 0, ((lac_vec_t *) Op2->Child)->iLength, ((lac_vec_t *) Op2->Child)->iSize, NULL);
        ((lac_vec_t *) Res->Child)->pData = ((lac_vec_t *) Op2->Child)->pData;
        ((lac_vec_t *) Res->Child)->iRef = ((lac_vec_t *) Op2->Child)->iRef - ((lac_int_t *) Op1->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else if (Op2->Type == LAC_INT && Op1->Type == LAC_VEC) {
        Res = env_create_lac_vec(NULL, 0, ((lac_vec_t *) Op1->Child)->iLength, ((lac_vec_t *) Op1->Child)->iSize, NULL);
        ((lac_vec_t *) Res->Child)->pData = ((lac_vec_t *) Op1->Child)->pData;
        ((lac_vec_t *) Res->Child)->iRef = ((lac_vec_t *) Op1->Child)->iRef - ((lac_int_t *) Op2->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else {
        printf("\n[ Warning ] Syntax Error, trying to compare two non-var types\n");
        g_env_reset();
        return;
    }
}

void proc_func_div() {
    lac_object_t *Op1 = stack_pop_data(&g_Env.StkData);
    lac_object_t *Op2 = stack_pop_data(&g_Env.StkData);
    if (Op1->Type == LAC_VAR) Op1 = ((lac_var_t *) Op1->Child)->Object;
    if (Op2->Type == LAC_VAR) Op2 = ((lac_var_t *) Op2->Child)->Object;

    lac_object_t *Res;
    if (Op1->Type == LAC_INT && Op2->Type == LAC_INT) {
        Res = env_create_lac_int(0);
        ((lac_int_t *) Res->Child)->iValue = ((lac_int_t *) Op2->Child)->iValue / ((lac_int_t *) Op1->Child)->iValue;
        stack_push_data(&g_Env.StkData, Res);
    } else {
        printf("\n[ Warning ] Syntax Error, trying to compare two non-var types\n");
        g_env_reset();
        return;
    }
}

void proc_func_lit() {
    int iAddr = stack_pop_return(&g_Env.StkReturn);
    stack_push_return(&g_Env.StkReturn, iAddr + 1);
    lac_object_t *pLACObjectTmp = g_Env.pLACVMTable->Objects[iAddr + 1];
    stack_push_data(&g_Env.StkData, pLACObjectTmp);

    if (pLACObjectTmp->Type == LAC_VAR) pLACObjectTmp = ((lac_var_t *) pLACObjectTmp->Child)->Object;

#ifdef DEBUG
    switch (pLACObjectTmp->Type) {
        case LAC_FUNC:
            printf("\n[ Debug ] Fonction (lit) executed, %d is read to StkData\n", ((lac_func_t *) pLACObjectTmp->Child)->iCFA);
            break;
        case LAC_INT:
            printf("\n[ Debug ] Fonction (lit) executed, %d is read to StkData\n", ((lac_int_t *) pLACObjectTmp->Child)->iValue);

        case LAC_VEC:
            printf("\n[ Debug ] Fonction (lit) executed, vector is read to StkData\n");
            break;
        case LAC_VAR:
            printf("\n[ Warning ] Syntax Error, trying to lit a var(var)\n");
            g_env_reset();
            return;
        case LAC_CLASS:
            printf("\n[ Debug ] Fonction (lit) executed, class %s is read to StkData\n", pLACObjectTmp->Name);
            break;
    }
#endif
}

void proc_func_fin() {
#ifdef DEBUG
    int iAddr = stack_pop_return(&g_Env.StkReturn);
    printf("\n[ Debug ] Function (fin) executed, %d is erased from StkReturn\n", iAddr);
#else
    stack_pop_return(&g_Env.StkReturn);
#endif
}

void proc_func_lac() {
    ;
}

void proc_func_calculate() {
    lac_object_t *Addr = stack_pop_data(&g_Env.StkData);
    if (Addr->Type == LAC_VAR) Addr = ((lac_var_t *) Addr->Child)->Object;

    if (Addr->Type == LAC_VEC) {
        int iLength = vmtable_strlen(Addr->Child);
        char *psCalcString = calloc((size_t) iLength + 1, sizeof(char));
        if (psCalcString == NULL) {
            printf("\n[ Error ] Memory Error\n");
        }
        for (int idx = 0; idx < iLength; idx++) {
            psCalcString[idx] = (char) ((lac_vec_t *) Addr->Child)->pData[idx];
        }
        int ans;
        calculate_exp_int(psCalcString, iLength, &ans);
        lac_object_t *Ans = env_create_lac_int(ans);
        stack_push_data(&g_Env.StkData, Ans);
        free(psCalcString);
    } else {
        printf("\n[ Warning ] Syntax Error, trying to calculate a non-vector\n");
        g_env_reset();
        return;
    }
}

void proc_func_if() {
    lac_object_t *Cond = stack_pop_data(&g_Env.StkData); // [?] Consumes the top of data stack
    if (Cond->Type == LAC_VAR) Cond = ((lac_var_t *) Cond->Child)->Object;

    int iAddr;

    if (Cond->Type != LAC_INT) {
        printf("\n[ Warning ] Syntax Error, trying to do condition on a non-var\n");
        g_env_reset();
        return;
    }
    if (((lac_int_t *) Cond->Child)->iValue > 0) {
        iAddr = stack_pop_return(&g_Env.StkReturn);
        stack_push_return(&g_Env.StkReturn, iAddr + 1);
#ifdef DEBUG
        printf("\n [ Debug ] Function If is taken\n");
#endif
    } else {
        iAddr = stack_pop_return(&g_Env.StkReturn);
        if (g_Env.pLACVMTable->Objects[iAddr + 1]->Type != LAC_INT) {
            printf("\n[ Warning ] Syntax Error, trying to do jump to a non-var destination\n");
            g_env_reset();
            return;
        }
        stack_push_return(&g_Env.StkReturn, ((lac_int_t *) (g_Env.pLACVMTable->Objects[iAddr + 1]->Child))->iValue);
#ifdef DEBUG
        printf("\n[ Debug ] Function If is not taken, jumps to %d from %d\n", ((lac_int_t *) (g_Env.pLACVMTable->Objects[iAddr + 1]->Child))->iValue, iAddr);
#endif
    }

}

void proc_func_else() {
    /* If else symbol is encountered, then the if branch must be taken */
    /* Jump to the end of conditonal branch */
#ifdef DEBUG
    printf("\n [ Debug ] Function Else\n");
#endif
    proc_func_jr();
}

void proc_func_then() {
    ;
}

void proc_func_recurse() {
    ;
}

void proc_func_at() {
    lac_object_t *Val = stack_pop_data(&g_Env.StkData);
    lac_object_t *pLACObjectTmp;
    switch (Val->Type) {
        case LAC_VEC:
            pLACObjectTmp = env_create_lac_int(((lac_vec_t *) Val->Child)->pData[((lac_vec_t *) Val->Child)->iRef]);
            stack_push_data(&g_Env.StkData, pLACObjectTmp);
            break;
        case LAC_VAR:
            pLACObjectTmp = ((lac_var_t *) Val->Child)->Object;
            stack_push_data(&g_Env.StkData, pLACObjectTmp);
            break;
        case LAC_FUNC:
            printf("\n[ Warning ] Syntax Error, trying to at a Func");
            break;
        case LAC_INT:
            stack_push_data(&g_Env.StkData, Val);
        case LAC_CLASS:
            printf("\n[ Warning ] Syntax Error, trying to at a Class");
            break;
    }
}

void proc_func_exclaim() {
    lac_object_t *Addr = stack_pop_data(&g_Env.StkData);
    lac_object_t *Val = stack_pop_data(&g_Env.StkData);

    if (Val->Type == LAC_VAR) Val = ((lac_var_t *) Val->Child)->Object;
    lac_object_t *pNewVal;

    switch (Val->Type) {
        case LAC_VAR:
            /* Not likely to happen */
            printf("\n[ Warning ] Syntax Error, trying to assign value from a var-var\n");
            g_env_reset();
            return;
        case LAC_INT:
            switch (Addr->Type) {
                case LAC_VAR:
                    pNewVal = env_create_lac_int(((lac_int_t *) Val->Child)->iValue);
                    env_set_lac_var(Addr, pNewVal);
                    break;
                case LAC_VEC:
                    ((lac_vec_t *) Addr->Child)->pData[((lac_vec_t *) Addr->Child)->iRef] = ((lac_int_t *) Val->Child)->iValue;
                    break;
                default:
                    printf("\n[ Warning ] Syntax Error, trying to assign value to a invariable object\n");
                    g_env_reset();
                    return;
            }
#ifdef DEBUG
            printf("\n[ Debug ] A value %d is set to %s\n", ((lac_int_t *) Val->Child)->iValue, Addr->Name);
            fflush(stdout);
#endif
            break;

        case LAC_FUNC:
            printf("\n[ Warning ] Syntax Error, trying to assign a func to var\n");
            g_env_reset();
            return;
        case LAC_VEC:
            switch (Addr->Type) {
                case LAC_VAR:
                    if (Val->iRefCnt == 0) {
                        env_set_lac_var(Addr, Val);
                    } else {
                        pNewVal = env_create_lac_vec(NULL, 0, ((lac_vec_t *) Val->Child)->iLength, sizeof(int), ((lac_vec_t *) Val->Child)->pData);
                        env_set_lac_var(Addr, pNewVal);
                    }

                    break;
                default:
                    printf("\n[ Warning ] Syntax Error, trying to assign value to a invariable object\n");
                    g_env_reset();
                    return;
            }
#ifdef DEBUG
            printf("\n[ Debug ] A vector is set to %s\n", Addr->Name);
            fflush(stdout);
#endif
        case LAC_CLASS:
            /* [!] */
            break;
    }
}

void proc_func_while() {
    /* [ while(JR) | x+4 | JR  | LOOP_END | ... | break(JR) | x+1 | ... |   loop   | WHILE_START |
     * [    x      | x+1 | x+2 |   x+3    | ... |           |     | ... | LOOP_END |     x-1     |*/
    proc_func_jr();
}

void proc_func_loop() {
#ifdef DEBUG
    printf("\n[ Debug ] Looping ...\n");
    fflush(stdout);
#endif
    proc_func_jr();
}

void proc_func_break() {
#ifdef DEBUG
    printf("\n[ Debug ] Break ...\n");
    fflush(stdout);
#endif
    proc_func_jr();

}

void proc_func_jr() {
    int iAddr = stack_pop_return(&g_Env.StkReturn);
    lac_object_t *pLACObjectTmp = g_Env.pLACVMTable->Objects[iAddr + 1];
    if (pLACObjectTmp->Type == LAC_VAR) pLACObjectTmp = ((lac_var_t *) pLACObjectTmp->Child)->Object;

    if (pLACObjectTmp->Type != LAC_INT) {
        printf("\n[ Warning ] Syntax Error, trying to do jump to a non-int destination\n");
        g_env_reset();
        return;
    }
    stack_push_return(&g_Env.StkReturn, ((lac_int_t *) (pLACObjectTmp->Child))->iValue);

#ifdef DEBUG
    printf("\n[ Debug ] Unconditional jump to %d from %d\n", ((lac_int_t *) (pLACObjectTmp->Child))->iValue, iAddr);
    fflush(stdout);
#endif
}

void g_env_init() {
    /* This function init the runtime environment */
    hash_symtable_init(&g_Env.SymTable);
    stack_init(&g_Env.StkData);
    stack_init(&g_Env.StkReturn);
    queue_init(&g_Env.Objects);
    vmtable_init(&g_Env.BasicFuncTable);
    g_Env.pQueRes = queue_create();
    queue_init(g_Env.pQueRes);
    g_Env.bInited = TRUE;
}

void g_env_reset() {
    /* This function clear the data stack and return stack */
    stack_clear(&g_Env.StkData);
    stack_clear(&g_Env.StkReturn);
    hash_symtable_revert(&g_Env.SymTable);
    queue_clear(g_Env.pQueRes);
    g_Env.bInited = TRUE;
}

void g_env_collect_garbage() {
    /* Free every object that has 0 reference */
    int iCnt = 0;
    lac_object_t *pLACObjectTmp;
    queue_node_t *pCursor = g_Env.Objects.pFront;
    queue_node_t *pCursorOld;
    while (pCursor != NULL) {
        pLACObjectTmp = *(lac_object_t **) pCursor->pData;
        pCursorOld = pCursor;
        queue_next(&pCursor);
        if (pLACObjectTmp->iRefCnt <= 0) {
            queue_del(&g_Env.Objects, pCursorOld, NULL, FALSE, FALSE    );
            /* Destroy the unused object */
            destroy_lac_obj(pLACObjectTmp);
            iCnt++;
        }
    }
#ifdef DEBUG
    printf("\n[ Info ] Garbage collection destroyed %d objects\n", iCnt);
    fflush(stdout);
#endif


    ;
}

void g_env_compile() {
    /* The basic functions of processor need to be compiled in advance */
    /* The two tables (symtable and vmtable) are assumed as inited */

    /* Add the basic functions to SymTable and VMTable*/
    if (g_Env.bInited != TRUE) g_env_init();
    lac_object_t *pBasicFuncTmp;

    for (int idx = 0; idx < INIT_PROC_FUNC_NUM; ++idx) {

        /* modify the vm table */
        pBasicFuncTmp = env_create_lac_func(g_proc_basic_func_name[idx], 0, g_Env.BasicFuncTable.iTail + 1, g_proc_basic_func_type[idx], g_proc_basic_func[idx]);
        vmtable_add(&g_Env.BasicFuncTable, pBasicFuncTmp);
        /* modify the symbol table */
        hash_symtable_add_obj(&g_Env.SymTable, g_proc_basic_func_name[idx], (int) strlen(g_proc_basic_func_name[idx]), pBasicFuncTmp);
    }

    /* Save the status of VMTable and StrTable */
    hash_symtable_checkout(&g_Env.SymTable);
}

