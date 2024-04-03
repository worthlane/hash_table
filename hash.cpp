#include <assert.h>
#include <string.h>

#include "hash.h"

static inline u_int32_t Rol(u_int32_t num)
{
    return ((num << 1)) | (num >> 31);
};

static inline u_int32_t Ror(u_int32_t num)
{
    return ((num >> 1)) | (num << 31);
};

// ======================================================

u_int32_t ConstHash(const char* key)
{
    assert(key);

    return 1337;
}

// --------------------------------------

u_int32_t FirstLetterHash(const char* key)
{
    assert(key);

    return key[0];
}

// --------------------------------------

u_int32_t ASCII_SumHash(const char* key)
{
    assert(key);

    size_t len = strlen(key);

    u_int32_t sum = 0;

    for (size_t i = 0; i < len; i++)
        sum += key[i];

    return sum;
}

// --------------------------------------

u_int32_t StrLenHash(const char* key)
{
    assert(key);

    u_int32_t len = strnlen(key, MAX_KEY_LEN);

    return len;
}

// --------------------------------------

u_int32_t ROR_Hash(const char* key)
{
    assert(key);

    u_int32_t hash = 0;

    size_t size = strnlen(key, MAX_KEY_LEN);

    for (size_t i = 0; i < size; i++)
    {
        hash = Ror(hash) ^ key[i];
    }

    return hash;
}

// --------------------------------------

u_int32_t ROL_Hash(const char* key)
{
    assert(key);

    u_int32_t hash = 0;

    size_t size = strnlen(key, MAX_KEY_LEN);

    for (size_t i = 0; i < size; i++)
    {
        hash = Rol(hash) ^ key[i];
    }

    return hash;
}

// --------------------------------------

u_int32_t CRC32_Hash(const char* key)
{
    assert(key);

    uint32_t crc = -1;

    size_t size = strnlen(key, MAX_KEY_LEN);

    for (size_t i = 0; i < size; i++)
    {
        crc = crc ^ (key[i] << 24);
        for (int bit = 0; bit < 8; bit++)
        {
            if (crc & (1L << 31))
                crc = (crc << 1) ^ 0x04C11DB7;
            else                   
                crc = (crc << 1);
        }
    }
    return ~crc;
}
