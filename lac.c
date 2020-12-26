//
// Created by 厉宇桐 on 2020/11/30.
//
#include <stdio.h>
#include <string.h>
#include "common/types.h"
#include "common/macros.h"
#include "lex/analex.h"
#include "runtime/interpret.h"

int main(int argc, const char *argv[]) {
//    extern vmenv_t g_Env;
    g_proc_env_init();
    g_proc_compile();
    bool bOutputInfo = FALSE;
    lac_queue_t queRes;
    queue_init(&queRes);
    int iReadCnt;

    /* */
    switch (argc) {
        case 3: {
            if (strncmp(argv[2], "-V", 2) == 0 || strncmp(argv[2], "-v", 2) == 0) {
                bOutputInfo = TRUE;
            } else {
                bOutputInfo = FALSE;
            }
        }
        case 2: {
            char *psReadBuffer = NULL;
            FILE *pInputFile;
            pInputFile = fopen(argv[1], "r");
            iReadCnt = input_to_buffer(pInputFile, &psReadBuffer);
            match_lac(psReadBuffer, psReadBuffer + iReadCnt, &queRes);

#ifdef DEBUG
            bOutputInfo = TRUE;
#endif

            if (bOutputInfo) {
                disp_vmtable(&g_Env.VMTable);
                disp_symtable(&g_Env.SymTable);
                disp_annalex_res(psReadBuffer, &queRes);
                visualize(psReadBuffer, &queRes);
                fflush(stdout);
            }
            interpret(psReadBuffer, &queRes);
            free(psReadBuffer);
        }
        case 1: {
            /* Interpret mode */
#ifdef DEBUG
            printf(VERSION_INFO_DEBUG);
#else
            printf(VERSION_INFO);
#endif
            char sInputBuffer[MAX_INPUT_LEN + 1];
            memset(sInputBuffer, 0, sizeof(char) * MAX_INPUT_LEN);
            sInputBuffer[0] = '\n'; // Manually add a return character to the line start
            printf(">>>");
            while (fgets(sInputBuffer + 1, MAX_INPUT_LEN, (FILE *) stdin) != NULL) {
                iReadCnt = (int) strlen(sInputBuffer);
                match_lac(sInputBuffer, sInputBuffer + iReadCnt, &queRes);
                interpret(sInputBuffer, &queRes);

                /* After interpret, clear queue and input Buffer */
                queue_clear(&queRes);
                memset(sInputBuffer, 0, sizeof(char) * MAX_INPUT_LEN);
                sInputBuffer[0] = '\n'; // Manually add a return character to the line start
                printf("\n>>>");
            }
            break;
        }
        default:
            printf("\n[ Error ] Incorrect number of arguments\n");
            break;
            /* execute mode */
    }

    return 0;

}