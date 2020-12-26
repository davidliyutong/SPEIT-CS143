//
// Created by 厉宇桐 on 2020/12/26.
//

#ifndef LAC_HASH_H
#define LAC_HASH_H

#include "md5.h"

int hash33(void *input, int iCnt, int iSize);

unsigned int str_to_md5hash(unsigned char *input);

#endif //LAC_HASH_H
