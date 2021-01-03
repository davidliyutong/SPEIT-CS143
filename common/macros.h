//
// Created by 厉宇桐 on 2020/11/18.
//
// All macros

#ifndef LAC_MACROS_H
#define LAC_MACROS_H

/* Error code */
#define ERR_NOT_IMPLEMENTED 80
#define ERR_IO 81
#define ERR_MEM 82
#define ERR_REGEX 83
#define ERR_DATA 84
#define ERR_SYNTAX 85
#define ERR_SEMANTIC 86

/* Global constraints */
#define MAX_MATCH_NUM 0xA
#define MAX_NUMBER_LEN 0x10
#define MAX_LEXEME_LEN 0x40
#define MAX_INPUT_LEN 256

#define INIT_VM_TABLE_LEN 0x400
#define INIT_SYM_TABLE_LEN 0x400

#define VERSION_INFO "\nThis is lac interactive mode Ver 3.5, by Yutong LI\n"
#define VERSION_INFO_DEBUG "\nThis is lac interactive mode Ver 3.5, by Yutong LI [DEBUG] \n"

#endif
