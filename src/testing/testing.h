#ifndef __TESTING_H
#define __TESTING_H

#include "../hashtable/hashtable.h"

//#define SHOW_DEBUG

static const size_t WORDS_AMT  = 370105;
static const size_t STEP       = 20000;

static const size_t FIND_ATTEMPTS = 100;

void InsertFileData(hashtable_t* table, const char* file_name, char** words);

void FindWordsInTable(hashtable_t* table, char** words, const size_t word_amt);

#endif
