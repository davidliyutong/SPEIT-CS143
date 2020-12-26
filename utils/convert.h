//
// Created by 厉宇桐 on 2020/12/1.
//

#ifndef LAC_CONVERT_H
#define LAC_CONVERT_H

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../lex/analex.h"
#include "../runtime/symtable.h"
#include "../runtime/interpret.h"

int lex_to_symbol(const char *psReadBuffer, lexeme_t Lex, u_symbol *pSymbol);

#endif //LAC_CONVERT_H
