#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashtable.h"

#define SHOW_DEBUG

static const char* OUTPUT_FILE = "assets/output/aboba_hash.txt";
static const char* INPUT_FILE  = "assets/input/in.txt";

static const size_t TABLE_SIZE = 10000;

static const size_t STEP = 5000;

static void InsertFileData(hashtable_t* table, const char* file_name);

// ================================================================

int main(const int argc, const char* argv[])
{

    hashtable_t* table = HashtableCtor(TABLE_SIZE, ROL_Hash);

    InsertFileData(table, INPUT_FILE);

    FILE* fp = fopen(OUTPUT_FILE, "w");
    if (!fp)
    {
        printf("CAN NOT FIND FILE \"%s\"\n", OUTPUT_FILE);
        return 1;
    }

    DumpHashtableSizes(table, fp);

    fclose(fp);
}

// ------------------------------------------------------

static void InsertFileData(hashtable_t* table, const char* file_name)
{
    assert(table);
    assert(file_name);

    FILE* fp = fopen(file_name, "r");
    if (!fp)
    {
        printf("CAN NOT FIND FILE \"%s\"\n", file_name);
        return;
    }

    char word[MAX_KEY_LEN] = "";

    size_t cnt = 0;

    while (fscanf(fp, "%s", word) != EOF)
    {
        HashtableInsert(table, word, strnlen(word, MAX_KEY_LEN));
        cnt++;

        if (cnt % STEP == 0)
        {
            #ifdef SHOW_DEBUG
            printf("WORDS HANDLED: %lu\n", cnt);
            #endif
        }
    }

    fclose(fp);
}
