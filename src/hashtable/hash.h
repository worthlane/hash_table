#ifndef __HASH_H
#define __HASH_H

#include <stdio.h>
#include <stdint.h>

static const size_t MAX_KEY_LEN = 32;

typedef uint32_t (*hashfunc_t)(const char* key);

uint32_t ConstHash(const char* key);
uint32_t FirstLetterHash(const char* key);
uint32_t ASCII_SumHash(const char* key);
uint32_t StrLenHash(const char* key);
uint32_t ROR_Hash(const char* key);
uint32_t ROL_Hash(const char* key);
uint32_t CRC32_Hash(const char* key);

uint32_t Fast_CRC32_Hash(const char* key);

#endif
