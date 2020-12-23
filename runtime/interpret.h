//
// Created by 厉宇桐 on 2020/11/26.
//

#ifndef LAC_INTERPRET_H
#define LAC_INTERPRET_H

#include <string.h>
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/types.h"
#include "../deprecated/strtable.h"
#include "../runtime/symtable.h"
#include "../runtime/vmtable.h"
#include "../utils/calculate.h"
#include "../utils/convert.h"

/*
    VM_DUP,   // dup        0
    VM_DROP,  // drop       1
    VM_SWAP,  // swap       2
    VM_DOT,   // .          3
    VM_COUNT, // count      4
    VM_TYPE,  // type       5
    VM_EQUAL, // =          6
    VM_MULT,  // *          7
    VM_PLUS,  // +          8
    VM_MINUS, // -          9
    VM_LIT,   // (lit)      10
    VM_FIN,   // (fin)      11
    VM_CALC,  // calculate  12
    VM_IF,    // if         13
    VM_ELSE,  // else       14
    VM_THEN,  // then       15

    dup (n – n n)       : duplique le sommet de la pile de données.
    drop (n – )         : supprime le sommet de la pile de données.
    swap (n1 n2 – n2 n1): échange les deux éléments sur le haut de la pile de données.
    . (e – )            : imprime l’entier qui est sur le sommet de la pile de données et le supprime.
    count (ad – ad’ e)  : prend l’adresse de chaîne en haut de la pile, et la remplace par l’adresse du début de la chaîne sans la longueur et met la longueur sur le sommet.
    type (ad e – )      : imprime e caractères dont les codes sont à partir de ad.
    = (e1 e2 – b)       : teste si e1 = e2 et empile un booléen.
    *                   :
    +                   :
    -                   :
    (lit)               :
    (fin)               :
    calculate           :
    if                  :
    else                :
    then                :

*/

int stack_push_vm(lac_stack_t *pStack, int iData);

int stack_pop_vm(lac_stack_t *pStack);

int stack_top_vm(lac_stack_t *pStack);

void proc_func_dup();

void proc_func_drop();

void proc_func_swap();

void proc_func_dot();

void proc_func_count();

void proc_func_type();

void proc_func_equal();

void proc_func_mult();

void proc_func_plus();

void proc_func_minus();

void proc_func_lit();

void proc_func_fin();

void proc_func_calculate();

void proc_func_if();

void proc_func_else();

void proc_func_then();

void proc_func_recurse();

void proc_func_at();

void proc_func_exclaim();

void proc_func_while();

void proc_func_loop();

void proc_func_break();

void g_proc_env_init();

void g_proc_env_reset();

//void g_proc_env_clear();

void g_proc_compile();

void exec_vm(int iCFA);

void compile_function(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_function(char *psReadBuffer, lac_queue_t *pqueRes);

void link_declaration(char *psReadBuffer, lac_queue_t *pqueRes);

void interpret(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_var(char *psReadBuffer, lac_queue_t *pqueRes);

void declare_vec(char *psReadBuffer, lac_queue_t *pqueRes);

#endif //LAC_INTERPRET_H