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
                                                  VM_BASIC_FUNC_SYM, // div
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_BASIC_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM, // recurse
                                                  VM_CTRL_FUNC_SYM, // @
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM,
                                                  VM_CTRL_FUNC_SYM
};


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
    int iAddr = stack_top_vm(&g_Env.StkData); // address of the string in the strtable
    int iLength = vmtable_strlen(&g_Env.VMTable, iAddr);
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

void proc_func_gt() {
    /* Greater than */
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    if (iOp1 < iOp2) {
        stack_push_vm(&g_Env.StkData, 1);
    } else {
        stack_push_vm(&g_Env.StkData, 0);
    }
}

void proc_func_lt() {
    /* Less than */
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    if (iOp1 > iOp2) {
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

void proc_func_div() {
    int iOp1 = stack_pop_vm(&g_Env.StkData);
    int iOp2 = stack_pop_vm(&g_Env.StkData);
    if (iOp1 != 0) {
        stack_push_vm(&g_Env.StkData, iOp2 / iOp1);
    } else {
        printf("\n[ Warning ] Divided by 0, The answer is 0\n");
        stack_push_vm(&g_Env.StkData, 0);
    };
}

void proc_func_lit() {
    int iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, iAddr + 1);
    stack_push_vm(&g_Env.StkData, g_Env.VMTable.OpCodes[iAddr + 1]);
#ifdef DEBUG
    printf("\n[ Debug ] Fonction (lit) executed, %d is read to StkData\n", g_Env.VMTable.OpCodes[iAddr + 1]);
#endif
}

void proc_func_fin() {
#ifdef DEBUG
    int iAddr = stack_pop_vm(&g_Env.StkReturn);
    printf("\n[ Debug ] Fonction (fin) executed, %d is erased from StkReturn\n", iAddr);
#else
    stack_pop_vm(&g_Env.StkReturn);
#endif
}

void proc_func_calculate() {
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int iLength = vmtable_strlen(&g_Env.VMTable, iAddr);
    char *psCalcString = calloc((size_t) iLength + 1, sizeof(char));
    for (int idx = 0; idx < iLength; idx++) {
        psCalcString[idx] = (char) g_Env.VMTable.OpCodes[iAddr + idx];
    }

    int ans;
    calculate_exp_int(psCalcString, iLength, &ans);

    stack_push_vm(&g_Env.StkData, ans);
    free(psCalcString);
}

void proc_func_if() {
    int iCond = stack_pop_vm(&g_Env.StkData); // [?] Consumes the top of data stack
    int iAddr;
    if (iCond > 0) {
        iAddr = stack_pop_vm(&g_Env.StkReturn);
        stack_push_vm(&g_Env.StkReturn, iAddr + 1);
#ifdef DEBUG
        printf("\n [ Debug ] Function If is taken\n");
#endif
    } else {
        iAddr = stack_pop_vm(&g_Env.StkReturn);
        stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);
#ifdef DEBUG
        printf("\n [ Debug ] Function If is not taken, jumps to %d from %d\n", g_Env.VMTable.OpCodes[iAddr + 1], iAddr);
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
    /* read variable */
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int ans;

    if (g_Env.VMTable.OpCodeTypes[iAddr] == OP_CODE_DATA) {
        ans = g_Env.VMTable.OpCodes[iAddr];
        stack_push_vm(&g_Env.StkData, ans);
#ifdef DEBUG
        printf("\n[ Debug ] A value %d is pushed into data stack\n", ans);
        fflush(stdout);
#endif
    } else {
        /* This place of memory is not data */
        printf("\n[ Warning ] Illegal lac memory access");
        g_proc_env_reset();
    }
}

void proc_func_exclaim() {
    int iAddr = stack_pop_vm(&g_Env.StkData);
    int iVal = stack_pop_vm(&g_Env.StkData);
    if (g_Env.VMTable.OpCodeTypes[iAddr] == OP_CODE_DATA) {
        /* This place of memory is writable */
        g_Env.VMTable.OpCodes[iAddr] = iVal;
#ifdef DEBUG
        printf("\n[ Debug ] A value %d is set to VMTable[%d]\n", iVal, iAddr);
        fflush(stdout);
#endif
    } else {
        /* This place of memory is not writable */
        printf("\n[ Warning ] Illegal lac memory access");
        g_proc_env_reset();
    }
}

void proc_func_while() {
    ;
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
    int iAddr = stack_pop_vm(&g_Env.StkReturn);
    stack_push_vm(&g_Env.StkReturn, g_Env.VMTable.OpCodes[iAddr + 1]);
#ifdef DEBUG
    printf("\n[ Debug ] Unconditional jump to %d from %d\n", g_Env.VMTable.OpCodes[iAddr + 1], iAddr);
    fflush(stdout);
#endif
}

void g_proc_env_init() {
    /* This function init the runtime environment */
    vmtable_init(&g_Env.VMTable);
    symtable_init(&g_Env.SymTable);
    stack_init(&g_Env.StkData);
    stack_init(&g_Env.StkReturn);
    g_Env.bInited = TRUE;
}

void g_proc_env_reset() {
    /* This function clear the data stack and return stack */
    stack_clear(&g_Env.StkData);
    stack_clear(&g_Env.StkReturn);
    symtable_revert(&g_Env.SymTable);
    vmtable_revert(&g_Env.VMTable);
    g_Env.bInited = TRUE;
}


void g_proc_compile() {
    /* The basic functions of processor need to be compiled in advance */
    /* The two tables (symtable and vmtable) are assumed as inited */

    /* Add the basic functions to SymTable and VMTable*/
    if (g_Env.bInited != TRUE) g_proc_env_init();

    for (int idx = 0; idx < INIT_PROC_FUNC_NUM; ++idx) {
        /* modify the symbol table */
        symtable_add(&g_Env.SymTable, g_proc_basic_func_name[idx], (int) strlen(g_proc_basic_func_name[idx]));
        /* iLength -2 is the current  symbol's CFA (NOT VALID) */

        /* modify the vm table */
        vmtable_add(&g_Env.VMTable, g_proc_basic_func_type[idx], OP_CODE_INST);
        vmtable_add(&g_Env.VMTable, idx, OP_CODE_INST);

        /* modify the symbol table CFA */
        symtable_set_cfa_by_name(&g_Env.SymTable, g_proc_basic_func_name[idx], (int) strlen(g_proc_basic_func_name[idx]), g_Env.VMTable.iTail - 1);
//        g_Env.SymTable.Symbols[g_Env.SymTable.iLength - 2].i = g_Env.VMTable.iTail - 1;

    }
    g_Env.bCompiled = TRUE;

    /* Save the status of VMTable and StrTable */
    symtable_checkout(&g_Env.SymTable);
    vmtable_checkout(&g_Env.VMTable);
}

