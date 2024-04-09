#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"

static const size_t MAX_LIST_LEN = 1000000;

static size_t  GetKeyIndex(hashtable_t* table, const char* key);

static size_t  GetListSize(node_t* root_cell);

extern "C" node_t* _FindKey(node_t* root_cell, const char* key);

// ===================================================

static inline int _StrCmp(const char* key1, const char* key2)
{
    assert(key1);
    assert(key2);

    int result = 0;

    asm inline
    (
        ".intel_syntax noprefix\n"
        "xor        %0, %0\n"                   // result = 0
        "vmovdqu    ymm0, YMMWORD PTR [%1]\n"   // place str1 in ymm0
        "vptest     ymm0, YMMWORD PTR [%2]\n"   // compare registers
        "setnc      %b0\n"                      // set bit if (CF == 0 && ZF == 0)
        "vzeroupper\n"
        ".att_syntax prefix\n"
        : "=&r" (result)
        : "r" (key1), "r" (key2)
        : "ymm0", "cc"
    );

    return result;
}

// --------------------------------------------------------------

node_t* NodeCtor()
{
    node_t* node = (node_t*) calloc(1, sizeof(node_t));

    node->next = node;
    node->prev = node;
    node->key  = nullptr;
    node->val  = 0;

    return node;
}

// --------------------------------------------------------------

void FillNode(node_t* node, const char* key, const int val, node_t* next, node_t* prev)
{
    assert(node);
    assert(key);

    char* key_copy = (char*) calloc(MAX_KEY_LEN, sizeof(char));
    assert(key_copy);

    memcpy(key_copy, key, MAX_KEY_LEN);

    node->key  = key_copy;
    node->next = next;
    node->prev = prev;
    node->val  = val;
}

// --------------------------------------------------------------

void NodeDtor(node_t* node)
{
    assert(node);

    free(node->key);

    free(node);
}

// --------------------------------------------------------------

hashtable_t* HashtableCtor(size_t size, hashfunc_t func)
{
    static const size_t prime_numbers[]   =
    {
        7, 19, 37, 61, 127, 271, 331, 397, 547, 631,
        919, 1657, 1801, 1951, 2269, 2437, 2791, 3169,
        3571, 4219, 4447, 5167, 5419, 6211, 7057, 7351, 8269,
        9241, 10267, 11719, 12097, 13267, 13669, 16651, 19441,
        19927, 22447, 23497, 24571, 25117, 26227, 27361, 33391, 35317,
        37633, 43201, 47629, 60493, 63949, 65713, 69313, 73009, 76801,
        84673, 106033, 108301, 112909, 115249
    };
    static const size_t prime_numbers_amt = sizeof(prime_numbers) / sizeof(*prime_numbers);

    size_t table_size = 0;
    for (size_t i = 0; i < prime_numbers_amt; i++)
    {
        if (size <= prime_numbers[i])
        {
            table_size = prime_numbers[i];
            break;
        }
    }

    if (table_size == 0)
    {
        printf("TOO BIG TABLE\n");
        return nullptr;
    }

    hashtable_t* table = (hashtable_t*) calloc(1, sizeof(hashtable_t));
    assert(table);

    node_t** cells = (node_t**) calloc(table_size, sizeof(node_t*));
    assert(cells);

    for (size_t i = 0; i < table_size; i++)
        cells[i] = NodeCtor();

    table->size = table_size;
    table->func = func;
    table->cells = cells;

    return table;
}

// --------------------------------------------------------------

void HashtableDtor(hashtable_t* table)
{
    assert(table);

    table->size = 0;
    table->func = nullptr;
    free(table->cells);
    free(table);
}

// --------------------------------------------------------------

static size_t GetKeyIndex(hashtable_t* table, const char* key)
{
    assert(table);
    assert(key);

    u_int32_t hash  = table->func(key);
    size_t    index = hash % table->size;

    return index;
}

// --------------------------------------------------------------

node_t* FindKeyInList(node_t* root_cell, const char* key)
{
    assert(root_cell);
    assert(key);

    node_t* cell = root_cell;

    size_t error_cnt = 0;

    while (cell->key != nullptr)
    {
        if (_StrCmp(cell->key, key) == 0)
            return cell;

        cell = cell->next;

        error_cnt++;

        if (error_cnt >= MAX_LIST_LEN)
        {
            perror("INVALID LIST FORMAT\n");
            return nullptr;
        }
    }

    return cell;
}

// --------------------------------------------------------------

void HashtableInsert(hashtable_t* table, const char* key, int val)
{
    assert(key);
    assert(table);

    size_t index = GetKeyIndex(table, key);
    node_t* root_cell = table->cells[index];

    node_t* key_node = _FindKey(root_cell, key);

    if (key_node->key != nullptr)
    {
        key_node->val = val;
        return;
    }

    assert(key_node);

    node_t* new_cell = (node_t*) calloc(1, sizeof(node_t));
    assert(new_cell);

    FillNode(new_cell, key, val, key_node, key_node->prev);
    (key_node->prev)->next = new_cell;
    key_node->prev         = new_cell;

    table->cells[index] = key_node->next;
}

// --------------------------------------------------------------

int HashtableGet(hashtable_t* table, const char* key)
{
    assert(key);
    assert(table);

    size_t index = GetKeyIndex(table, key);
    node_t* root_cell = table->cells[index];

    node_t* key_node = _FindKey(root_cell, key);

    if (key_node->key != nullptr)
        return key_node->val;

    return UNKNOWN_VAL;
}

// --------------------------------------------------------------

static size_t GetListSize(node_t* root_cell)
{
    assert(root_cell);

    size_t size = 0;

    node_t* cell = root_cell;

    while (cell->key != nullptr)
    {
        size++;

        if (size >= MAX_LIST_LEN)
        {
            printf("INVALID LIST FORMAT\n");
            return size;
        }

        cell = cell->next;
    }

    return size;
}

// --------------------------------------------------------------

void DumpHashtableSizes(hashtable_t* table, FILE* fp)
{
    assert(table);

    for (size_t i = 0; i < table->size; i++)
    {
        size_t list_size = GetListSize(table->cells[i]);

        fprintf(fp, "%u %u\n", i, list_size);
    }
}

