#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "linkedlists.h"
#include "hashtable.h"

typedef struct __hashtable_entry_struct_t {
  void *key;
  void *value;
} __hashtable_entry_t;

static void error_no_mem(void) {
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

hashtable_t *create_hashtable(size_t size)
{
  size_t n, i;
  hashtable_t *hashtable;

  n = size;
  if (n < ((size_t) 1)) {
    n = (size_t) 1;
  } 

  hashtable = (hashtable_t *) calloc(1, sizeof(hashtable_t));
  if (hashtable == NULL) error_no_mem();

  hashtable->size = n;
  hashtable->table = (list_t **) calloc(hashtable->size, sizeof(list_t *));
  if (hashtable->table == NULL) {
    free(hashtable);
    error_no_mem();
  }

  for (i = ((size_t) 0); i < hashtable->size; ++i) {
    hashtable->table[i] = NULL;
  }

  return hashtable;
}

static void __delete_hashtable_entry(void *entry, void *data)
{
  __hashtable_entry_t *pvt_entry = entry;
  struct {
    void (*delete_key)(void *, void *);
    void (*delete_value)(void *, void *);
    void *data;
  } *pvt_data = data;

  pvt_data->delete_key(pvt_entry->key, pvt_data->data);
  pvt_data->delete_value(pvt_entry->value, pvt_data->data);
  free(entry);
}

void delete_hashtable(hashtable_t *hashtable,
          void (*delete_key)(void *, void *),
          void (*delete_value)(void *, void *),
          void *data)
{
  size_t i;
  struct {
    void (*delete_key)(void *, void *);
    void (*delete_value)(void *, void *);
    void *data;
  } mydata;

  mydata.delete_key = delete_key;
  mydata.delete_value = delete_value;
  mydata.data = data;

  for (i = ((size_t) 0); i < hashtable->size; ++i) {
    if (hashtable->table[i] != NULL) {
      delete_list(hashtable->table[i], __delete_hashtable_entry, &mydata);
    }
  }

  free(hashtable->table);
  free(hashtable);
}

static int __compare_hashtable_entry(void *a, void *b, void *data)
{
  struct {
    int (*compare_keys)(void *, void *, void *);
    void *data;
  } *pvt_data = data;
  __hashtable_entry_t *pvt_a = a;
  __hashtable_entry_t *pvt_b = b;

  return pvt_data->compare_keys(pvt_a->key, pvt_b->key, pvt_data->data);
}

void *lookup_in_hashtable(hashtable_t *hashtable,
        void *key,
        uint32_t (*hash_key)(void *, void *),
        int (*compare_keys)(void *, void *, void *),
        void *data)
{
  uint32_t hash;
  size_t idx;
  __hashtable_entry_t *entry;
  struct __hashtable_entry_struct_t sought_entry;
  struct {
    int (*compare_keys)(void *, void *, void *);
    void *data;
  } mydata;

  hash = hash_key(key, data);
  idx = ((size_t) hash) % hashtable->size;
  
  if (hashtable->table[idx] == NULL) return NULL;

  sought_entry.key = key;
  sought_entry.value = NULL;
  mydata.compare_keys = compare_keys;
  mydata.data = data;
  
  entry = search_list(hashtable->table[idx],
          &sought_entry,
          __compare_hashtable_entry,
          &mydata);
  
  if (entry == NULL) return NULL;
  
  return entry->value;
}

static void *__copy_hashtable_entry(void *entry, void *data)
{
  struct {
    void *(*copy_key)(void *, void *);
    void *(*copy_value)(void *, void *);
    void *data;
  } *pvt_data = data;
  __hashtable_entry_t *pvt_entry = entry;
  __hashtable_entry_t *new_entry;

  new_entry = (__hashtable_entry_t *) calloc(1, sizeof(__hashtable_entry_t));
  if (new_entry == NULL) error_no_mem();

  new_entry->key = pvt_data->copy_key(pvt_entry->key, pvt_data->data);
  new_entry->value = pvt_data->copy_value(pvt_entry->value, pvt_data->data);

  return new_entry;
}

void add_to_hashtable(hashtable_t *hashtable,
          void *key,
          void *value,
          void *(*copy_key)(void *, void *),
          void *(*copy_value)(void *, void *),
          uint32_t (*hash_key)(void *, void *),
          void *data)
{
  uint32_t hash;
  size_t idx;
  struct __hashtable_entry_struct_t added_entry;
  struct {
    void *(*copy_key)(void *, void *);
    void *(*copy_value)(void *, void *);
    void *data;
  } mydata;

  hash = hash_key(key, data);
  idx = ((size_t) hash) % hashtable->size;

  if (hashtable->table[idx] == NULL) {
    hashtable->table[idx] = create_list();
  }

  added_entry.key = key;
  added_entry.value = value;
  mydata.copy_key = copy_key;
  mydata.copy_value = copy_value;
  mydata.data = data;

  // TODO:
  printf("--------------\n");
  printf("idx: %zu, key: %s\n", idx, (char *) key);
  printf("&hashtable->table[%zu] = %p\n", idx, &hashtable->table[idx]);
  printf("hashtable->table[%zu] = %p\n", idx, hashtable->table[idx]);

  prepend_to_list(hashtable->table[idx],
      &added_entry,
      __copy_hashtable_entry,
      &mydata);
}

size_t number_entries_in_hashtable(hashtable_t *hashtable)
{
  size_t i, k, l;

  k = (size_t) 0;
printf("215:\n");
printf("hashtable->size = %zu\n", hashtable->size);
  for (i = ((size_t) 0); i < hashtable->size; ++i) {
    if (hashtable->table[i] != NULL) {
      l = length_list(hashtable->table[i]);
      k += l;
    }
  }
printf("223:\n");

  return k;
}

size_t max_number_collisions_in_hashtable(hashtable_t *hashtable)
{
  size_t i, k, l;

  k = (size_t) 0;
  for (i = ((size_t)0); i < hashtable->size; ++i) {
    if (hashtable->table[i] != NULL) {
      l = length_list(hashtable->table[i]);
      if (l > k) k = l;
    }
  }

  if (k == ((size_t) 0)) return 0;

  return k - ((size_t) 1);
}

size_t number_empty_entries_in_hashtable(hashtable_t *hashtable)
{
  size_t i, n;

  n = (size_t) 0;
  for (i = ((size_t)0); i < hashtable->size; ++i) {
    if (hashtable->table[i] == NULL) ++n;
  }

  return n;
}

