#include <stdio.h>
#include "searchtrees.h"

typedef struct __tree_node_struct_t {
  void *key;
  void *value;
  struct __tree_node_struct_t *parent;
  struct __tree_node_struct_t *left;
  struct __tree_node_struct_t *right;
} tree_node_t;

struct __search_tree_struct_t {
  tree_node_t *root;
};

static void error_no_mem(void)
{
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

search_tree_t *search_tree_create(void)
{
  search_tree_t *tree;

  tree = calloc(1, sizeof(search_tree_t));
  if (tree == NULL) error_no_mem();

  tree->root = NULL;

  return tree;
}

static void __search_tree_delete_aux(
  tree_node_t *node,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
  if (node == NULL) return;
  __search_tree_delete_aux(node->left,
                           delete_key, delete_value, data);
  __search_tree_delete_aux(node->right,
                           delete_key, delete_value, data);
  delete_key(node->key, data);
  delete_value(node->value, data);
  free(node);
}

void search_tree_delete(
  search_tree_t *tree,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
  __search_tree_delete_aux(tree->root, delete_key, delete_value, data);
  free(tree);
}

static size_t __search_tree_number_entries_aux(tree_node_t *node)
{
  size_t l, r, n;

  if (node == NULL) return ((size_t) 0);

  l = __search_tree_number_entries_aux(node->left);
  r = __search_tree_number_entries_aux(node->right);

  n = (l + r) + ((size_t) 1);

  return n;
}

size_t search_tree_number_entries(search_tree_t *tree)
{
  return __search_tree_number_entries_aux(tree->root);
}

static size_t __search_tree_height_aux(tree_node_t *node)
{
  size_t l, r, h;

  if (node == NULL) return ((size_t) 0);

  l = __search_tree_height_aux(node->left);
  r = __search_tree_height_aux(node->right);

  h = ((l > r) ? l : r) + ((size_t) 1);

  return h;
}

size_t search_tree_height(search_tree_t *tree)
{
  return __search_tree_height_aux(tree->root);
}

static tree_node_t *__search_tree_search_aux(
  tree_node_t *node,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data)
{
  int cmp;

  if (node == NULL) return NULL;

  cmp = compare_key(key, node->key, data);

  if (cmp == 0) return node;
  if (cmp < 0) return __search_tree_search_aux(node->left,
                                               key,
                                               compare_key,
                                               data);
  return __search_tree_search_aux(node->right,
                                  key,
                                  compare_key,
                                  data);
}

void *search_tree_search(
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data)
{
  tree_node_t *node;

  node = __search_tree_search_aux(tree->root,
                                  key,
                                  compare_key,
                                  data);
  if (node == NULL) return NULL;
  return node->value;
}

void search_tree_minimum(
  void **min_key,
  void **min_value,
  search_tree_t *tree)
{
  tree_node_t *node;

  if (tree->root == NULL) {
    *min_key = NULL;
    *min_value = NULL;
    return;
  }

  for (node = tree->root; node->left != NULL; node = node->left);

  *min_key = node->key;
  *min_value = node->value;
}

void search_tree_maximum(
  void **max_key,
  void **max_value,
  search_tree_t *tree)
{
  tree_node_t *node;

  if (tree->root == NULL) {
    *max_key = NULL;
    *max_value = NULL;
    return;
  }

  for (node = tree->root; node->right != NULL; node = node->right);

  *max_key = node->key;
  *max_value = node->value;
}

void search_tree_predecessor(
  void **prec_key,
  void **prec_value,
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data)
{
  tree_node_t *x, *y;

  x = __search_tree_search_aux(tree->root, key, compare_key, data);

  // If node doesn't exists
  if (x == NULL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  // If there is a left node, go left and then all right
  if (x->left != NULL) {
    for (y = x->left; y->right != NULL; y = y->right);
    *prec_key = y->key;
    *prec_value = y->value;
    return;
  }

  // Find the next parent where x is a right child from
  for (y = x->parent; ((y != NULL) && (x == y->left)); ) {
    x = y;
    y = y->parent;
  }

  // If parent doesn't exists return NULL (x is the minimum element in the tree)
  if (y == NULL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  // Otherwise, return parent key,value
  *prec_key = y->key;
  *prec_value = y->value;
}

void search_tree_successor(
  void **succ_key,
  void **succ_value,
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data)
{
  tree_node_t *x, *y;

  x = __search_tree_search_aux(tree->root, key, compare_key, data);

  // If node doesn't exists
  if (x == NULL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  // If there is a right node, go one right and then all left
  if (x->right != NULL) {
    for (y = x->right; y->left != NULL; y = y->left);
    *succ_key = y->key;
    *succ_value = y->value;
    return;
  }

  // Find the next parent where x is a child in the left sub-tree
  for (y = x->parent; ((y != NULL) && (x == y->right)); ) {
    x = y;
    y = y->parent;
  }

  // If parent doesn't exists return NULL (x is the maximum element in the tree)
  if (y == NULL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  // Otherwise return parent key,value
  *succ_key = y->key;
  *succ_value = y->value;
}

static tree_node_t *__search_tree_insert_aux(
  void *key,
  void *value,
  void *(*copy_key)(void *, void *),
  void *(*copy_value)(void *, void *),
  void *data)
{
  tree_node_t *new_node;

  new_node = calloc(1, sizeof(tree_node_t));
  if (new_node == NULL) error_no_mem();

  new_node->key = copy_key(key, data);
  new_node->value = copy_value(value, data);
  new_node->parent = NULL;
  new_node->left = NULL;
  new_node->right = NULL;

  return new_node;
}

void search_tree_insert(
  search_tree_t *tree,
  void *key,
  void *value,
  int (*compare_key)(void *, void *, void *),
  void *(*copy_key)(void *, void *),
  void *(*copy_value)(void *, void *),
  void *data)
{
  tree_node_t *x, *y, *z;

  if (__search_tree_search_aux(tree->root, key, compare_key, data) != NULL) return;

  z = __search_tree_insert_aux(key, value, copy_key, copy_value, data);

  if (tree->root == NULL) {
    tree->root = z;
    return;
  }

  x = tree->root;
  y = NULL;
  while (x!=NULL) {
    y = x;
    if (compare_key(z->key, x->key, data) < 0) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == NULL) {
    tree->root = z;
  } else {
    if (compare_key(z->key, y->key, data) < 0) {
      y->left = z;
    } else {
      y->right = z;
    }
  }
}

static void __search_tree_remove_aux_transplant(
  search_tree_t *tree,
  tree_node_t *u,
  tree_node_t *v)
{
  if (u->parent == NULL) {
    tree->root = v;
  } else {
    if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
  }
  if (v != NULL) {
    v->parent = u->parent;
  }
}

static tree_node_t *__search_tree_remove_aux_minimum(tree_node_t *z)
{
  tree_node_t *x;

  for (x = z; x->left != NULL; x = x->left);

  return x;
}

static void __search_tree_remove_aux(
  search_tree_t *tree,
  tree_node_t *z)
{
  tree_node_t *y;

  if (z->left == NULL) {
    __search_tree_remove_aux_transplant(tree, z, z->right);
  } else {
    if (z->right == NULL) {
      __search_tree_remove_aux_transplant(tree, z, z->left);
    } else {
      y = __search_tree_remove_aux_minimum(z->right);
      if (y != z->right) {
        __search_tree_remove_aux_transplant(tree, y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }
      __search_tree_remove_aux_transplant(tree, z, y);
      y->left = z->left;
      y->left->parent = y;
    }
  }
}

void search_tree_remove(
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
  tree_node_t *z;

  z = __search_tree_search_aux(tree->root, key, compare_key, data);

  if (z == NULL) return;

  __search_tree_remove_aux(tree, z);

  delete_key(z->key, data);
  delete_value(z->value, data);
  free(z);
}

