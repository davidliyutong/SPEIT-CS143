//
// Created by 厉宇桐 on 2020/12/1.
//
// Code for converting formats

#include "convert.h"

inline int lex_to_symbol(const char *psReadBuffer, lexeme_t Lex, u_symbol *pSymbol) {
    /* This function converts a lexem to a symbol */
    /* Src: Lex, Dst pSymbol */

    int iLength = Lex.iEnd - Lex.iStart;
    if (iLength >= MAX_WORD_LEN - 1) {
        printf("\n[ Warning ] Syntax error, lexeme %.*s too long at %d\n", Lex.iEnd - Lex.iStart,
               psReadBuffer + Lex.iStart, Lex.iStart);
        return ERR_SYNTAX;
    }

    memset(pSymbol, 0, sizeof(union u_symbol) * MAX_WORD_LEN);
    pSymbol[0].i = iLength;
    for (int i = 1; i <= pSymbol[0].i; ++i) pSymbol[i].c = psReadBuffer[Lex.iStart + i - 1];
    return TRUE;
}

inline int str_to_symbol(const char *pStr, int iLength, u_symbol *pSymbol) {
    /* This function converts a lexem to a symbol */
    /* Src: Lex, Dst pSymbol */

    if (iLength >= MAX_WORD_LEN - 1) {
        printf("\n [ Warning ] Syntax error, string %.*s too long\n", iLength, pStr);
        return ERR_SYNTAX;
    }
    memset(pSymbol, 0, sizeof(union u_symbol) * MAX_WORD_LEN);

    pSymbol[0].i = iLength;
    for (int i = 1; i <= iLength; ++i) pSymbol[i].c = pStr[i - 1];
    return TRUE;
}
