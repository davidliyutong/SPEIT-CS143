//
// Created by 厉宇桐 on 2020/10/28.
//

#ifndef LAC_ANALEX_H
#define LAC_ANALEX_H

#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/types.h"
#include "../common/macros.h"
#include "../common/queue.h"

#define REGEX_COMMENT       "[\n| ]\\( [^\\)\\\\]*\\)|[\n| ]\\\\[^\n]*"
#define REGEX_STRING        "[\n| ]\" [^\"]*\""
#define REGEX_IDENTIFIER    "[^ \n]+"
#define REGEX_INTEGER       "(\\-|\\+)?[0-9]+(\\.[0-9]+)?[ |\n]"

// v_2
// commentaire        : [\n| ]\( [^\)\\]*?\)|[\n| ]\\[^\n]*\n
// chaîne de caractère: [\n| ]" [^"]*?"
// identificateurs    : [^ \n]+
// entiers            : (\-|\+)?[0-9]+(\.[0-9]+)?[ |\n]

typedef enum e_lexeme_types {
    WORD,
    STRING,
    NUMBER,
} e_lexeme_types;

typedef struct lexeme_t {
    e_lexeme_types type;
    int iStart;
    int iEnd;
    int iLength;
    char *pString;
} lexeme_t;

typedef enum e_interpret_stat {
    INTERPRET_DEF, // :
    INTERPRET_ENDDEF, // ;
    INTERPRET_DEFER,
    INTERPRET_LINK,
    INTERPRET_VAR,
    INTERPRET_VEC,
    INTERPRET_IMPORT,
    INTERPRET_CLASS,
    INTERPRET_EXIT,
    INTERPRET_DEFAULT,
} e_interpret_stat;

int isInteger(const char *pcBeg, const char *pcFin, regex_t *pRegInteger);

int match_id(const char *psReadBuffer, const char *pcIDBeg, const char *pcIDFin, regex_t *pRegIdentifier,
             regex_t *pRegInteger, lac_queue_t *pQueRes);

int match_string(const char *psReadBuffer, const char *pcStringBeg, const char *pcStringFin, regex_t *pRegString,
                 regex_t *pRegIdnetifier, regex_t *pRegInteger,
                 lac_queue_t *pQueRes);

int match_lac(char *psReadBuffer, char *pcReadBufferFin, lac_queue_t *pQueRes);

e_interpret_stat match_keyword(struct lexeme_t LexTmp);

#endif