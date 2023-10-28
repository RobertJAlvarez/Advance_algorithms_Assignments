#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdint.h>

typedef struct __hashtable_struct_t {
  size_t size;
  list_t **table;
} hashtable_t;

/* Create a hashtable of certain size given in argument.

   O(n)

   Creates a hashtable of size 1, if the size in 
   argument is zero.
*/
hashtable_t *create_hashtable(size_t);

/* Delete a hashtable. Calls delete_key for each key and
   calls delete_value for each value.

   O(n)

   The data pointer is given back to the delete_key
   and delete_value functions as their last argument.
*/
void delete_hashtable(hashtable_t *hashtable,
          void (*delete_key)(void *, void *),
          void (*delete_value)(void *, void *),
          void *data);

/* Lookup a key in a hashtable. Calls hash_key to compute 
   the hash. Calls compare_keys to compare the keys 
   in the collisions list with the given key.

   O(1) if no collision, O(n) if collisions.

   Returns a pointer to the value. The value is 
   the copy held in the hashtable. No copy is made.

   If the key is not found, returns NULL.

   The data pointer is given back to the hash_key
   and compare_keys functions as their last argument.
*/
void *lookup_in_hashtable(hashtable_t *hashtable,
        void *key,
        uint32_t (*hash_key)(void *, void *),
        int (*compare_keys)(void *, void *, void *),
        void *data);

/* Add a key->value pair to a hashtable. Calls 
   copy_key to copy the key. Calls copy_value to copy the 
   value. Calls hash_key to compute the hash of the key.

   O(1) if no collisions, O(n) if collisions.

   The data pointer is given back to the copy_key, copy_value
   and hash_key functions as their last argument.
*/
void add_to_hashtable(hashtable_t *hashtable,
          void *key,
          void *value,
          void *(*copy_key)(void *, void *),
          void *(*copy_value)(void *, void *),
          uint32_t (*hash_key)(void *, void *),
          void *data);

/* Returns the number of entries in the hashtable

   O(n)
*/
size_t number_entries_in_hashtable(hashtable_t *hashtable);

/* Returns the maximum number of collisions in the hashtable

   If the hashtable has no entries, returns 0.

   If the hashtable does have entries, returns the 
   length of the longest collision list minus 1.

   O(n)
*/
size_t max_number_collisions_in_hashtable(hashtable_t *hashtable);

/* Returns the number of entries that are empty in hashtable.

   O(n)
*/
size_t number_empty_entries_in_hashtable(hashtable_t *hashtable);

#endif
