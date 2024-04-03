#ifndef __HASH_H
#define __HASH_H

static const size_t MAX_KEY_LEN = 1000;

typedef u_int32_t (*hashfunc_t)(const char* key);

u_int32_t ConstHash(const char* key);
u_int32_t FirstLetterHash(const char* key);
u_int32_t ASCII_SumHash(const char* key);
u_int32_t StrLenHash(const char* key);
u_int32_t ROR_Hash(const char* key);
u_int32_t ROL_Hash(const char* key);
u_int32_t CRC32_Hash(const char* key);

#endif
