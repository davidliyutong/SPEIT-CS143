//
// Created by 厉宇桐 on 2020/12/26.
//

#include "hash.h"

int hash33(void *input, int iCnt, int iSize) {
    int iLength = iCnt * iSize;
    int ans = 0;
    for (int i = 0; i < iLength; i++) {
        ans = ans * 33 + (int) *((char *) input + i);
    }
    return ans;
}


unsigned int str_to_md5hash(unsigned char *input) {
    /* Input is a string  */
    /* Output is a char[16] */
    unsigned char output[16];
    unsigned int ans;

    /* Do md5 first */
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, input, strlen((char *) input));
    MD5Final(&md5, output);

    /* Then do hash33 */
    ans = hash33(output, 16, sizeof(char));

    return ans;
}