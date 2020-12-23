//
// Created by 厉宇桐 on 2020/11/19.
//
#include "../utils/calculate.h"

#define EXP_MAX_LEN 0x7F

int main(int argc, const char *argv[]) {
    char *sExp;
    int res;

    if (argc > 1) {
        sExp = (char *) argv[1];
    } else {
        sExp = (char *) malloc(sizeof(char) * (EXP_MAX_LEN + 1));
        if (sExp == NULL) {
            exit(ERR_MEM);
        }
        memset(sExp, 0, sizeof(char) * (EXP_MAX_LEN + 1));
        printf("Input expression: ");

        if (fgets(sExp, EXP_MAX_LEN, stdin) == NULL) {
            exit(ERR_IO);
        }
    }
    if (argc > 2) {
        if (strncmp(argv[2], "-v", 2) == 0 || strncmp(argv[2], "-V", 2) == 0) {
            /* verbose */
        }
    }

    int iNumBytes = (int) strlen(sExp);

//    for (int i = 0; i < 10000; i ++) {

    calculate_exp_int(sExp, iNumBytes, &res);
    printf("Result：%d\n", res);
//    }
    return 0;
}