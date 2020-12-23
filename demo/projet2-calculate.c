//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet2 demo

#include "../utils/calculate.h"

#define EXP_MAX_LEN 0x7F

int main(int argc, const char *argv[]) {
    char *sExp;
    int res;

    if (argc > 1) {
        /* Use input parameter as expresion */
        sExp = (char *) argv[1];
    } else {
        sExp = (char *) calloc((EXP_MAX_LEN + 1), sizeof(char));
        if (sExp == NULL) exit(ERR_MEM);
        printf("Input expression: ");
        if (fgets(sExp, EXP_MAX_LEN, stdin) == NULL) exit(ERR_IO);
        printf("%s", sExp);
    }

    int iNumBytes = (int) strlen(sExp);
    calculate_exp_int(sExp, iNumBytes, &res);
    printf("Result：%d\n", res);
    return 0;
}