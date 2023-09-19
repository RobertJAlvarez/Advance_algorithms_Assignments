#include <stdio.h>
#include <string.h>
#include "util.h"

/* START: Naive multiplication */
static void prop_op(
  uint32_t *c, uint32_t h,
  void (*op)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  if (h == ((uint32_t) 0)) return;
  op(&h, c, *c, h);
  prop_op(&c[1], h, op);
}

void multiply_schoolbook(
  uint32_t *c, const uint32_t *a, const uint32_t *b, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t h, l;
  size_t i, j;

  memset(c, 0, 2*n*sizeof(uint32_t));

  for (i = ((uint32_t) 0); i < n; ++i) {
    for (j = ((uint32_t) 0); j < n; ++j) {
      mul(&h, &l, a[i], b[j]);
      // Add low value to c[i+j] and propagate carry up if needed
      prop_op(&c[i+j], l, add);
      // Add high value to c[i+j+1] and propagate carry up if needed
      prop_op(&c[i+j+1], h, add);
    }
  }

  return;
}
/* END: Naive multiplication */

/* START: Karatsuba Algorithm */
static void array_op(
  uint32_t *c, const uint32_t *z, const uint32_t n,
  void (*op)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  for (uint32_t i = ((uint32_t) 0); i < n; ++i)
    prop_op(&c[i], z[i], op);
}

static uint32_t calc_n(const uint32_t *x, const uint32_t *y, uint32_t n)
{
  if (n == ((uint32_t) 0)) return 0;

  do {
    if ((x[n-1] != 0) || (y[n-1] != 0)) break;
    --n;
  } while (n > 0);

  return n;
}

static uint32_t *helper_multiply(
  const uint32_t *x, const uint32_t *y, size_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*sub)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *xy;
  uint32_t *a, *d, *e;
  uint32_t *new_x, *new_y;

  uint32_t l, h;
  size_t half_n;
  size_t new_n;

  if (n == 0) {
    fprintf(stderr, "ERROR: helper_multiply: size of arrays is 0.\n");
    return NULL;
  }

  xy = (uint32_t *) malloc(2*n*sizeof(uint32_t));
  memset(xy, 0, 2*n*sizeof(uint32_t));

  if (n == 1) {
    mul(&h, &l, x[0], y[0]);
    prop_op(xy, l, add);
    prop_op(&xy[1], h, add);
    return xy;
  }

  half_n = n/2 + n%2;

  /* a = X_H * Y_H */
  if ((a = helper_multiply(&x[half_n], &y[half_n], n-half_n, mul, add, sub)) == NULL) {
    free(xy);
    return NULL;
  }

  /* d = X_L * Y_L */
  if ((d = helper_multiply(x, y, half_n, mul, add, sub)) == NULL) {
    free(xy);
    free(a);
    return NULL;
  }

  /* e = (X_L + X_H)*(Y_L + Y_H) - a - d */
  new_n = half_n + 1;

  // new_x = X_L + X_H
  new_x = (uint32_t *) malloc(new_n*sizeof(uint32_t));
  if (new_x == NULL) {
    free(xy);
    free(a);
    free(d);
    return NULL;
  }
  memset(new_x, 0, new_n*sizeof(uint32_t));
  array_op(new_x, x, half_n, add);
  array_op(new_x, &x[half_n], n-half_n, add);

  // new_y = Y_L + Y_H
  new_y = (uint32_t *) malloc(new_n*sizeof(uint32_t));
  if (new_y == NULL) {
    free(xy);
    free(a);
    free(d);
    free(new_x);
    return NULL;
  }
  memset(new_y, 0, new_n*sizeof(uint32_t));
  array_op(new_y, y, half_n, add);
  array_op(new_y, &y[half_n], n-half_n, add);

  new_n = calc_n(new_x, new_y, new_n);
  e = helper_multiply(new_x, new_y, new_n, mul, add, sub);

  if (e == NULL) {
    free(xy);
    free(a);
    free(d);
    return NULL;
  }

  // e = new_x*new_y - a - d
  array_op(e, a, 2*(n-half_n), sub);
  array_op(e, d, 2*half_n, sub);

  /*
  printf("x = ");
  print_uint_nums(x,n);
  printf("y = ");
  print_uint_nums(y,n);
  printf("a = ");
  print_uint_nums(a,2*(n-half_n));
  printf("d = ");
  print_uint_nums(d,2*half_n);
  printf("e = ");
  print_uint_nums(e,2*new_n);

  printf("d starts at 0 and use %zu digits\n", 2*half_n);
  printf("e starts at %zu and use %zu digits\n", half_n, 2*new_n);
  printf("a starts at %zu and use %zu digits\n", n, 2*(n-half_n));
  */

  // xy = a*b^n + e*b^(n/2) + d
  memcpy(xy, d, (2*half_n)*sizeof(uint32_t));
  array_op(&xy[half_n], e, 2*new_n, add);
  array_op(&xy[2*half_n], a, 2*(n-half_n), add);

  free(a);
  free(d);
  free(e);
  free(new_x);
  free(new_y);

  return xy;
}

