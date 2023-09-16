#include <stdio.h>
#include <string.h>
#include "util.h"

/* START: multiply_fast helper function declarations */
static int calc_e(
  uint32_t *e, const uint32_t *x, const uint32_t *y, const uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b)
);

static uint32_t *helper_multiply(
  const uint32_t *x, const uint32_t *y, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b)
);

static uint32_t calc_n(const uint32_t *x, const uint32_t *y, uint32_t n);
/* END: multiply fast helper function declarations */

/* START: Naive multiplication */
static void prop_add(
  uint32_t h, uint32_t *c,
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  if (h == 0) return;
  add(&h, c, *c, h);
  prop_add(h, &c[1], add);
}

void multiply_schoolbook(
  uint32_t *c, const uint32_t *a, const uint32_t *b, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t h, l;
  size_t i, j;

  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      mul(&h, &l, a[i], b[j]);
      // Add low value to c[i+j] and propagate carry up if needed
      prop_add(l, &c[i+j], add);
      // Add high value to c[i+j+1] and propagate carry up if needed
      prop_add(h, &c[i+j+1], add);
    }
  }

  return;
}
/* END: Naive multiplication */

/* START: Karatsuba Algorithm */
static void add_arrays(
  uint32_t *c, const uint32_t *z, const uint32_t n,
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  for (uint32_t i = 0; i < n; ++i)
    prop_add(z[i], &c[i], add);
}

// e = (X_L + X_H)*(Y_L + Y_H) - a - d
static int calc_e(
  uint32_t *e, const uint32_t *x, const uint32_t *y, const uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *new_x, *new_y, *temp;
  uint32_t half_n, new_n;

  half_n = n/2;
  new_n = half_n + 1 + (n%2);

  // new_x = X_L + X_H
  if ((new_x = (uint32_t *) malloc(new_n*sizeof(uint32_t))) == NULL) return 1;
  memset(new_x, 0, new_n*sizeof(uint32_t));
  add_arrays(new_x, x, half_n, add);
  add_arrays(new_x, &x[half_n], n-half_n, add);

  // new_y = Y_L + Y_H
  if ((new_y = (uint32_t *) malloc(new_n*sizeof(uint32_t))) == NULL) {
    free(new_x);
    return 1;
  }
  memset(new_y, 0, new_n*sizeof(uint32_t));
  add_arrays(new_y, y, half_n, add);
  add_arrays(new_y, &y[half_n], n-half_n, add);

  new_n = calc_n(new_x, new_y, new_n);

  // e = new_x * new_y
  temp = helper_multiply(new_x, new_y, new_n, mul, add);

  free(new_x);
  free(new_y);

  if (temp == NULL) return 1;

  memcpy(e, temp, 2*new_n*sizeof(uint32_t));
  free(temp);

  // e = e - a - d
  // TODO: How to "-"?

  return 0;
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
  const uint32_t *x, const uint32_t *y, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *xy;
  uint32_t *a, *d;
  uint32_t l, h;
  uint32_t half_n, arr_size;

  if (n == 0) {
    fprintf(stderr, "ERROR: helper_multiply: size of arrays is 0.\n");
    return NULL;
  }

  xy = (uint32_t *) malloc(2*n*sizeof(uint32_t));

  if (n == 1) {
    mul(&h, &l, x[0], y[0]);
    prop_add(l, xy, add);
    prop_add(h, &xy[1], add);
    return xy;
  }

  half_n = n/2;
  arr_size = half_n * half_n;

  // a = X_H * Y_H
  if ((a = helper_multiply(&x[half_n], &y[half_n], n-half_n, mul, add)) == NULL) {
    free(xy);
    return NULL;
  }

  // d = X_L * Y_L
  if ((d = helper_multiply(x, y, half_n, mul, add)) == NULL) {
    free(xy);
    free(a);
    return NULL;
  }

  // e = (X_L + X_H)*(Y_L + Y_H) - a - d
  if (calc_e(xy, x, y, n, mul, add) > 0) {
    free(xy);
    free(a);
    free(d);
    return NULL;
  }

  // xy = a*b^n + d*b^(n/2) + e
  add_arrays(&xy[half_n], d, arr_size, add);
  add_arrays(&xy[n], a, arr_size, add);

  free(a);
  free(d);

  return xy;
}

void multiply_faster(
  uint32_t *c, const uint32_t *a, const uint32_t *b, uint32_t n,
  void (*mul)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b),
  void (*add)(uint32_t *h, uint32_t *l, uint32_t a, uint32_t b))
{
  uint32_t *ans;

  n = calc_n(a, b, n);

  if ((ans = helper_multiply(a, b, n, mul, add)) == NULL) return;

  memcpy(c, ans, n*n*sizeof(uint32_t));

  free(ans);

  return;
}
/* END: Karatsuba Algorithm */

int main(void)
{
  uint32_t *a, *b, *c;
  uint32_t n;
  size_t base;

  n = 5;
  base = 10;

  if ((a = gen_uint_arr(n,base)) == NULL) return 0;
  if ((b = gen_uint_arr(n,base)) == NULL) {
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

  memset(c, 0, 2*n*sizeof(uint32_t));
  multiply_schoolbook(c, a, b, n, mul10, add10);

  printf("c = ");
  print_uint_nums(c,n*n);

  free(a);
  free(b);
  free(c);

  return 0;
}

