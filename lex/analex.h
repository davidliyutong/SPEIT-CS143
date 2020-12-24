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

int isInteger(const char *pcBeg, const char *pcFin, regex_t *pregInteger);

int match_id(const char *psReadBuffer, const char *pcIDBeg, const char *pcIDFin, regex_t *pregIdentifier,
             regex_t *pregInteger, lac_queue_t *pqueRes);

int match_string(const char *psReadBuffer, const char *pcStringBeg, const char *pcStringFin, regex_t *pregString,
                 regex_t *pregIdnetifier, regex_t *pregInteger,
                 lac_queue_t *pqueRes);

int match_lac(char *psReadBuffer, char *pcReadBufferFin, lac_queue_t *pqueRes);

#endif