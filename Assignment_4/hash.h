#ifndef HASH_H
#define HASH_H

#include <stdint.h>

uint32_t hash_uint64(uint64_t);
uint32_t hash_int64(int64_t);
uint32_t hash_uint32(uint32_t);
uint32_t hash_int32(int32_t);
uint32_t hash_uint16(uint16_t);
uint32_t hash_int16(int16_t);
uint32_t hash_uint8(uint8_t);
uint32_t hash_int8(int8_t);
uint32_t hash_double(double);
uint32_t hash_float(float);
uint32_t hash_mem(const void *, size_t);
uint32_t hash_str(const char *);

#endif
