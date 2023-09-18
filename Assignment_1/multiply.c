#include <stdio.h>
#include <string.h>
#include "util.h"

/* START: multiply_fast helper function declarations */
static uint32_t *helper_multiply(
  const uint32_t *x, const uint32_t *y, size_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*sub)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b)
);

static uint32_t calc_n(const uint32_t *x, const uint32_t *y, uint32_t n);
/* END: multiply fast helper function declarations */

/* START: Naive multiplication */
static void prop_op(
  uint32_t h, uint32_t *c,
  void (*op)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  if (h == 0) return;
  op(&h, c, *c, h);
  prop_op(h, &c[1], op);
}

void multiply_schoolbook(
  uint32_t *c, const uint32_t *a, const uint32_t *b, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t h, l;
  size_t i, j;

  memset(c, 0, 2*n*sizeof(uint32_t));

  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      mul(&h, &l, a[i], b[j]);
      // Add low value to c[i+j] and propagate carry up if needed
      prop_op(l, &c[i+j], add);
      // Add high value to c[i+j+1] and propagate carry up if needed
      prop_op(h, &c[i+j+1], add);
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
  for (uint32_t i = 0; i < n; ++i)
    prop_op(z[i], &c[i], op);
}

// e = (X_L + X_H)*(Y_L + Y_H) - a - d
static uint32_t *calc_e(
  const uint32_t *x, const uint32_t *y, const size_t n,
  uint32_t *a, uint32_t *d,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*sub)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *new_x, *new_y, *e;
  size_t half_n, new_n;

  half_n = n/2;
  new_n = half_n + 1 + (n%2);

  // new_x = X_L + X_H
  if ((new_x = (uint32_t *) malloc(new_n*sizeof(uint32_t))) == NULL) return NULL;
  memset(new_x, 0, new_n*sizeof(uint32_t));
  array_op(new_x, x, half_n, add);
  array_op(new_x, &x[half_n], n-half_n, add);

  // new_y = Y_L + Y_H
  if ((new_y = (uint32_t *) malloc(new_n*sizeof(uint32_t))) == NULL) {
    free(new_x);
    return NULL;
  }
  memset(new_y, 0, new_n*sizeof(uint32_t));
  array_op(new_y, y, half_n, add);
  array_op(new_y, &y[half_n], n-half_n, add);

  new_n = calc_n(new_x, new_y, new_n);

  // e = new_x * new_y
  e = helper_multiply(new_x, new_y, new_n, mul, add, sub);

  free(new_x);
  free(new_y);

  if (e == NULL) return NULL;

  // e = e - a - d
  array_op(e, a, n, sub);
  array_op(e, d, n, sub);

  return e;
}

static uint32_t calc_n(const uint32_t *x, const uint32_t *y, uint32_t n)
{
  if (n == 0) return 0;

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
  uint32_t l, h;
  size_t half_n;

  if (n == 0) {
    fprintf(stderr, "ERROR: helper_multiply: size of arrays is 0.\n");
    return NULL;
  }

  xy = (uint32_t *) malloc(2*n*sizeof(uint32_t));

  if (n == 1) {
    mul(&h, &l, x[0], y[0]);
    prop_op(l, xy, add);
    prop_op(h, &xy[1], add);
    return xy;
  }

  half_n = n/2;

  // a = X_H * Y_H
  if ((a = helper_multiply(&x[half_n], &y[half_n], n-half_n, mul, add, sub)) == NULL) {
    free(xy);
    return NULL;
  }

  // d = X_L * Y_L
  if ((d = helper_multiply(x, y, half_n, mul, add, sub)) == NULL) {
    free(xy);
    free(a);
    return NULL;
  }

  // e = (X_L + X_H)*(Y_L + Y_H) - a - d
  if ((e = calc_e(x, y, n, a, d, mul, add, sub)) == NULL) {
    free(xy);
    free(a);
    free(d);
    return NULL;
  }

  // xy = a*b^n + e*b^(n/2) + d
  memcpy(xy, d, n*sizeof(uint32_t));
  array_op(&xy[half_n], e, n, add);
  array_op(&xy[n], a, n, add);

  free(a);
  free(d);
  free(e);

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
void test(void)
{
  // 1,234 * 4,321 = 5,332,114
  uint32_t a[] = {4,3,2,1};
  uint32_t b[] = {1,2,3,4};
  size_t n = 4;
  uint32_t *c;

  if ((c = (uint32_t *) malloc(2*n*sizeof(uint32_t))) == NULL) return;
  memset(c, 0, 2*n*sizeof(uint32_t));

  multiply_faster(c, a, b, n, mul10, add10, sub10);
}

int main(void)
{
  uint32_t *a, *b, *c;
  size_t n;
  size_t base;

  n = 3;
  base = 10;

  if ((a = gen_uint_arr(n,base,0)) == NULL) return 0;
  if ((b = gen_uint_arr(n,base,10)) == NULL) {
    free(a);
    return 1;
  }
  if ((c = (uint32_t *) malloc(2*n*sizeof(uint32_t))) == NULL) {
    free(a);
    free(b);
    return 1;
  }

  printf("a = ");
  print_uint_nums(a,n);
  printf("b = ");
  print_uint_nums(b,n);

  multiply_faster(c, a, b, n, mul10, add10, sub10);
  printf("multiply_faster:\n\ta * b = ");
  print_uint_nums(c,2*n);

  multiply_schoolbook(c, a, b, n, mul10, add10);
  printf("multiply_schoolbook:\n\ta * b = ");
  print_uint_nums(c,2*n);

  free(a);
  free(b);
  free(c);

  return 0;
}

