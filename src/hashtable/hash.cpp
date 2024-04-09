#include <assert.h>
#include <string.h>
#include <immintrin.h>

#include "hash.h"

static inline uint32_t Rol(uint32_t num)
{
    return ((num << 1)) | (num >> 31);
}

static inline uint32_t Ror(uint32_t num)
{
    return ((num >> 1)) | (num << 31);
}

// ======================================================

uint32_t ConstHash(const char* key)
{
    assert(key);

    return 1337;
}

// --------------------------------------

uint32_t FirstLetterHash(const char* key)
{
    assert(key);

    return key[0];
}

// --------------------------------------

uint32_t ASCII_SumHash(const char* key)
{
    assert(key);

    size_t len = strlen(key);

    uint32_t sum = 0;

    for (size_t i = 0; i < len; i++)
        sum += key[i];

    return sum;
}

// --------------------------------------

uint32_t StrLenHash(const char* key)
{
    assert(key);

    uint32_t len = strnlen(key, MAX_KEY_LEN);

    return len;
}

// --------------------------------------

uint32_t ROR_Hash(const char* key)
{
    assert(key);

    uint32_t hash = 0;

    size_t size = strnlen(key, MAX_KEY_LEN);

    for (size_t i = 0; i < size; i++)
    {
        hash = Ror(hash) ^ key[i];
    }

    return hash;
}

// --------------------------------------

uint32_t ROL_Hash(const char* key)
{
    assert(key);

    uint32_t hash = 0;

    size_t size = strnlen(key, MAX_KEY_LEN);

    for (size_t i = 0; i < size; i++)
    {
        hash = Rol(hash) ^ key[i];
    }

    return hash;
}

// --------------------------------------

uint32_t CRC32_Hash(const char* key)
{
    assert(key);

    uint32_t crc = -1;

    int i = 0;

    while (key[i] != 0)
    {
        crc = crc ^ (key[i] << 24);
        for (int bit = 0; bit < 8; bit++)
        {
            if (crc & (1L << 31))
                crc = (crc << 1) ^ 0x04C11DB7;
            else
                crc = (crc << 1);
        }

        i++;
    }
    return ~crc;
}
// --------------------------------------

uint32_t Fast_CRC32_Hash(const char* key)
{
    assert(key);

    uint32_t hash = 0;

    hash = _mm_crc32_u32(hash, *((uint32_t*) key));
    hash = _mm_crc32_u32(hash, *((uint32_t*) key + 1));
    hash = _mm_crc32_u32(hash, *((uint32_t*) key + 2));
    hash = _mm_crc32_u32(hash, *((uint32_t*) key + 3));

    return hash;
}
