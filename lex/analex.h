//
// Created by 厉宇桐 on 2020/10/28.
//
/**@file  analex.h
* @brief    lexical analyze types, macros
* @details  
* @author      厉宇桐
* @date        2020/10/28
*/

#ifndef LAC_ANALEX_H
#define LAC_ANALEX_H

#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/types.h"
#include "../common/macros.h"
#include "../common/queue.h"

/** @brief Regex expression to identify comment 
 * @remark The string must begin with a return character
 */
#define REGEX_COMMENT       "[\n| ]\\( [^\\)\\\\]*\\)|[\n| ]\\\\[^\n]*"

/** @brief Regex expression to identify strings 
 * @remark The string must begin with a return character
*/
#define REGEX_STRING        "[\n| ]\" [^\"]*\""

/** @brief Regex expression to identify words 
*/
#define REGEX_IDENTIFIER    "[^ \n]+"

/** @brief Regex expression to numbers 
* @remark It can identify real numbers
*/
#define REGEX_INTEGER       "(\\-|\\+)?[0-9]+(\\.[0-9]+)?[ |\n]"

// v_2
// commentaire        : [\n| ]\( [^\)\\]*?\)|[\n| ]\\[^\n]*\n
// chaîne de caractère: [\n| ]" [^"]*?"
// identificateurs    : [^ \n]+
// entiers            : (\-|\+)?[0-9]+(\.[0-9]+)?[ |\n]

/** @enum e_lexeme_types 
 * @brief There are three types of lexem
 */
typedef enum e_lexeme_types {
    WORD, /// identifiers
    STRING, /// strings
    NUMBER, /// numbers
} e_lexeme_types;

/** @struct lexeme_t
 * @brief Data structure of a lexeme
 */
typedef struct lexeme_t {
    e_lexeme_types type;    /// Type of lexeme
    int iStart;             /// Offset of lexeme start from beginning of char string
    int iEnd;               /// Offset of lexeme end from beginning of char string
    int iLength;            /// Lexeme length
    char *pString;          /// Lexeme start, equals to pString + iStart
} lexeme_t;

/** @enum e_interpret_stat
 * @brief A independent function is used to match interpret mode keywords.
 */
typedef enum e_interpret_stat {
    INTERPRET_DEF,          /// :
    INTERPRET_ENDDEF,       /// ;
    INTERPRET_DEFER,        /// defer
    INTERPRET_LINK,         /// link
    INTERPRET_VAR,          /// variable
    INTERPRET_VEC,          /// vec
    INTERPRET_IMPORT,       /// import
    INTERPRET_CLASS,        /// class
    INTERPRET_EXIT,         /// exit
    INTERPRET_DEFAULT,      /// * (others)
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