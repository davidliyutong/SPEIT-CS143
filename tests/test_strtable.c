//
// Created by 厉宇桐 on 2020/12/1.
//

#include <stdio.h>

int main() {
    strtable_t StrTable;
    strtable_init(&StrTable);
    strtable_add(&StrTable, "hahaha", 6);
    strtable_add(&StrTable, "ppp", 3);

    printf("%s", StrTable.cStr);
    printf("%s", StrTable.cStr + 7);
    return 0;
}