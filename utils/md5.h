//
// Created by 厉宇桐 on 2020/12/26.
//

#ifndef LAC_MD5_H
#define LAC_MD5_H

#ifdef USE_MD5
#include <memory.h>

typedef struct MD5_CTX {
    unsigned int Count[2];
    unsigned int State[4];
    unsigned char Buffer[64];
} MD5_CTX;


#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x)^(y)^(z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
#define FF(a, b, c, d, x, s, ac) \
          { \
          (a) += F(b,c,d) + (x) + (ac); \
          (a) = ROTATE_LEFT(a,s); \
          (a) += (b); \
          }
#define GG(a, b, c, d, x, s, ac) \
          { \
          (a) += G(b,c,d) + (x) + (ac); \
          (a) = ROTATE_LEFT(a,s); \
          (a) += (b); \
          }
#define HH(a, b, c, d, x, s, ac) \
          { \
          (a) += H(b,c,d) + (x) + (ac); \
          (a) = ROTATE_LEFT(a,s); \
          (a) += (b); \
          }
#define II(a, b, c, d, x, s, ac) \
          { \
          (a) += I(b,c,d) + (x) + (ac); \
          (a) = ROTATE_LEFT(a,s); \
          (a) += (b); \
          }

void MD5Init(MD5_CTX *context);

void MD5Update(MD5_CTX *context, const unsigned char *input, unsigned int inputlen);

void MD5Final(MD5_CTX *context, unsigned char digest[16]);

void MD5Transform(unsigned int state[4], const unsigned char block[64]);

void MD5Encode(unsigned char *output, const unsigned int *input, unsigned int len);

void MD5Decode(unsigned int *output, const unsigned char *input, unsigned int len);
#endif
#endif