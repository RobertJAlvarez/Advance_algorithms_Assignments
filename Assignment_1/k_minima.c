#include <stdlib.h> // free()
#include <stdio.h>
#include "util.h"

static void swap(int *a, int *b)
{
  if (*a == *b) return;

  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
}

static size_t partition(int *arr, size_t high)
{
  int pivot = arr[high];
  size_t i = 0;

  for (size_t j = ((size_t) 0); j < high; ++j)
    if (arr[j] < pivot) swap(&arr[i++], &arr[j]);

  swap(&arr[i], &arr[high]);

  return i;
}

void k_minima(int *arr, ssize_t high, size_t k)
{
  if (high <= 0) return;

  size_t pi = partition(arr, (size_t) high);
  k_minima(arr, pi-1, k);

  // Only sort sub array above pivot if k > pivot
  if (pi < k)
    k_minima(&arr[pi+1], high-(pi+1), k-(pi+1));
}

int main(void)
{
  int *nums;
  int SIZE = 10;
  size_t k = 5;

  if ((nums = gen_ran_arr(SIZE)) == NULL) return 1;

  printf("k = %zu\n", k);

  printf("Before: ");
  print_nums(nums, SIZE);

  k_minima(nums, ((ssize_t) SIZE-1), k);

  printf(" After: ");
  print_nums(nums, SIZE);

  free(nums);

  return 0;
}

