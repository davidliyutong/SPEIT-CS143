//
// Created by 厉宇桐 on 2020/11/30.
//
#include <stdio.h>
#include <string.h>
#include "common/types.h"
#include "lex/analex.h"
#include "runtime/interpret.h"

int main(int argc, const char *argv[]) {
//    extern vmenv_t g_Env;
    g_env_init();
    g_env_compile();
    bool bOutputInfo = FALSE;
    int iReadCnt;
    compile_stat_t CompileStat = {FALSE, FALSE, FALSE};
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
            match_lac(psReadBuffer, psReadBuffer + iReadCnt, g_Env.pQueRes);

#ifdef DEBUG
            bOutputInfo = TRUE;
#endif

            if (bOutputInfo) {
                disp_vmtable(&g_Env.BasicFuncTable);
                disp_symtable(&g_Env.SymTable);
                disp_annalex_res(psReadBuffer, g_Env.pQueRes);
                visualize(psReadBuffer, g_Env.pQueRes);
                fflush(stdout);
            }
            interpret(g_Env.pQueRes, CompileStat);
            g_env_collect_garbage();
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
            while (fgets(sInputBuffer + 1, MAX_INPUT_LEN, stdin) != NULL) {
                iReadCnt = (int) strlen(sInputBuffer);
                match_lac(sInputBuffer, sInputBuffer + iReadCnt, g_Env.pQueRes);
                interpret(g_Env.pQueRes, CompileStat);
                g_env_collect_garbage();


                /* After interpret, clear queue and input Buffer */
                queue_clear(g_Env.pQueRes);
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