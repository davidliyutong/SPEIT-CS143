//
// Created by 厉宇桐 on 2020/12/23.
//

#ifndef LAC_PROC_H
#define LAC_PROC_H

#include <stdio.h>
#include <stdlib.h>
#include "../common/macros.h"
#include "../utils/calculate.h"
#include "env.h"
#include "symtable.h"
#include "vmtable.h"

extern void (*g_proc_basic_func[INIT_PROC_FUNC_NUM])();

extern char g_proc_basic_func_name[INIT_PROC_FUNC_NUM][10];

extern int g_proc_basic_func_type[INIT_PROC_FUNC_NUM];

void proc_func_dup();

void proc_func_drop();

void proc_func_swap();

void proc_func_dot();

void proc_func_count();

void proc_func_type();

void proc_func_equal();

void proc_func_gt();

void proc_func_lt();

void proc_func_mult();

void proc_func_plus();

void proc_func_minus();

void proc_func_div();

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

void proc_func_jr();

void g_proc_env_init();

void g_proc_env_reset();

//void g_proc_env_clear();

void g_proc_compile();

#endif //LAC_PROC_H
