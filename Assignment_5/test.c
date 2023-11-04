#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include "redblacktrees.h"
#include "searchtrees.h"

#define LINE_BUFFER_LEN ((size_t) 4096)

static void error_no_mem(void)
{
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

static void input_string(char str[], size_t n)
{
  char c;
  size_t i;
  int first;

  if (n < ((size_t) 1)) return;
  first = 1;
  for (i=0; i<(n-((size_t) 1)); i++) {
    scanf("%c", &c);
    if (c == '\n') {
      if (first) {
        first = 0;
        i = (size_t) 0;
        i--;
        continue;
      } else {
        break;
      }
    }
    str[i] = c;
    first = 0;
  }
  str[i] = '\0';
}

static void delete_string(void *ptr)
{
  free(ptr);
}

static void delete_key(void *ptr, void *data)
{
  delete_string(ptr);
}

static void delete_value(void *ptr, void *data)
{
  delete_string(ptr);
}

static void *copy_string(void *ptr)
{
  char *str = ptr;
  size_t len;
  char *new_str;

  len = strlen(ptr);
  len++;

  new_str = calloc(len, sizeof(char));
  if (new_str == NULL) error_no_mem();

  strcpy(new_str, str);

  return new_str;
}

static void *copy_key(void *ptr, void *data)
{
  return copy_string(ptr);
}

static void *copy_value(void *ptr, void *data)
{
  return copy_string(ptr);
}

static int compare_key(const void *ptr_a, const void *ptr_b, void *data)
{
  const char *str_a = ptr_a;
  const char *str_b = ptr_b;

  return strcmp(str_a, str_b);
}

static void rbt_print_menu(void)
{
  printf("1. Print tree characteristics.\n");
  printf("2. Insert new node.\n");
  printf("3. Search for node.\n");
  printf("4. Delete node.\n");
  printf("5. Print tree.\n");
  printf("6. Exit\n");
}

static void rbt_menu(void)
{
  char key[LINE_BUFFER_LEN];
  char value[LINE_BUFFER_LEN];
  char *temp_key, *temp_value;
  red_black_tree_t *tree;

  tree = red_black_tree_create();

  while (1) {
    rbt_print_menu();

    input_string(key, sizeof(key));
    if (key[0] == '6') break;

    switch (atoi(key)) {
    case 1:
      printf("The current search tree has %zu entries.\n", red_black_tree_number_entries(tree));
      printf("The current search tree has height %zu.\n", red_black_tree_height(tree));
      if (red_black_tree_is_balanced(tree) < 0) {
        printf("WARNING: tree is not balanced\n");
      }

      // Print tree min
      red_black_tree_minimum((void **) &temp_key, (void **) &temp_value, tree);
      if ((temp_key != NULL) && (temp_value != NULL)) {
        printf("The minimum key is \"%s\", the associated value is \"%s\".\n", temp_key, temp_value);
      } else {
        printf("The tree has no minimum key.\n");
      }

      // Print tree max
      red_black_tree_maximum((void **) &temp_key, (void **) &temp_value, tree);
      if ((temp_key != NULL) && (temp_value != NULL)) {
        printf("The maximum key is \"%s\", the associated value is \"%s\".\n", temp_key, temp_value);
      } else {
        printf("The tree has no maximum key.\n");
      }
      break;
    case 2:
      // Read new key
      printf("Please enter a key to add to the tree. Enter <Q> to stop.\n");
      input_string(key, sizeof(key));
      // Break if <Q>
      if (strcmp(key, "<Q>") == 0) break;
      // Read new value
      printf("Please enter a value associated with the key.\n");
      input_string(value, sizeof(value));
      // If node with same key already exist, don't add new node. Otherwise, insert it into tree
      temp_value = red_black_tree_search(tree, key, compare_key, NULL);
      if (temp_value != NULL) {
        printf("Cannot enter the new key \"%s\" with new value \"%s\" as the tree already contains the key with value \"%s\".\n", key, value, temp_value);
      } else {
        red_black_tree_insert(tree, key, value, compare_key, copy_key, copy_value, NULL);
      }
      break;
    case 3:
      // Ask user to search something
      printf("Please enter a key to search for in the tree.\n");
      input_string(key, sizeof(key));
      temp_value = red_black_tree_search(tree, key, compare_key, NULL);
      // If key exists in tree, ...
      if (temp_value != NULL) {
        // Search and print key and value of predecessor, if it exists
        printf("The tree contains the key \"%s\" with the associated value \"%s\".\n", key, temp_value);
        red_black_tree_predecessor((void **) &temp_key, (void **) &temp_value, tree, key, compare_key, NULL);
        if ((temp_key == NULL) || (temp_value == NULL)) {
          printf("The key \"%s\" does not have a predecessor in the tree.\n", key);
        } else {
          printf("The key \"%s\" has the predecessor key \"%s\" with value \"%s\".\n", key, temp_key, temp_value);
        }
        // Search and print key and value of successor, if it exists
        red_black_tree_successor((void **) &temp_key, (void **) &temp_value, tree, key, compare_key, NULL);
        if ((temp_key == NULL) || (temp_value == NULL)) {
          printf("The key \"%s\" does not have a successor in the tree.\n", key);
        } else {
          printf("The key \"%s\" has the successor key \"%s\" with value \"%s\".\n", key, temp_key, temp_value);
        }
      } else {
        printf("The tree does not contain an entry with key \"%s\".\n", key);
      }
      break;
    case 4:
      // Ask user to delete something or <N> to skip
      printf("Please enter a key to delete from the tree. Enter <N> to delete nothing.\n");
      input_string(key, sizeof(key));
      if (strcmp(key, "<N>") != 0) {
        red_black_tree_remove(tree, key, compare_key, delete_key, delete_value, NULL);
      }
      break;
    case 5:
      print2D(tree, /*print_all = */1);
      break;
    case 6:
    default:
    ;
    }
  }

  // Delete everything from tree
  red_black_tree_delete(tree, delete_key, delete_value, NULL);
}

static char *rand_string(char *str, size_t size)
{
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      int key = rand() % (int) (sizeof charset - 1);
      str[n] = charset[key];
    }
    str[size] = '\0';
  }

  return str;
}

