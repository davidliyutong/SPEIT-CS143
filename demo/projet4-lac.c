//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet3 demo

#include "../common/types.h"
#include "../lex/analex.h"
#include "../runtime/interpret.h"

int main() {
    g_env_init();
    g_env_compile();
    disp_vmtable(&g_Env.BasicFuncTable);
    disp_symtable(&g_Env.SymTable);
    compile_stat_t CompileStat = {FALSE, FALSE, FALSE};


    char *psReadBuffer = NULL;
    FILE *pInputFile;
    pInputFile = fopen("projet4.lac", "r");
    if (pInputFile == NULL) {
        printf("[ Warning ] You have to put the code in a file named after \"projet4.lac\" next to the program. For example:\n{CURRENT_DIR}\n   ├── projet4 (exe)\n   └── projet4.lac (lac code)\n");
        exit(ERR_IO);
    }
    int iReadCnt = input_to_buffer(pInputFile, &psReadBuffer);

    lac_queue_t queRes;
    queue_init(&queRes);
    match_lac(psReadBuffer, psReadBuffer + iReadCnt, &queRes);
    disp_annalex_res(psReadBuffer, &queRes);
    visualize(psReadBuffer, &queRes);
    fflush(stdout);

    interpret(&queRes, CompileStat);

    printf("\n[ Info ] After the execution, the symbol table is: \n");
    disp_symtable(&g_Env.SymTable);
    disp_objects(&g_Env.Objects);
    g_env_collect_garbage();
    disp_objects(&g_Env.Objects);

    return 0;

}