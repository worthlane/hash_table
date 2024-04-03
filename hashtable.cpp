#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "hashtable.h"

static const size_t MAX_LIST_LEN = 1000000;

static node_t* FindKey(node_t* root_cell, const char* key);
static size_t  GetKeyIndex(hashtable_t* table, const char* key);

static size_t  GetListSize(node_t* root_cell);

// ===================================================

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
    hashtable_t* table = (hashtable_t*) calloc(1, sizeof(hashtable_t));
    assert(table);

    node_t** cells = (node_t**) calloc(size, sizeof(node_t*));
    assert(cells);

    for (size_t i = 0; i < size; i++)
        cells[i] = NodeCtor();

    table->size = size;
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

static node_t* FindKey(node_t* root_cell, const char* key)
{
    assert(root_cell);
    assert(key);

    node_t* cell = root_cell;

    size_t error_cnt = 0;

    while (cell->key != nullptr)
    {
        if (strncmp(cell->key, key, MAX_KEY_LEN) == 0)
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

    node_t* key_node = FindKey(root_cell, key);

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

void HashtableErase(hashtable_t* table, const char* key)
{
    assert(key);
    assert(table);

    /*size_t index = GetKeyIndex(table, key);
    node_t* root_cell = table->cells[index];

    node_t* key_node = FindKey(root_cell, key);

    if (key_node->key == nullptr)
        return;

    */
}

// --------------------------------------------------------------

int HashtableGet(hashtable_t* table, const char* key)
{
    assert(key);
    assert(table);

    size_t index = GetKeyIndex(table, key);
    node_t* root_cell = table->cells[index];

    node_t* key_node = FindKey(root_cell, key);

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
