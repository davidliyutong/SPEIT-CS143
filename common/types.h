//
// Created by 厉宇桐 on 2020/11/18.
//
// Definition of custom types
/**@file  types.h
* @brief    Common types
* @details  
* @author      厉宇桐
* @date        2020/11/21
*/
#ifndef LAC_TYPES_H
#define LAC_TYPES_H

#include "macros.h"

/** @enum Bool type */
typedef enum bool {
    TRUE = 1,
    FALSE = 0,
} bool;

/** @typedef Status code type */
typedef int status;

/** @typedef Basic function type */
typedef void (*basic)(void);

#endif
