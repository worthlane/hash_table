#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "testing.h"

// -----------------------------------------------------------------------------

void InsertFileData(hashtable_t* table, const char* file_name, char** words)
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
        HashtableInsert(table, word, cnt);

        char* word_copy = (char*) calloc(MAX_KEY_LEN, sizeof(char));
        assert(word_copy);

        memcpy(word_copy, word, MAX_KEY_LEN);
        words[cnt++] = word_copy;

        if (cnt % STEP == 0)
        {
            #ifdef SHOW_DEBUG
            printf("WORDS HANDLED: %lu\n", cnt);
            #endif
        }
    }

    fclose(fp);
}

// ----------------------------------------------------------------------------

void FindWordsInTable(hashtable_t* table, char** words, const size_t word_amt)
{
    assert(table);
    assert(words);

    for (size_t attempt = 0; attempt < FIND_ATTEMPTS; attempt++)
    {
        for (size_t i = 0; i < word_amt; i++)
        {
            int val = HashtableGet(table, words[i]);
            assert(val == (int) i);
        }
    }
}
