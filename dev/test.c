#include <stdio.h>
#include <string.h>

#include "hash_table.h"
#include "hash_symtable.h"

int main(int argc, char *argv[]) {
    hash_table_t HashTable;
    hash_table_init(&HashTable);
    char *key1 = "dup";
    int value1 = 0;
    hash_table_query_res iRet;
    iRet = hash_table_add(&HashTable, key1, (void *) &value1, sizeof(value1));
    hash_table_checkout(&HashTable, iRet.idx);
    iRet = hash_table_add(&HashTable, key1, (void *) &value1, sizeof(value1));
    printf("%d", iRet.idx);

    return 0;
}