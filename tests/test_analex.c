//
// Created by 厉宇桐 on 2020/11/19.
//
#include <stdio.h>
#include "../common/queue.h"
#include "../lex/analex.h"
#include "../utils/debug.h"
#include "../utils/io.h"

int main(int argc, const char *argv[]) {
    char *psReadBuffer = NULL;
    FILE *pInputFile;

    /* Usage: ${PATH_TO_EXE}/${EXE} ${FILENAME}
    *  Usage: cat ${FILENAME} | ${PATH_TO_EXE}/${EXE}
    */
    assign_input_stream(argc, argv, &pInputFile); // redirect input to pInputFile pointer

//    int iRet = 0; // return value
    long ulReadCnt = input_to_buffer(pInputFile, &psReadBuffer);
    if (ulReadCnt <= 0) exit(ERR_IO);// read file

    queue_t queRes;
    queue_init(&queRes);
    /* Test memory leakage */
    for (int i = 0; i < 10; ++i) {
        match_lac(psReadBuffer, psReadBuffer + ulReadCnt, &queRes);
        disp_annalex_res(psReadBuffer, &queRes);
        fflush(stdout);
        queue_clear(&queRes);
    }
    return 0;
}