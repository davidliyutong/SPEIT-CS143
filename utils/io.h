//
// Created by 厉宇桐 on 2020/11/19.
//

#ifndef LAC_IO_H
#define LAC_IO_H

#include <stdio.h>
#include <stdlib.h>
#include "../common/macros.h"
#include "../common/queue.h"
#include "../common/stack.h"
#include "../common/types.h"

int input_to_buffer(FILE *pInputFile, char **ppsReadBuffer);

void assign_input_stream(int argc, const char *argv[], FILE **ppInputFile);

#endif //LAC_IO_H
