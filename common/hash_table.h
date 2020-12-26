//
// Created by 厉宇桐 on 2020/12/26.
//

#ifndef LAC_HASH_TABLE_H
#define LAC_HASH_TABLE_H


#define  HASH_TABLE_LEN 8

#include "queue.h"
#include "macros.h"
#include "../utils/hash.h"

#define MAX_KEY_LEN MAX_LEXEME_LEN

typedef struct hash_table_t {
    int iNumEntry;
    lac_queue_t Data[HASH_TABLE_LEN]; /* should be automatically initialized by 0 */
    int CheckPoints[HASH_TABLE_LEN];
} hash_table_t;

typedef struct hash_table_entry {
    char Key[MAX_KEY_LEN + 1];
    void *pData;
} hash_table_entry;

typedef struct hash_table_query_res {
    int idx;
    queue_node_t *pNode;
} hash_table_query_res;

unsigned int hash_table_compute_hash(unsigned char *input);

hash_table_entry *hash_table_gen_entry(const char *sKey, void *pValue, int iSize);

int hash_table_reset(hash_table_t *pHashTable);

int hash_table_init(hash_table_t *pHashTable);

int hash_table_revert(hash_table_t *pHashTable);

int hash_table_checkout(hash_table_t *pHashTable);

int hash_table_entry_is_empty(hash_table_t *pHashTable, int iHashTableIdx);

hash_table_query_res hash_table_query(hash_table_t *pHashTable, char *sKey);

hash_table_query_res hash_table_add(hash_table_t *pHashTable, char *sKey, void *pValue, int iSize);

#endif