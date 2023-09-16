#include <stdio.h>  // printf()
#include <string.h> // memcpy()
#include "util.h"

static int merge(int *arr, size_t l_size, size_t r_size)
{
  int *l_arr = arr;
  int *r_arr = &arr[l_size];
  int *temp;

  size_t l_i = 0;
  size_t r_i = 0;
  size_t t_i = 0;

  if ((temp = (int *) malloc(l_size+r_size+1)) == NULL) return 1;

  while (l_i < l_size && r_i < r_size)
    temp[t_i++] = (l_arr[l_i] < r_arr[r_i] ? l_arr[l_i++] : r_arr[r_i++]);

  while (l_i < l_size) temp[t_i++] = l_arr[l_i++];

  while (r_i < r_size) temp[t_i++] = r_arr[r_i++];

  memcpy(arr, temp, (l_size+r_size)*sizeof(int));
  free(temp);

  return 0;
}

int merge_sort(int *arr, size_t n)
{
  if (n <= 1) return 0;
  size_t mid = n/2;

  if (merge_sort(arr, mid)) return 1;
  if (merge_sort(&arr[mid], n-mid)) return 1;
  return merge(arr, mid, n-mid);
}

int main(void)
{
  int *arr;
  int SIZE = 20;

  if ((arr = gen_ran_arr(SIZE)) == NULL) return 1;

  printf("Unsorted numbers: ");
  print_nums(arr, SIZE);

  if (merge_sort(arr, SIZE)) {
    printf("Error while executing merge sort:(\n");
    return 1;
  }

  printf("  Sorted numbers: ");
  print_nums(arr, SIZE);

  return 0;
}

