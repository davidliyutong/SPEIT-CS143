//
// Created by 厉宇桐 on 2020/12/26.
//

#ifndef LAC_HASH_H
#define LAC_HASH_H

#ifdef USE_MD5
#include "md5.h"
#endif

#include <string.h>

int hash33(void *input, int iCnt, int iSize);

unsigned int str_to_hash(unsigned char *input);

#endif //LAC_HASH_H
