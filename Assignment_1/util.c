#include <stdio.h>  // printf()
#include <time.h>   // time()
#include "util.h"

void print_nums(int *arr, size_t n)
{
  for (size_t i = ((size_t) 0); i < n; ++i)
    printf("%d, ", arr[i]);
  printf("\n");
}

int *gen_ran_arr(size_t size)
{
  int *nums = (int *) malloc(size*sizeof(int));

  if (nums == NULL) return NULL;

  srand(time(NULL));
  for (size_t i = ((size_t) 0); i < size; ++i)
    nums[i] = rand()%100;

  return nums;
}

void mul10(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b)
{
  *l =  a * b;
  *h = *l / ((uint32_t) 10);
  *l = *l % ((uint32_t) 10);
}

void add10(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b)
{
  *l =  a + b;
  *h = *l / ((uint32_t) 10);
  *l = *l % ((uint32_t) 10);
}

void print_uint_nums(uint32_t *arr, size_t n)
{
  int i = n-1;
  // Skip all 0s
  while (arr[i] == 0) --i;

  // Print numbers with a space in between
  while (i >= 0)
    printf("%d ", arr[i--]);
  printf("\n");
}

uint32_t *gen_uint_arr(uint32_t size, size_t base)
{
  uint32_t *nums = (uint32_t *) malloc(size*sizeof(uint32_t));

  if (nums == NULL) return NULL;

  srand(time(NULL));
  for (size_t i = ((size_t) 0); i < size; ++i)
    nums[i] = (uint32_t) rand()%base;

  return nums;
}

