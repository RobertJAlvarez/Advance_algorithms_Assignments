#ifndef __RED_BLACK_TREES_H__
#define __RED_BLACK_TREES_H__

#include <stdlib.h>

typedef struct __red_black_tree_struct_t red_black_tree_t;

/* Print red-black tree */
void print2D(const red_black_tree_t *tree, const int print_all);

/* Creates an empty red-black tree */
red_black_tree_t *red_black_tree_create(void);

/* Deletes a red-black tree, calling delete_key and delete_value
   on each key resp. value, passing in the data pointer.
*/
void red_black_tree_delete(
  red_black_tree_t *tree,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data
);

/* Returns the number of entries in a red-black tree

   Returns zero for an empty tree.
*/
size_t red_black_tree_number_entries(const red_black_tree_t *tree);

/* Returns the height of a red-black tree

   Returns zero for an empty tree.
*/
size_t red_black_tree_height(const red_black_tree_t *tree);

/* Searches a red-black tree for a key, comparing keys with
   compare_key, returning the associated value.

   Returns NULL if the sought for key cannot be found.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void *red_black_tree_search(
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data
);

/* Returns the minimum key and associated value, comparing
   the keys with compare_key.

   Returns NULL for both the key and the value if the
   tree is empty.
*/
void red_black_tree_minimum(
  void **min_key,
  void **min_value,
  const red_black_tree_t *tree
);

/* Returns the maximum key and associated value, comparing
   the keys with compare_key.

   Returns NULL for both the key and the value if the
   tree is empty.
*/
void red_black_tree_maximum(
  void **max_key,
  void **max_value,
  const red_black_tree_t *tree
);

/* Returns the predecessor of a key and value associated with that
   key, comparing the keys with compare_key.

   Returns NULL for both the key and the value if the
   key passed in argument cannot be found or if that
   key has no predecessor.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void red_black_tree_predecessor(
  void **prec_key,
  void **prec_value,
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data
);

/* Returns the successor of a key and value associated with that
   key, comparing the keys with compare_key.

   Returns NULL for both the key and the value if the
   key passed in argument cannot be found or if that
   key has no successor.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void red_black_tree_successor(
  void **succ_key,
  void **succ_value,
  const red_black_tree_t *tree,
  const void *key,
  int (*compare_key)(const void *, const void *, void *),
  void *data
);

/* Inserts a key and an associated value into a tree, comparing the
   keys with compare_key and copying the key and value with the
   copy_key resp. copy_value functions.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void red_black_tree_insert(
  red_black_tree_t *tree,
  void *key,
  void *value,
  int (*compare_key)(const void *, const void *, void *),
  void *(*copy_key)(void *, void *),
  void *(*copy_value)(void *, void *),
  void *data
);

/* Removes a key and the associated value in a tree, comparing the
   keys with compare_key and deleting the key and value with the
   delete_key resp. delete_value function.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void red_black_tree_remove(
  red_black_tree_t *tree,
  void *key,
  int (*compare_key)(const void *, const void *, void *),
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data
);

/* Return heigh of tree if is balance.
   Return -1 if tree is unbalance.

   O(n)
*/
int is_balanced(const red_black_tree_t *tree);

#endif
