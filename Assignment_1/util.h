#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

void print_nums(const int *arr, const size_t n);
int *gen_ran_arr(const size_t size);

void mul10(uint32_t *h, uint32_t *l, const uint32_t a, const uint32_t b);
void add10(uint32_t *h, uint32_t *l, const uint32_t a, const uint32_t b);
void sub10(uint32_t *h, uint32_t *l, uint32_t a, const uint32_t b);

void print_uint_nums(const uint32_t *arr, const size_t n);
uint32_t *gen_uint_arr(const uint32_t size, const size_t base, const int seed_offset);

#endif
