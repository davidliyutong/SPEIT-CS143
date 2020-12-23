//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet1 demo

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

    /* redirect input to pInputFile pointer*/
    assign_input_stream(argc, argv, &pInputFile);

    long ulReadCnt = input_to_buffer(pInputFile, &psReadBuffer);
    if (ulReadCnt <= 0) exit(ERR_IO);// Failed to read any bytes

    /* Queue where the lexemes will be stored */
    lac_queue_t queRes;
    queue_init(&queRes);

    /* Perform Analyze lexical */
    match_lac(psReadBuffer, psReadBuffer + ulReadCnt, &queRes);

    /* Visualization of results */
    disp_annalex_res(psReadBuffer, &queRes);
    visualize(psReadBuffer, &queRes);
    fflush(stdout);

    /* Free memory */
    queue_clear(&queRes);
    return 0;
}