static void random_generation(void)
{
  const int BUFFER_LEN = 8;
  const int MAX_SHIFTS = 15;
  int n_keys;
  char keys[2<<MAX_SHIFTS][BUFFER_LEN];
  char value[BUFFER_LEN];
  char key[BUFFER_LEN];

  clock_t t, rbt_time, bst_time;
  int shifts, i;

  red_black_tree_t *rbt;
  search_tree_t *bst;

  rbt = red_black_tree_create();
  bst = search_tree_create();

  printf("n_keys,insert_rbt,insert_bst,search_existent,serch_non_existent,remove_keys_rbt,remove_keys_bst\n");

  for (shifts = 2; shifts <= MAX_SHIFTS; ++shifts) {
    n_keys = 1 << shifts;
    printf("%d,", n_keys);

    /* Create red-black tree and bst with n_keys. At the end, print
     * time elapse to make all n_keys insertions per tree.
     */
    rbt_time = bst_time = 0;
    for (i = 0; i < n_keys; ++i) {
      rand_string(keys[i], BUFFER_LEN);

      if (red_black_tree_search(rbt, keys[i], compare_key, NULL) == NULL) {
        rand_string(value, BUFFER_LEN);

        t = clock();
        red_black_tree_insert(rbt, keys[i], value, compare_key, copy_key, copy_value, NULL);
        rbt_time += (clock() - t);

        t = clock();
        search_tree_insert(bst, keys[i], value, compare_key, copy_key, copy_value, NULL);
        bst_time += (clock() - t);

        if (red_black_tree_is_balanced(rbt) < 0) {
          printf("---\n");
          fprintf(stderr, "WARNING: tree is not balanced\n");
          print2D(rbt, /*print_all = */0);
          printf("---\n");
        }
      }
    }
    printf("%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    printf("%f,", ((double) bst_time)/CLOCKS_PER_SEC);

    // Perform search for all n_keys
    rbt_time = bst_time = 0;
    for (i = 0; i < n_keys; ++i) {
      t = clock();
      red_black_tree_search(rbt, keys[i], compare_key, NULL);
      rbt_time += (clock() - t);

      t = clock();
      search_tree_search(bst, keys[i], compare_key, NULL);
      bst_time += (clock() - t);
    }
    printf("%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    printf("%f,", ((double) bst_time)/CLOCKS_PER_SEC);

    // Perform search for n_keys keys that are not in tree
    rbt_time = bst_time = 0;
    for (i = 0; i < n_keys; ++i) {
      rand_string(key, BUFFER_LEN);

      t = clock();
      red_black_tree_search(rbt, key, compare_key, NULL);
      rbt_time += (clock() - t);

      t = clock();
      search_tree_search(bst, key, compare_key, NULL);
      bst_time += (clock() - t);
    }
    printf("%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    printf("%f,", ((double) bst_time)/CLOCKS_PER_SEC);

    // Delete one key at a time for each tree and print total time
    rbt_time = bst_time = 0;
    for (i = 0; i < n_keys; ++i) {
      t = clock();
      red_black_tree_remove(rbt, keys[i], compare_key, delete_key, delete_value, NULL);
      rbt_time += (clock() - t);

      t = clock();
      search_tree_remove(bst, keys[i], compare_key, delete_key, delete_value, NULL);
      bst_time += (clock() - t);
    }
    printf("%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    printf("%f\n", ((double) bst_time)/CLOCKS_PER_SEC);
  }

  // Delete everything from tree
  red_black_tree_delete(rbt, delete_key, delete_value, NULL);
  search_tree_delete(bst, delete_key, delete_value, NULL);
}

int main(void)
{
  // rbt_menu();

  random_generation();

  return 0;
}

