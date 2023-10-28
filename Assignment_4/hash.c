#include <string.h>
#include <stdint.h>
#include "hash.h"

#define Q            ((uint64_t) (18446744073709551359ull))    /* 2^64 - 257 is prime */
#define TWO64_MOD_Q  ((uint64_t) (257))                        /* 2^64 mod Q */
#define A            ((uint64_t) (16777890769592355103ull))    /* prime */
#define B            ((uint64_t) (14721169578037290713ull))    /* prime */

static inline uint64_t add_uint64_mod_q(uint64_t a, uint64_t b)
{
  __uint128_t t;
  uint64_t c, s;

  t = ((__uint128_t) a) + ((__uint128_t) b);
  s = (uint64_t) t;
  c = (uint64_t) (t >> 64);
  return (((s % Q) + (c * TWO64_MOD_Q)) % Q);
}

static inline uint64_t mul_uint64_mod_q(uint64_t a, uint64_t b)
{
  __uint128_t t, r;
  uint64_t h, l, s, c;

  t = ((__uint128_t) a) * ((__uint128_t) b);
  l = (uint64_t) t;
  h = (uint64_t) (t >> 64);
  r = (((__uint128_t) h) * ((__uint128_t) TWO64_MOD_Q)) + ((__uint128_t) l);
  s = (uint64_t) r;
  c = (uint64_t) (r >> 64);
  return (((s % Q) + (c * TWO64_MOD_Q)) % Q);
}

uint32_t hash_uint64(uint64_t a)
{
  uint64_t prod, sum;

  prod = mul_uint64_mod_q(a, A);
  sum = add_uint64_mod_q(prod, B);

  return (uint32_t) sum;
}

uint32_t hash_int64(int64_t a)
{
  int64_t aa;

  aa = a;

  return hash_uint64(*((uint64_t *) &aa));
}

uint32_t hash_uint32(uint32_t a)
{
  return hash_uint64((uint64_t) a);
}

uint32_t hash_int32(int32_t a)
{
  return hash_int64((int64_t) a);
}

uint32_t hash_uint16(uint16_t a)
{
  return hash_uint64((uint64_t) a);
}

uint32_t hash_int16(int16_t a)
{
  return hash_int64((int64_t) a);
}

uint32_t hash_uint8(uint8_t a)
{
  return hash_uint64((uint64_t) a);
}

uint32_t hash_int8(int8_t a)
{
  return hash_int64((int64_t) a);
}

uint32_t hash_double(double a)
{
  double aa;
  uint64_t t;

  aa = a;
  memcpy(&t, &aa, sizeof(t));

  return hash_uint64(t);
}

uint32_t hash_float(float a)
{
  return hash_double((double) a);
}

static inline uint32_t hash_mem_up_to_8(const void *ptr, size_t n)
{
  uint64_t t;

  if (n > ((size_t) 8)) return ((uint32_t) 0);
  
  t = (uint64_t) 0;
  if (n > ((size_t) 0)) {
    memcpy(&t, ptr, n);
  }

  return hash_uint64(t);
}

static inline uint32_t hash_combine(uint32_t a, uint32_t b)
{
  uint64_t t, h, l;

  h = (uint64_t) a;
  h <<= 32;
  l = (uint64_t) b;
  t = h | l;

  return hash_uint64(t);
}

uint32_t hash_mem(const void *ptr, size_t n)
{
  size_t w, r, i;
  uint32_t t, tt;

  if (n <= ((size_t) 8))
    return hash_mem_up_to_8(ptr, n);

  w = n >> 3;
  r = n - (n << 3);

  t = hash_mem_up_to_8(ptr, ((size_t) 8));
  w--;
  for (i = ((size_t) 0); i < w; i++) {
    tt = hash_mem_up_to_8(&(((char *) ptr)[(i + ((size_t) 1)) << 3]), ((size_t) 8));
    t = hash_combine(t, tt);
  }
  if (r > ((size_t) 0)) {
    tt = hash_mem_up_to_8(&(((char *) ptr)[(i + ((size_t) 1)) << 3]), r);
    t = hash_combine(t, tt);
  }

  return t;
}

uint32_t hash_str(const char *ptr)
{
  return hash_mem(ptr, strlen(ptr));
}

