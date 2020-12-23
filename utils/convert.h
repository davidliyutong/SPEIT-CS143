//
// Created by 厉宇桐 on 2020/12/1.
//

#ifndef LAC_CONVERT_H
#define LAC_CONVERT_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "../common/types.h"

int lex_to_symbol(const char *psReadBuffer, lexeme_t Lex, u_symbol *pSymbol);

int str_to_symbol(const char *pStr, int iLength, u_symbol *pSymbol);

#endif //LAC_CONVERT_H