void multiply_faster(
  uint32_t *c, const uint32_t *a, const uint32_t *b, size_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*sub)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *ans;

  memset(c, 0, 2*n*sizeof(uint32_t));

  n = calc_n(a, b, n);

  if ((ans = helper_multiply(a, b, n, mul, add, sub)) == NULL) return;

  memcpy(c, ans, 2*n*sizeof(uint32_t));

  free(ans);

  return;
}
/* END: Karatsuba Algorithm */

int check_mul(const uint32_t *a, const uint32_t *b, const size_t n)
{
  uint32_t *school_ans;
  uint32_t *fast_ans;
  int same_ans;

  if ((school_ans = (uint32_t *) malloc(2*n*sizeof(uint32_t))) == NULL) return 0;
  if ((fast_ans = (uint32_t *) malloc(2*n*sizeof(uint32_t))) == NULL) {
    free(school_ans);
    return 0;
  };

  memset(school_ans, 0, 2*n*sizeof(uint32_t));
  memset(fast_ans, 0, 2*n*sizeof(uint32_t));

  multiply_schoolbook(school_ans, a, b, n, mul10, add10);
  multiply_faster(fast_ans, a, b, n, mul10, add10, sub10);

  // Compare answers
  same_ans = 1;
  if (comp10(school_ans, fast_ans, 2*n) != 0) {
    printf("a = ");
    print_uint_nums(a,n);
    printf("b = ");
    print_uint_nums(b,n);

    printf("Schoolbook answer: ");
    print_uint_nums(school_ans,2*n);

    printf("Fast answer: ");
    print_uint_nums(fast_ans,2*n);

    same_ans = 0;
  }

  free(school_ans);
  free(fast_ans);

  return same_ans;
}

void test_odd(void)
{
  // 870 * 200 = 174,000
  uint32_t a[] = {0,7,8};
  uint32_t b[] = {0,0,2};
  size_t n = 3;

  check_mul(a, b, n);
}

void test_even(void)
{
  // 1,234 * 4,321 = 5,332,114
  uint32_t a[] = {4,3,2,1};
  uint32_t b[] = {1,2,3,4};
  size_t n = 4;

  check_mul(a, b, n);
}

int main(void)
{
  uint32_t *a, *b;
  size_t base, n_pass;
  const size_t N_TESTS = 10;

  base = 10;

  for (size_t i = ((size_t) 1); i < base; ++i) {
    n_pass = ((size_t) 0);
    for (size_t j = ((size_t) 0); j < N_TESTS; ++j) {
      if ((a = gen_uint_arr(i,base,0)) == NULL) return 0;
      if ((b = gen_uint_arr(i,base,10)) == NULL) {
        free(a);
        return 1;
      }

      n_pass += ((size_t) check_mul(a, b, i));

      free(a);
      free(b);
    }
    printf("%zu/%zu pass for arrays of size %zu\n", n_pass, N_TESTS, i);
  }

  return 0;
}

