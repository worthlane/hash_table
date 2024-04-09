#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "hashtable/hashtable.h"
#include "testing/testing.h"

static const char* OUTPUT_FILE = "assets/output/aboba_hash.txt";
static const char* INPUT_FILE  = "assets/input/370k_in.txt";

static const size_t TABLE_SIZE = 70000;

extern "C" uint64_t _GetTick();

// ================================================================

int main(const int argc, const char* argv[])
{
    char* words[WORDS_AMT] = {};

    hashtable_t* table = HashtableCtor(TABLE_SIZE, Fast_CRC32_Hash);

    InsertFileData(table, INPUT_FILE, words);

    uint64_t start = _GetTick();
    FindWordsInTable(table, words, WORDS_AMT);
    uint64_t end   = _GetTick();

    uint64_t dur = end - start;
    printf("%lu\n", dur);

    for (int i = 0; i < WORDS_AMT; i++)
        free(words[i]);

    HashtableDtor(table);

}

// ------------------------------------------------------
