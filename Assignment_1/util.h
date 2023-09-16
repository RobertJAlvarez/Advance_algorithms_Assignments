#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

void print_nums(int *arr, size_t n);
int *gen_ran_arr(size_t size);

void mul10(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b);
void add10(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b);
void print_uint_nums(uint32_t *arr, size_t n);
uint32_t *gen_uint_arr(uint32_t size, size_t base);

#endif
