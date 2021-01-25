//
// Created by 厉宇桐 on 2020/12/26.
//
/**@file hash_table.h
 * @brief This file contains declaration related to a hash table
 * @details
 * @author 厉宇桐
 * @data 2020/12/25
 */
 
#ifndef LAC_HASH_TABLE_H
#define LAC_HASH_TABLE_H

/** @brief Length of hash table, 32 is a default value */
/** @todo The length of hash table should be flexible by definition:,
 * - For global symbol table, the length of hash table should be set relatively larger
 * - For local symbol table, it should be smaller
 * - The hash table should be able to automatically expand and rebalanced.
 */
#define  HASH_TABLE_LEN 32

#include "queue.h"
#include "macros.h"
#include "../utils/hash.h"

/** @brief Length of key, the same as max length of a lexeme */
#define MAX_KEY_LEN MAX_LEXEME_LEN

/** @struct hash_table_t 
 * The hash table structure.
 * @remark The table use linked-list to solve hash collision
*/
typedef struct hash_table_t {
    int iNumEntry;  /// Number of entries
    lac_queue_t Data[HASH_TABLE_LEN]; /// Hash table data are general lac queues@related lac_queue_t. @remark The should be initialized by 0 (zerofilled)*/
    int CheckPoints[HASH_TABLE_LEN]; /// Check points for each table row, marks the end of valid content.
} hash_table_t;

/** @struct hash_table_entry_t
 * The hash table structure.
 * @remark The table use linked-list to solve hash collision
*/
typedef struct hash_table_entry_t {
    char Key[MAX_KEY_LEN + 1]; /// Key is a char array
    void *pData; /// Pointer points to data
} hash_table_entry_t;

/** @struct hash_table_query_res_t
 * Query result of a hash table
*/
typedef struct hash_table_query_res_t {
    int idx;
    queue_node_t *pNode;
} hash_table_query_res_t;

unsigned int hash_table_compute_hash(unsigned char *input);

hash_table_entry_t *hash_table_gen_entry(const char *sKey, void *pValue, int iSize);

bool hash_table_reset(hash_table_t *pHashTable);

bool hash_table_init(hash_table_t *pHashTable);

bool hash_table_revert(hash_table_t *pHashTable);

bool hash_table_checkout(hash_table_t *pHashTable);

bool hash_table_entry_is_empty(hash_table_t *pHashTable, int iHashTableIdx);

hash_table_query_res_t hash_table_query(hash_table_t *pHashTable, char *sKey);

hash_table_query_res_t hash_table_add(hash_table_t *pHashTable, char *sKey, void *pValue, int iSize);

#endif