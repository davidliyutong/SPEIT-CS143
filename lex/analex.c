//
// Created by 厉宇桐 on 2020/10/28.
//
// Code to perform lexical analyze


#include "analex.h"

inline int isInteger(const char *pcBeg, const char *pcFin, regex_t *pRegInteger) {
    /* This function can tell if a string(pcBeg -> pcFin) is an integer ) */
    /* [?] Here the regex is used for all real numbers */

    int iStart;
    int iRet;

    regmatch_t pMatch[MAX_MATCH_NUM];
    int iEnd;
    iRet = regexec(pRegInteger, pcBeg, 1, pMatch, 0);
    if (iRet == REG_NOMATCH) {
        return FALSE;
    } else {
        iStart = (int) pMatch[0].rm_so;
        iEnd = (int) pMatch[0].rm_eo;

        /* Match out of range, break */
        if (pcBeg + iEnd - 1 != pcFin || iStart != 0) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

inline int match_id(const char *psReadBuffer, const char *pcIDBeg, const char *pcIDFin, regex_t *pRegIdentifier,
                    regex_t *pRegInteger,
                    lac_queue_t *pQueRes) {
    /* This function is used to match identifiers */
    /* It will also check if the identifier is a number */

    int iRet;
    int iOffset;
    regmatch_t pMatch[MAX_MATCH_NUM];
    int iIDStart;
    int iIDEnd;

    lexeme_t ans;
    ans.type = WORD;

    while (TRUE) {
        iRet = regexec(pRegIdentifier, pcIDBeg, 1, pMatch, 0);
        if (iRet == REG_NOMATCH) { // No match
            break;
        } else {
            iIDStart = (int) pMatch[0].rm_so;
            iIDEnd = (int) pMatch[0].rm_eo;

            /* Match out of range, break */
            if (pcIDBeg + iIDEnd > pcIDFin) {
                break;
            }

            /* Calculate offset */
            iOffset = (int) (pcIDBeg - psReadBuffer);
            ans.iStart = iIDStart + iOffset;
            ans.iEnd = iIDEnd + iOffset;
            ans.iLength = ans.iEnd - ans.iStart;
            ans.pString = (char *) psReadBuffer + ans.iStart;

            /* Identifier number */
            if (isInteger(psReadBuffer + ans.iStart, psReadBuffer + ans.iEnd, pRegInteger) == TRUE) {
                ans.type = NUMBER;
            } else {
                ans.type = WORD;
            }

            /* Add a record */
            queue_push_back(pQueRes, (void *) &ans, sizeof(ans));
            pcIDBeg += iIDEnd;

        }
    }
    return TRUE;
}

inline int match_string(const char *psReadBuffer, const char *pcStringBeg, const char *pcStringFin, regex_t *pRegString,
                        regex_t *pRegIdnetifier, regex_t *pRegInteger, lac_queue_t *pQueRes) {
    /* This function matches strings from pcStringBeg to pcStringFin*/
    /* It also match the identifiers from pcStringBeg to the begin of first string match */
    /* Then it loops until all the strings are matched */

    int iRet;
    int iOffset;
    regmatch_t pMatch[MAX_MATCH_NUM];
    int iStringStart;
    int iStringEnd;
    const char *pcIDBeg = NULL;
    const char *pcIDFin = NULL;

    lexeme_t ans;
    ans.type = STRING;

    while (TRUE) {
        iRet = regexec(pRegString, pcStringBeg, 2, pMatch, 0);
        if (iRet == REG_NOMATCH) {
            /* Last string is matched, try match id in the rest of characters */
            pcIDBeg = pcStringBeg;
            pcIDFin = (char *) pcStringFin;
            match_id(psReadBuffer, pcIDBeg, pcIDFin, pRegIdnetifier, pRegInteger, pQueRes);
            break;
        } else {
            iStringStart = (int) pMatch[0].rm_so;
            iStringEnd = (int) pMatch[0].rm_eo;
            /* The match is outside of boundary, try match id */
            if ((pcStringBeg + iStringEnd) > pcStringFin) {
                pcIDBeg = pcStringBeg;
                pcIDFin = (char *) pcStringFin;
                match_id(psReadBuffer, pcIDBeg, pcIDFin, pRegIdnetifier, pRegInteger, pQueRes);
                break;
            }
            /* Set limit so that it doesnt overlap with strings*/
            pcIDBeg = pcStringBeg;
            pcIDFin = pcStringBeg + iStringStart;

            iOffset = (int) (pcStringBeg - psReadBuffer);
            ans.iStart = iStringStart + iOffset + 3; // Remove "
            ans.iEnd = iStringEnd + iOffset - 1; // Remove "
            ans.iLength = ans.iEnd - ans.iStart;
            ans.pString = (char *) psReadBuffer + ans.iStart;

            /* Match identifier */
            match_id(psReadBuffer, pcIDBeg, pcIDFin, pRegIdnetifier, pRegInteger, pQueRes);

            /* Add a record */
            queue_push_back(pQueRes, (void *) &ans, sizeof(ans));
            pcStringBeg += iStringEnd;
        }
    }
    return TRUE;
}

int match_lac(char *psReadBuffer, char *pcReadBufferFin, lac_queue_t *pQueRes) {
    /* This function matches comment from psReadBuffer to pcReadBufferFin*/
    /* It also match the strings from psReadBuffer to the begin of first comment match */
    /* Then it loops until all the comments are matched */


    int iRet = 0;
    // Definition of 3 regex parser
    regex_t regComment;
    regex_t regString;
    regex_t regIdentifier;
    regex_t regInteger;
    iRet += regcomp(&regComment, REGEX_COMMENT, REG_EXTENDED);
    iRet += regcomp(&regString, REGEX_STRING, REG_EXTENDED);
    iRet += regcomp(&regIdentifier, REGEX_IDENTIFIER, REG_EXTENDED);
    iRet += regcomp(&regInteger, REGEX_INTEGER, REG_EXTENDED);
    if (iRet > 0) exit(ERR_REGEX); // the compile has failed, let OS to handle

    regmatch_t pMatch[MAX_MATCH_NUM]; // Match results
    int iCommentStart;
    int iCommentEnd;
    char *pcCommentBeg = psReadBuffer;
    char *pcStringBeg = NULL;
    char *pcStringFin = NULL;

    while (TRUE) {
        /* Match comments */
        iRet = regexec(&regComment, pcCommentBeg, 1, pMatch, 0);
        if (iRet == REG_NOMATCH) { // The last comment is extracted, no more comments
            pcStringBeg = pcCommentBeg;
            pcStringFin = pcReadBufferFin;
            /* Match string from the rest of characters*/
            match_string(psReadBuffer, pcStringBeg, pcStringFin, &regString, &regIdentifier, &regInteger, pQueRes);
            break;
        } else {
            /* There is a comment behind current position (pcCommentBeg) */
            /* Set limit to string match so that it will not overlap with comment */
            iCommentStart = (int) pMatch[0].rm_so;
            iCommentEnd = (int) pMatch[0].rm_eo;
            pcStringBeg = pcCommentBeg;
            pcStringFin = pcCommentBeg + iCommentStart;

            /* Match string in range pcStringBeg-pcStringFin*/
            match_string(psReadBuffer, pcStringBeg, pcStringFin, &regString, &regIdentifier, &regInteger, pQueRes);

            /* Move on to the next comment */
            pcCommentBeg += iCommentEnd;
        }

    }
    /* Free regex expressions */
    regfree(&regComment);
    regfree(&regString);
    regfree(&regIdentifier);
    regfree(&regInteger);

    return TRUE;
}

inline e_interpret_stat match_keyword(lexeme_t LexTmp) {

    if (strncmp(LexTmp.pString, ":", 1) == 0 && (LexTmp.iLength) == 1) {
        return INTERPRET_DEF;
    }

    if (strncmp(LexTmp.pString, ";", 1) == 0 && (LexTmp.iLength) == 1) {
        return INTERPRET_ENDDEF;
    }

    /* case it is a defer */
    if (strncmp(LexTmp.pString, "defer", 5) == 0 && (LexTmp.iLength) == 5) {
        return INTERPRET_DEFER;
    }

    /* case it is a ' */
    if (strncmp(LexTmp.pString, "'", 1) == 0 && (LexTmp.iLength) == 1) {
        return INTERPRET_LINK;
    }

    /* case it is a variable */
    if (strncmp(LexTmp.pString, "variable", 8) == 0 && (LexTmp.iLength) == 8) {
        return INTERPRET_VAR;
    }

    /* case it is a vector */
    if (strncmp(LexTmp.pString, "vec", 3) == 0 && (LexTmp.iLength) == 3) {
        return INTERPRET_VEC;
    }

    if (strncmp(LexTmp.pString, "exit", 4) == 0 && (LexTmp.iLength) == 4) {
        return INTERPRET_EXIT;
    }

    /* case it is an import */
    if (strncmp(LexTmp.pString, "import", 6) == 0 && (LexTmp.iLength) == 6) {
        return INTERPRET_IMPORT;
    }

    /* case it is an class */
    if (strncmp(LexTmp.pString, "class", 5) == 0 && (LexTmp.iLength) == 5) {
        return INTERPRET_CLASS;
    }

    return INTERPRET_DEFAULT;
}



