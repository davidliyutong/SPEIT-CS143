//
// Created by 厉宇桐 on 2020/11/30.
//
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
    int iReadCnt = input_to_buffer(pInputFile, &psReadBuffer);

    queue_t queRes;
    queue_init(&queRes);
    match_lac(psReadBuffer, psReadBuffer + iReadCnt, &queRes);
    disp_annalex_res(psReadBuffer, &queRes);
    fflush(stdout);

    interpret(psReadBuffer, &queRes);


    return 0;

}