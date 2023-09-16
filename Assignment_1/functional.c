#include <limits.h>
#include <stdio.h>
#include "util.h"

int add(int a, int b)
{
  return a + b;
}

int multiply(int a, int b)
{
  return a * b;
}

int max_val(int a, int b)
{
  return a > b ? a : b;
}

int min_val(int a, int b)
{
  return a < b ? a : b;
}

int fold(int *arr, size_t n, int seed, int (*fun_ptr)(int, int))
{
  if (n == 0) return seed;
  return (*fun_ptr)(*arr, fold(&arr[1], n-1, seed, fun_ptr));
}

int main(void)
{
  // fun_ptr_arr is an array of function pointers
  int *arr;
  int SIZE = 5;

  int (*fun_ptr_arr[])(int, int) = {add, multiply, min_val, max_val};
  int seeds[] = {0, 1, INT_MAX, INT_MIN};

  if ((arr = gen_ran_arr(SIZE)) == NULL) return 1;

  printf("Numbers = ");
  print_nums(arr, SIZE);

  printf("Addition answer: %d\n", fold(arr, SIZE, seeds[0], fun_ptr_arr[0]));
  printf("Multiplication answer: %d\n", fold(arr, SIZE, seeds[1], fun_ptr_arr[1]));
  printf("Min value: %d\n", fold(arr, SIZE, seeds[2], fun_ptr_arr[2]));
  printf("Max value: %d\n", fold(arr, SIZE, seeds[3], fun_ptr_arr[3]));

  free(arr);

  return 0;
}

