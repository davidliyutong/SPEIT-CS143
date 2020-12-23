#include <stdio.h>
#include "../common/queue.h"
#include "../lex/analex.h"
#include "../utils/debug.h"
#include "../utils/io.h"


int main(int argc, const char *argv[]) {
    /* This program highlights lac, so that your debug process could be easier */
    if (argc == 1) {
        printf("This program highlights lac, so that your debug process could be easier\nUsage: ${NAME_EXECUTABLE} ${NAME_LAC_FILE}\nExample: $ color factorielle.lac\nWhat will happen: A colored lac program will appear on the terminal\n\nNote: This program works only under linux terminal!\n");
    }
    char *psReadBuffer = NULL;
    FILE *pInputFile;
    assign_input_stream(argc, argv, &pInputFile); // redirect input to pInputFile pointer
    long ulReadCnt = input_to_buffer(pInputFile, &psReadBuffer);
    if (ulReadCnt <= 0) exit(ERR_IO);// read file

    lac_queue_t queRes;
    queue_init(&queRes);
    match_lac(psReadBuffer, psReadBuffer + ulReadCnt, &queRes);
    disp_annalex_res(psReadBuffer, &queRes);
    visualize(psReadBuffer, &queRes);
    fflush(stdout);
    queue_clear(&queRes);
    return 0;
}