#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include "hash.h"

static const int UNKNOWN_VAL = -1337;

struct node_t
{
    char* key;
    int   val;

    node_t* next;
    node_t* prev;
};

node_t* NodeCtor();
void    NodeDtor(node_t* node);
void    FillNode(node_t* node, const char* key, const int val, node_t* next, node_t* prev);

struct hashtable_t
{
    node_t** cells;
    size_t   size;

    hashfunc_t func;
};

node_t*      FindKeyInList(node_t* root_cell, const char* key);
hashtable_t* HashtableCtor(size_t size, hashfunc_t func);
void         HashtableDtor(hashtable_t* table);
void         HashtableInsert(hashtable_t* table, const char* key, int val);
int          HashtableGet(hashtable_t* table, const char* key);

void         DumpHashtableSizes(hashtable_t* table, FILE* fp);

#endif
