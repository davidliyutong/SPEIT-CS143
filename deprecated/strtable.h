//
// Created by 厉宇桐 on 2020/12/1.
//

#ifndef LAC_STRTABLE_H
#define LAC_STRTABLE_H

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "../common/types.h"

#define INIT_STR_TABLE_LEN 0x400

typedef struct strtable_t {
    int iMaxLength; // The max length of the vm table
    int iTail; // The place of last char
    char *cStr;
} strtable_t;


/* We choosed a different way to handle string [?] */
/* The strings are stored in a seperated strtable,
 * only their begin address (integer) will be stored
 * in data stack
 */

int strtable_init(strtable_t *pStrTable);

int strtable_expand(strtable_t *pStrTable);

int strtable_add(strtable_t *pStrTable, const char *pStr, int iLength);

int strtable_clear(strtable_t *pStrTable);

#endif //LAC_STRTABLE_H
