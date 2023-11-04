#include <stdio.h>
#include "redblacktrees.h"

typedef enum { RED_BLACK_TREE_COLOR_RED, RED_BLACK_TREE_COLOR_BLACK } color_t;

typedef struct __tree_node_struct_t {
  void *key;
  void *value;
  color_t color;
  struct __tree_node_struct_t *parent;
  struct __tree_node_struct_t *left;
  struct __tree_node_struct_t *right;
} tree_node_t;

struct __red_black_tree_struct_t {
  tree_node_t *root;
};

static tree_node_t *T_NIL = NULL;

static void error_no_mem(void)
{
//printf("8: error_no_mem()\n");
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

static void print2DUtil(const tree_node_t *node, const int space, const int print_all)
{
  char node_color;

  // Base case
  if (node == T_NIL) return;
 
  // Process right child first
  print2DUtil(node->right, space+1, print_all);
 
  // Print current node
  if (print_all) {
    for (int i = 0; i < space; i++) printf("\t");
    node_color = (node->color == RED_BLACK_TREE_COLOR_RED ? 'R' : 'B');
    printf("%c: %s,%s\n", node_color, (char *) node->key, (char *) node->value);
  } else {
    for (int i = 0; i < space; i++) printf(" ");
    printf("%c\n", (node->color == RED_BLACK_TREE_COLOR_RED ? 'R' : 'B'));
  }
 
  // Process left child
  print2DUtil(node->left, space+1, print_all);
}

void print2D(const red_black_tree_t *tree, const int print_all)
{
  if (tree == NULL) return;

  print2DUtil(tree->root, 0, print_all);
}

red_black_tree_t *red_black_tree_create(void)
{
  red_black_tree_t *tree;
//printf("16: red_black_tree_create()\n");

  tree = (red_black_tree_t *) calloc(1, sizeof(red_black_tree_t));
  if (tree == NULL) error_no_mem();

  // Initialize sentinel, if needed
  if (T_NIL == NULL) {
    T_NIL = (tree_node_t *) calloc(1, sizeof(tree_node_t));
    if (T_NIL == NULL) {
      free(tree);
      return NULL;
    }
    // Set sentinel fields
    T_NIL->color = RED_BLACK_TREE_COLOR_BLACK;
    T_NIL->parent = T_NIL->left = T_NIL->right = T_NIL;
    T_NIL->key = T_NIL->value = NULL;
  }

  tree->root = T_NIL;

  return tree;
}

static void __red_black_tree_delete_aux(
  tree_node_t *node,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
//printf("45: __red_black_tree_delete_aux()\n");
  if (node == T_NIL) return;

  __red_black_tree_delete_aux(node->left, delete_key, delete_value, data);
  __red_black_tree_delete_aux(node->right, delete_key, delete_value, data);

  delete_key(node->key, data);
  delete_value(node->value, data);

  free(node);
}

void red_black_tree_delete(
  red_black_tree_t *tree,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
//printf("63: red_black_tree_delete()\n");
  if (tree == NULL) return;

  __red_black_tree_delete_aux(tree->root, delete_key, delete_value, data);

  free(tree);
  free(T_NIL);

  T_NIL = NULL;
}

static size_t __red_black_tree_number_entries_aux(const tree_node_t *node)
{
  size_t l, r;
//printf("74: __red_black_tree_number_entries_aux()\n");

  if (node == T_NIL) return ((size_t) 0);

  l = __red_black_tree_number_entries_aux(node->left);
  r = __red_black_tree_number_entries_aux(node->right);

  return (l + r) + ((size_t) 1);
}

size_t red_black_tree_number_entries(const red_black_tree_t *tree)
{
//printf("86: red_black_tree_number_entries()\n");
  if (tree == NULL) return ((size_t) 0);

  return __red_black_tree_number_entries_aux(tree->root);
}

static size_t __red_black_tree_height_aux(const tree_node_t *node)
{
  size_t l, r;
//printf("95: __red_black_tree_height_aux()\n");

  if (node == T_NIL) return ((size_t) 0);

  l = __red_black_tree_height_aux(node->left);
  r = __red_black_tree_height_aux(node->right);

  return ((l > r) ? l : r) + ((size_t) 1);
}

size_t red_black_tree_height(const red_black_tree_t *tree)
{
//printf("107: red_black_tree_height()\n");
  if (tree == NULL) return ((size_t) 0);

  return __red_black_tree_height_aux(tree->root);
}

static void *__red_black_tree_search_aux(
  tree_node_t *node,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data)
{
  int cmp;

//printf("121: __red_black_tree_search_aux()\n");

  while (node != T_NIL) {
    cmp = compare_key(key, node->key, data);
    if (cmp == 0) break;
    else if (cmp < 0) node = node->left;
    else node = node->right;
  }

  return node;
}

void *red_black_tree_search(
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data)
{
  tree_node_t *node;

//printf("141: red_black_tree_search()\n");

  if (tree == NULL) return NULL;

  node = (tree_node_t *) __red_black_tree_search_aux(tree->root, key, compare_key, data);

  if (node == T_NIL) return NULL;

  return node->value;
}

void red_black_tree_minimum(
  void **min_key,
  void **min_value,
  const red_black_tree_t *tree)
{
  tree_node_t *node;

//printf("159: red_black_tree_minimum()\n");

  if ((tree == NULL) || (tree->root == T_NIL)) {
    *min_key = NULL;
    *min_value = NULL;
    return;
  }

  for (node = tree->root; node->left != T_NIL; node = node->left);

  *min_key = node->key;
  *min_value = node->value;
}

void red_black_tree_maximum(
  void **max_key,
  void **max_value,
  const red_black_tree_t *tree)
{
  tree_node_t *node;
//printf("179: red_black_tree_maximum()\n");

  if ((tree == NULL) || (tree->root == T_NIL)) {
    *max_key = NULL;
    *max_value = NULL;
    return;
  }

  for (node = tree->root; node->right != T_NIL; node = node->right);

  *max_key = node->key;
  *max_value = node->value;
}

void red_black_tree_predecessor(
  void **prec_key,
  void **prec_value,
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data)
{
  tree_node_t *x, *y;
//printf("202: red_black_tree_predecessor()\n");

  if (tree == NULL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  x = (tree_node_t *) __red_black_tree_search_aux(tree->root, key, compare_key, data);

  // If node doesn't exists
  if (x == T_NIL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  // If there is a left node, go left and then all right
  if (x->left != T_NIL) {
    for (y = x->left; y->right != T_NIL; y = y->right);
    *prec_key = y->key;
    *prec_value = y->value;
    return;
  }

  // Find the next parent where x is a child in the right sub-tree
  for (y = x->parent; ((y != T_NIL) && (x == y->left)); ) {
    x = y;
    y = y->parent;
  }

  // If parent doesn't exists return NULL (x is the minimum element in the tree)
  if (y == T_NIL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  // Otherwise, return parent key,value
  *prec_key = y->key;
  *prec_value = y->value;
}

void red_black_tree_successor(
  void **succ_key,
  void **succ_value,
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data)
{
  tree_node_t *x, *y;
//printf("254: red_black_tree_successor()\n");

  if (tree == NULL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  x = (tree_node_t *) __red_black_tree_search_aux(tree->root, key, compare_key, data);

  // If node doesn't exists
  if (x == T_NIL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  // If there is a right node, go one right and then all left
  if (x->right != T_NIL) {
    for (y = x->right; y->left != T_NIL; y = y->left);
    *succ_key = y->key;
    *succ_value = y->value;
    return;
  }

  // Find the next parent where x is a child in the left sub-tree
  for (y = x->parent; ((y != T_NIL) && (x == y->right)); ) {
    x = y;
    y = y->parent;
  }

  // If parent doesn't exists return NULL (x is the maximum element in the tree)
  if (y == T_NIL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  // Otherwise return parent key,value
  *succ_key = y->key;
  *succ_value = y->value;
}

static void __right_rotate(
  red_black_tree_t *tree,
  tree_node_t *x)
{
  tree_node_t *y;
//printf("302: __right_rotate()\n");

  if (x == T_NIL) return;

  y = x->left;
  x->left = y->right;   // Turn y's right sub-tree into x's left sub-tree

  // If y's right sub-tree is not empty, then x becomes the parent of the sub-tree's root
  if (y->right != T_NIL)
    y->right->parent = x;

  // x's parent becomes y's parent
  y->parent = x->parent;

  // If x was the root then y becomes the root
  if (x->parent == T_NIL)
    tree->root = y;
  // Otherwise, if x was a right child then y becomes a right child
  else if (x == x->parent->right)
    x->parent->right = y;
  // Otherwise, x was a left child, and now y is
  else
    x->parent->left = y;

  // Make x become y's right child
  y->right = x;
  x->parent = y;
}

static void __left_rotate(
  red_black_tree_t *tree,
  tree_node_t *x)
{
  tree_node_t *y;
//printf("336: __left_rotate()\n");

  if (x == T_NIL) return;

  y = x->right;
  x->right = y->left;   //Turn y's left sub-tree into x's right sub-tree

  // If y's left sub-tree is not empty, then x becomes the parent of the sub-tree's root
  if (y->left != T_NIL)
    y->left->parent = x; 

  // x's parent becomes y's parent
  y->parent = x->parent;

  // If x was the root then y becomes the root
  if (x->parent == T_NIL)
    tree->root = y;
  // Otherwise, if x was a left child then y becomes a left child
  else if (x == x->parent->left)
    x->parent->left = y;
  // Otherwise, x was a right child, and now y is
  else
    x->parent->right = y;

  // Make x become y's left child
  y->left = x;
  x->parent = y;
}

static void __red_black_insert_fixup(
  red_black_tree_t *tree,
  tree_node_t *z)
{
  tree_node_t *y;
//printf("370: __red_black_insert_fixup()\n");

  while (z->parent->color == RED_BLACK_TREE_COLOR_RED) {
    // Is z's parent a left child?
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right; // y is z's uncle
      // Are z's parent and uncle both red?
      if (y->color == RED_BLACK_TREE_COLOR_RED) {
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        y->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          __left_rotate(tree,z);
        }
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        __right_rotate(tree, z->parent->parent);
      }
    }
    // Same as first if but with "right" and "left" exchanged
    else {
      y = z->parent->parent->left;
      if (y->color == RED_BLACK_TREE_COLOR_RED) {
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        y->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          __right_rotate(tree, z);
        }
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        __left_rotate(tree, z->parent->parent);
      }
    }
  }

  tree->root->color = RED_BLACK_TREE_COLOR_BLACK;
}

void red_black_tree_insert(
  red_black_tree_t *tree,
  void *key,
  void *value,
  int (*compare_key)(const void *, const void *, void *),
  void *(*copy_key)(void *, void *),
  void *(*copy_value)(void *, void *),
  void *data)
{
  tree_node_t *x, *y, *z;
//printf("425: red_black_tree_insert()\n");

  if (tree == NULL) return;

  // Make space for z and set key value fields
  z = (tree_node_t *) calloc(1, sizeof(tree_node_t));
  if (z == NULL) error_no_mem();
  z->key = copy_key(key, data);
  z->value = copy_value(value, data);

  x = tree->root;
  y = T_NIL;

  // Descend until reaching the sentinel
  while (x != T_NIL) {
    y = x;
    x = (compare_key(key, x->key, data) < 0 ? x->left : x->right);
  }

  // Found the location - insert z with parent y
  z->parent = y;

  if (y == T_NIL) tree->root = z; // Tree was empty
  else if (compare_key(z->key, y->key, data) < 0) y->left = z;
  else y->right = z;

  // Set left, right, and color z fields
  z->left = T_NIL;   // Both of z's children are the sentinel
  z->right = T_NIL;
  z->color = RED_BLACK_TREE_COLOR_RED;  // The new node starts out red

  // Correct any violations of red-black properties
  __red_black_insert_fixup(tree, z);
}

static void __red_black_transplant(
  red_black_tree_t *tree,
  tree_node_t *u,
  tree_node_t *v)
{
//printf("465: __red_black_transplant()\n");
  if (u->parent == T_NIL) tree->root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;

  v->parent = u->parent;
}

static tree_node_t *__red_black_tree_minimum(tree_node_t *x)
{
//printf("475: __red_black_tree_minimum()\n");
  if (x == T_NIL) return T_NIL;
  while (x->left != T_NIL) x = x->left;
  return x;
}

static void __red_black_delete_fixup(red_black_tree_t *tree, tree_node_t *x)
{
  tree_node_t *w;
//printf("484: __red_black_delete_fixup()\n");

  while ((x != tree->root) && (x->color == RED_BLACK_TREE_COLOR_BLACK)) {
    // Is x a left child?
    if (x == x->parent->left) {
      // w is x's sibling
      w = x->parent->right;
      if (w->color == RED_BLACK_TREE_COLOR_RED) {
        w->color = RED_BLACK_TREE_COLOR_BLACK;
        x->parent->color = RED_BLACK_TREE_COLOR_RED;
        __left_rotate(tree,x->parent);
        w = x->parent->right;
      }
      if ((w->left->color == RED_BLACK_TREE_COLOR_BLACK) && (w->right->color == RED_BLACK_TREE_COLOR_BLACK)) {
        w->color = RED_BLACK_TREE_COLOR_RED;
        x = x->parent;
      } else {
        if (w->right->color == RED_BLACK_TREE_COLOR_BLACK) {
          w->left->color = RED_BLACK_TREE_COLOR_BLACK;
          w->color = RED_BLACK_TREE_COLOR_RED;
          __right_rotate(tree, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        w->right->color = RED_BLACK_TREE_COLOR_BLACK;
        __left_rotate(tree, x->parent);
        x = tree->root;
      }
    }
    // Same as if case but with "right" and "left" exchanged
    else {
      w = x->parent->left;
      if (w->color == RED_BLACK_TREE_COLOR_RED) {
        w->color = RED_BLACK_TREE_COLOR_BLACK;
        x->parent->color = RED_BLACK_TREE_COLOR_RED;
        __right_rotate(tree, x->parent);
        w = x->parent->left;
      }
      if ((w->right->color == RED_BLACK_TREE_COLOR_BLACK) && (w->left->color == RED_BLACK_TREE_COLOR_BLACK)) {
        w->color = RED_BLACK_TREE_COLOR_RED;
        x = x->parent;
      } else {
        if (w->left->color == RED_BLACK_TREE_COLOR_BLACK) {
          w->right->color = RED_BLACK_TREE_COLOR_BLACK;
          w->color = RED_BLACK_TREE_COLOR_RED;
          __left_rotate(tree, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        w->left->color = RED_BLACK_TREE_COLOR_BLACK;
        __right_rotate(tree, x->parent);
        x = tree->root;
      }
    }
  }

  x->color = RED_BLACK_TREE_COLOR_BLACK;
}

void red_black_tree_remove(
  red_black_tree_t *tree,
  void *key,
  int (*compare_key)(const void *, const void *, void *),
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data)
{
  tree_node_t *x, *y, *z;
  color_t y_org_color;
//printf("555: red_black_tree_remove()\n");

  if (tree == NULL) return;

  // If key is not on the tree we return
  z = (tree_node_t *) __red_black_tree_search_aux(tree->root, key, compare_key, data);
  if (z == T_NIL) return;

  y = z;
  y_org_color = y->color;

  if (z->left == T_NIL) {
    x = z->right;
    // Replace z by its right child
    __red_black_transplant(tree, z, z->right);
  } else if (z->right == T_NIL) {
    x = z->left;
    // Replace z by its left child
    __red_black_transplant(tree, z, z->left);
  } else {
    // y is z's successor
    y = (tree_node_t *) __red_black_tree_minimum(z->right);
    y_org_color = y->color;
    x = y->right;
    // Is y father down the tree?
    if (y != z->right) {
      // Replace y by its right child
      __red_black_transplant(tree, y, y->right);
      // z's right child becomes
      y->right = z->right;
      // y's right child
      y->right->parent = y;
    } else {
      // In case x is NULL
      x->parent = y;
    }
    // Replace z by its successor y
    __red_black_transplant(tree, z, y);
    // And give z's left child to y
    y->left = z->left;
    // Which had no left child
    y->left->parent = y;
    y->color = z->color;
  }

  // If any red-black violations occurred, correct them
  if (y_org_color == RED_BLACK_TREE_COLOR_BLACK)
    __red_black_delete_fixup(tree, x);

  // Delete z
  delete_key(z->key, data);
  delete_value(z->value, data);
  free(z);
}

static int black_height(const tree_node_t *node)
{
  int l, r;

  if (node == T_NIL) return 1;
  if ((l = black_height(node->left)) == 0) return 0;
  if ((r = black_height(node->right)) == 0) return 0;
  if (l != r) return 0;

  return l + (node->color == RED_BLACK_TREE_COLOR_BLACK ? 1 : 0);
}

int red_black_tree_is_balanced(const red_black_tree_t *tree)
{
  if (tree == NULL) return 0;
  return black_height(tree->root);
}

