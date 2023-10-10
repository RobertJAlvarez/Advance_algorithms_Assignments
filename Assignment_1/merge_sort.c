#include <stdio.h>  // printf()
#include <stdlib.h> // qsort(), malloc(), free()
#include <string.h> // memcpy()
#include "util.h"

// Function to be use in qsort() to compare merge sort result
int cmpfunc(const void * a, const void * b)
{
   return ( *((int *) a) - *((int *) b) );
}

static int eq_arr(const int *a, const int *b, const size_t n)
{
  for (size_t i = 0; i < n; ++i)
    if (a[i] != b[i]) return 0;

  return 1;
}

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
  int *arr, *copy_arr;
  int n_pass;
  size_t size;
  const int N_TESTS = 10;

  for (size = ((size_t) 1); size < ((size_t) 50); size += ((size_t) 5)) {
    n_pass = 0;
    for (int i = 0; i < N_TESTS; ++i) {
      if ((arr = gen_ran_arr(size)) == NULL) return 1;

      if ((copy_arr = (int *) malloc(size*sizeof(int)))== NULL) {
        free(arr);
        return 1;
      }

      memcpy(copy_arr, arr, size*sizeof(int));

      if (merge_sort(arr, size)) {
        printf("Error while executing merge sort:(\n");
        return 1;
      }

      qsort(copy_arr, size, sizeof(int), cmpfunc);

      n_pass += eq_arr(arr, copy_arr, size);

      free(arr);
      free(copy_arr);
    }

    printf("%d/%d pass for array of size %zu\n", n_pass, N_TESTS, size);
  }

  return 0;
}

