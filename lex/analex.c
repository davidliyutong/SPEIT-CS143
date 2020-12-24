//
// Created by 厉宇桐 on 2020/10/28.
//
// Code to perform lexical analyze


#include "analex.h"

inline int isInteger(const char *pcBeg, const char *pcFin, regex_t *pregInteger) {
    /* This function can tell if a string(pcBeg -> pcFin) is an integer ) */
    /* [?] Here the regex is used for all real numberss */

    int iStart;
    int iRet;

    regmatch_t pMatch[MAX_MATCH_NUM];
    int iEnd;
    iRet = regexec(pregInteger, pcBeg, 1, pMatch, 0);
    if (iRet == REG_NOMATCH) {
        return FALSE;
    } else {
        iStart = pMatch[0].rm_so;
        iEnd = pMatch[0].rm_eo;

        /* Match out of range, break */
        if (pcBeg + iEnd - 1 != pcFin || iStart != 0) {
            return FALSE;
        } else {
            return TRUE;
        }
    }
}

inline int match_id(const char *psReadBuffer, const char *pcIDBeg, const char *pcIDFin, regex_t *pregIdentifier,
                    regex_t *pregInteger,
                    lac_queue_t *pqueRes) {
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
        iRet = regexec(pregIdentifier, pcIDBeg, 1, pMatch, 0);
        if (iRet == REG_NOMATCH) { // No match
            break;
        } else {
            iIDStart = pMatch[0].rm_so;
            iIDEnd = pMatch[0].rm_eo;

            /* Match out of range, break */
            if (pcIDBeg + iIDEnd > pcIDFin) {
                break;
            }

            /* Calculate offset */
            iOffset = (int) (pcIDBeg - psReadBuffer);
            ans.iStart = iIDStart + iOffset;
            ans.iEnd = iIDEnd + iOffset;

            /* Identifier number */
            if (isInteger(psReadBuffer + ans.iStart, psReadBuffer + ans.iEnd, pregInteger) == TRUE) {
                ans.type = NUMBER;
            } else {
                ans.type = WORD;
            }

            /* Add a record */
            queue_push_back(pqueRes, (void *) &ans, sizeof(ans));
            pcIDBeg += iIDEnd;

        }
    }
    return TRUE;
}

inline int match_string(const char *psReadBuffer, const char *pcStringBeg, const char *pcStringFin, regex_t *regString,
                        regex_t *pregIdnetifier, regex_t *pregInteger, lac_queue_t *pqueRes) {
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
        iRet = regexec(regString, pcStringBeg, 2, pMatch, 0);
        if (iRet == REG_NOMATCH) {
            /* Last string is matched, try match id in the rest of characters */
            pcIDBeg = pcStringBeg;
            pcIDFin = (char *) pcStringFin;
            match_id(psReadBuffer, pcIDBeg, pcIDFin, pregIdnetifier, pregInteger, pqueRes);
            break;
        } else {
            iStringStart = pMatch[0].rm_so;
            iStringEnd = pMatch[0].rm_eo;
            /* The match is outside of boundary, try match id */
            if ((pcStringBeg + iStringEnd) > pcStringFin) {
                pcIDBeg = pcStringBeg;
                pcIDFin = (char *) pcStringFin;
                match_id(psReadBuffer, pcIDBeg, pcIDFin, pregIdnetifier, pregInteger, pqueRes);
                break;
            }
            /* Set limit so that it doesnt overlap with strings*/
            pcIDBeg = pcStringBeg;
            pcIDFin = pcStringBeg + iStringStart;

            iOffset = (int) (pcStringBeg - psReadBuffer);
            ans.iStart = iStringStart + iOffset + 3; // Remove "
            ans.iEnd = iStringEnd + iOffset - 1; // Remove "

            /* Match identifier */
            match_id(psReadBuffer, pcIDBeg, pcIDFin, pregIdnetifier, pregInteger, pqueRes);

            /* Add a record */
            queue_push_back(pqueRes, (void *) &ans, sizeof(ans));
            pcStringBeg += iStringEnd;
        }
    }
    return TRUE;
}

int match_lac(char *psReadBuffer, char *pcReadBufferFin, lac_queue_t *pqueRes) {
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
            match_string(psReadBuffer, pcStringBeg, pcStringFin, &regString, &regIdentifier, &regInteger, pqueRes);
            break;
        } else {
            /* There is a comment behind current position (pcCommentBeg) */
            /* Set limit to string match so that it will not overlap with comment */
            iCommentStart = pMatch[0].rm_so;
            iCommentEnd = pMatch[0].rm_eo;
            pcStringBeg = pcCommentBeg;
            pcStringFin = pcCommentBeg + iCommentStart;

            /* Match string in range pcStringBeg-pcStringFin*/
            match_string(psReadBuffer, pcStringBeg, pcStringFin, &regString, &regIdentifier, &regInteger, pqueRes);

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


