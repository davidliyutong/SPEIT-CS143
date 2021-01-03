//
// Created by 厉宇桐 on 2020/11/18.
//
// Definition of custom types

#ifndef LAC_TYPES_H
#define LAC_TYPES_H

#include "macros.h"

typedef enum bool {
    TRUE = 1,
    FALSE = 0,
} bool;

typedef int status;

typedef void (*basic)(void);

#endif
