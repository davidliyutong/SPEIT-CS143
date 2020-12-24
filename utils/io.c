//
// Created by 厉宇桐 on 2020/11/19.
//
// Code that interact with disk IO

#include "io.h"

int input_to_buffer(FILE *pInputFile, char **ppsReadBuffer) {
    /* This function read from pInputFile, write characters to ppsReadBuffer */

    int iReadCnt;
    long lSize; // size of file
    if (pInputFile != NULL && !feof(pInputFile)) {

        // Get the size of file
        fseek(pInputFile, 0, SEEK_END);
        lSize = ftell(pInputFile);
        rewind(pInputFile);
        if (lSize <= 0) {
            printf("\n[ Error ] IO error\n");
            exit(ERR_IO);
        }

        // allocate memory for file
        *ppsReadBuffer = (char *) malloc(sizeof(char) * (lSize + 3));
        if (ppsReadBuffer == NULL) {
            printf("\n[ Error ] Memory error\n");
            exit(ERR_MEM);
        }

        // read file
        (*ppsReadBuffer)[0] = '\n'; /* Manually add a return character*/
        iReadCnt = (int) fread(*ppsReadBuffer + 1, 1, (size_t) lSize, pInputFile);
        if (iReadCnt != lSize) {
            printf("\n[ Error ] IO error\n");
            free(ppsReadBuffer);
            exit(ERR_IO);
        }

        (*ppsReadBuffer)[iReadCnt + 1] = '\n';
        // return the number of byte read
        fclose(pInputFile);
        return iReadCnt + 1;
    } else {
        printf("\n[ Error ] Memory error\n");
        exit(ERR_IO); // The file is not opened
    }
}

void assign_input_stream(int argc, const char *argv[], FILE **ppInputFile) {
    /* This function choose a input source (stdin or FILE) */

    if (argc < 2) {
        *ppInputFile = (FILE *) stdin; // use stdin as input
    } else {
        *ppInputFile = fopen(argv[1], "r"); // open a file
        if (*ppInputFile == NULL) {
            printf("\n[ Error ] Failed to open %s\n", argv[1]);
            exit(ERR_IO);
        }
    }
}