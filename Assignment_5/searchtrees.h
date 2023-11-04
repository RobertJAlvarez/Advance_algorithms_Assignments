#ifndef __SEARCH_TREES_H__
#define __SEARCH_TREES_H__

#include <stdlib.h>


typedef struct __search_tree_struct_t search_tree_t;

/* Creates an empty search tree */
search_tree_t *search_tree_create(void);

/* Deletes a search tree, calling delete_key and delete_value
   on each key resp. value, passing in the data pointer.
*/
void search_tree_delete(
  search_tree_t *tree,
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data
);

/* Returns the number of entries in a search tree

   Returns zero for an empty tree.
*/
size_t search_tree_number_entries(search_tree_t *tree);

/* Returns the height of a search tree

   Returns zero for an empty tree.
*/
size_t search_tree_height(search_tree_t *tree);

/* Searches a search tree for a key, comparing keys with
   compare_key, returning the associated value.

   Returns NULL if the sought for key cannot be found.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void *search_tree_search(
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data
);

/* Returns the minimum key and associated value, comparing
   the keys with compare_key.

   Returns NULL for both the key and the value if the
   tree is empty.
*/
void search_tree_minimum(
  void **min_key,
  void **min_value,
  search_tree_t *tree
);

/* Returns the maximum key and associated value, comparing
   the keys with compare_key.

   Returns NULL for both the key and the value if the
   tree is empty.
*/
void search_tree_maximum(
  void **max_key,
  void **max_value,
  search_tree_t *tree
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
void search_tree_predecessor(
  void **prec_key,
  void **prec_value,
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
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
void search_tree_successor(
  void **succ_key,
  void **succ_value,
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void *data
);

/* Inserts a key and an associated value into a tree, comparing the
   keys with compare_key and copying the key and value with the
   copy_key resp. copy_value functions.

   compare_key takes two keys and the data pointer in
   argument. It returns -1, 0, 1 depending on the
   ordering of the two keys.
*/
void search_tree_insert(
  search_tree_t *tree,
  void *key,
  void *value,
  int (*compare_key)(void *, void *, void *),
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
void search_tree_remove(
  search_tree_t *tree,
  void *key,
  int (*compare_key)(void *, void *, void *),
  void (*delete_key)(void *, void *),
  void (*delete_value)(void *, void *),
  void *data
);

#endif
