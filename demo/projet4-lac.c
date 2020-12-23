//
// Created by 厉宇桐 on 2020/12/11.
//
// Projet3 demo

#include "../common/types.h"
#include "../lex/analex.h"
#include "../runtime/interpret.h"

int main() {
    extern vmenv_t g_Env;
    g_proc_env_init();
    g_proc_compile();
    disp_vmtable(&g_Env.VMTable);
    disp_symtable(&g_Env.SymTable);

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

    interpret(psReadBuffer, &queRes);

    printf("\n[ Info ] After the execution, the VM table and symble table are: \n");
    disp_vmtable(&g_Env.VMTable);
    disp_symtable(&g_Env.SymTable);

    return 0;

}