//
// Created by 厉宇桐 on 2020/11/18.
//
// All macros

#ifndef LAC_MACROS_H
#define LAC_MACROS_H

#define VM_BASIC_FUNC_SYM (-1)
#define VM_LAC_FUNC_SYM (-2)
#define VM_CTRL_FUNC_SYM (-3) // for if else then and other function non-utilisable in execution

#define VM_CFA_ERROR (-1)

#define VM_CFA_LIT 26
#define VM_CFA_FIN 28
#define VM_CFA_IF 32
#define VM_CFA_ELSE 34
#define VM_CFA_THEN 36
#define VM_CFA_RECURSE 38
//#define VM_CFA_AT 40
//#define VM_CFA_EXCLAIM 42
#define VM_CFA_WHILE 44
#define VM_CFA_LOOP 46
#define VM_CFA_BREAK 48
#define VM_CFA_JR 50

#define ERR_NOT_IMPLEMENTED 80
#define ERR_IO 81
#define ERR_MEM 82
#define ERR_REGEX 83
#define ERR_DATA 84
#define ERR_SYNTAX 85
#define ERR_SEMANTIC 86
#define ERR_SYMBOL_OVERFLOW (-2)
#define INFO_SYMBOL_NOT_FOUND (-1)

#define MAX_MATCH_NUM 0xA
#define MAX_NUMBER_LEN 0x10
#define MAX_LEXEME_LEN 0x40
#define MAX_INPUT_LEN 256

#define INIT_VM_TABLE_LEN 0x400
#define INIT_SYM_TABLE_LEN 0x400

#define INIT_PROC_FUNC_NUM 26

#define VERSION_INFO "\nThis is lac interactive mode Ver 2.5, by Yutong LI\n"
#define VERSION_INFO_DEBUG "\nThis is lac interactive mode Ver 2.5, by Yutong LI [DEBUG] \n"
#define COLOR_NUMBER 34
#define COLOR_WORD 33
#define COLOR_STRING 32
#endif
