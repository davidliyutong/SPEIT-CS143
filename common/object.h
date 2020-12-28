//
// Created by 厉宇桐 on 2020/12/28.
//

#ifndef LAC_OBJECT_H
#define LAC_OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "types.h"

typedef enum e_lac_object_type {
    LAC_FUNC,
    LAC_VAR,
    LAC_VEC
} e_lac_object_type;


typedef struct lac_object_t {
    /* Base class */
    int iRefCnt;
    e_lac_object_type Type;
    struct lac_object_t *Self;
    char Name[MAX_LEXEME_LEN];
    void *Child;
} lac_object_t;

typedef struct lac_func_t {
    lac_object_t Parent;
    struct lac_func_t *Self;
    int iCFA;
} lac_func_t;

typedef struct lac_var_t {
    lac_object_t Parent;
    struct lac_var_t *Self;
    int iValue;
} lac_var_t;

typedef struct lac_vec_t {
    lac_object_t Parent;
    struct lac_vec_t *Self;
    int iRef; /* reference of this vector */
    int iLength;
    int iSize;
    int *pData;
} lac_vec_t;

lac_object_t *create_lac_func(const char *FuncNameStr, int iCFA) {
    lac_object_t *NewObject = (lac_object_t *) calloc(1, sizeof(lac_object_t));
    if (NewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }
    NewObject->iRefCnt = 0;
    NewObject->Type = LAC_FUNC;
    NewObject->Self = NewObject;
    /* [!] */
    strncpy(NewObject->Name, FuncNameStr, MAX_LEXEME_LEN);

    lac_func_t *NewFunc = (lac_func_t *) calloc(1, sizeof(lac_func_t));
    if (NewFunc == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(NewObject);
        exit(ERR_MEM);
    }
    NewFunc->Parent.iRefCnt = NewObject->iRefCnt;
    NewFunc->Parent.Type = NewObject->Type;
    NewFunc->Parent.Self = NewObject->Self;
    NewFunc->iCFA = iCFA;

    return NewObject;
}

int destroy_lac_func(lac_object_t *LACFuncObject) {
    free(LACFuncObject->Child);
    free(LACFuncObject);
    return TRUE;
}

lac_object_t *create_lac_var(const char *FuncNameStr, int iValue) {
    lac_object_t *NewObject = (lac_object_t *) calloc(1, sizeof(lac_object_t));
    if (NewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }
    NewObject->iRefCnt = 0;
    NewObject->Type = LAC_FUNC;
    NewObject->Self = NewObject;
    /* [!] */
    strncpy(NewObject->Name, FuncNameStr, MAX_LEXEME_LEN);

    lac_var_t *NewFunc = (lac_var_t *) calloc(1, sizeof(lac_func_t));
    if (NewFunc == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(NewObject);
        exit(ERR_MEM);
    }
    NewFunc->Parent.iRefCnt = NewObject->iRefCnt;
    NewFunc->Parent.Type = NewObject->Type;
    NewFunc->Parent.Self = NewObject->Self;
    NewFunc->iValue = iValue;

    return NewObject;
}

int destroy_lac_var(lac_object_t *LACFuncObject) {
    free(LACFuncObject->Child);
    free(LACFuncObject);
    return TRUE;
}

lac_object_t *create_lac_vec(const char *FuncNameStr, int iLength, int iSize, void *pData) {
    lac_object_t *NewObject = (lac_object_t *) calloc(1, sizeof(lac_object_t));
    if (NewObject == NULL) {
        printf("\n[ Error ] Memory Error\n");
        exit(ERR_MEM);
    }
    NewObject->iRefCnt = 0;
    NewObject->Type = LAC_FUNC;
    NewObject->Self = NewObject;
    /* [!] */
    strncpy(NewObject->Name, FuncNameStr, MAX_LEXEME_LEN);

    lac_vec_t *NewFunc = (lac_vec_t *) calloc(1, sizeof(lac_func_t));
    if (NewFunc == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(NewObject);
        exit(ERR_MEM);
    }
    NewFunc->Parent.iRefCnt = NewObject->iRefCnt;
    NewFunc->Parent.Type = NewObject->Type;
    NewFunc->Parent.Self = NewObject->Self;
    NewFunc->iRef = 0;
    NewFunc->iLength = iLength;
    NewFunc->iSize = iSize;

    NewFunc->pData = calloc(iLength + 1, sizeof(int));
    if (NewFunc->pData == NULL) {
        printf("\n[ Error ] Memory Error\n");
        free(NewFunc);
        free(NewObject);
        exit(ERR_MEM);
    }

    /* The input is a empty vector, we only fill it with 0*/
    if (pData == NULL) {
        for (int idx = 0; idx < NewFunc->iLength; idx++) {
            /* Char stored as int */
            NewFunc->pData[idx] = 0;
        }
    } else {
        /* The input is not a empty vector */
        switch (NewFunc->iSize) {
            case 1:
                for (int idx = 0; idx < NewFunc->iLength; idx++) {
                    /* Char stored as int */
                    NewFunc->pData[idx] = (int) ((char *) pData)[idx];
                }
                /* Add zero */
                NewFunc->pData[iLength] = 0;
                break;
            case 4:
                for (int idx = 0; idx < NewFunc->iLength; idx++) {
                    NewFunc->pData[idx] = (int) ((char *) pData)[idx];
                }
                break;
            default:
                printf("\n[ Error ] The Size of each element must be 1(char) or 4(int) Byte\n");
                exit(ERR_MEM);
        }
    }

    return NewObject;
}

int destroy_lac_vec(lac_object_t *LACFuncObject) {
    free(((lac_vec_t *) LACFuncObject->Child)->pData);
    free(LACFuncObject->Child);
    free(LACFuncObject);
    return TRUE;
}

#endif //LAC_OBJECT_H
