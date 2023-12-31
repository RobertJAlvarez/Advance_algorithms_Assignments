#include <sys/stat.h>
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
  static const char charset[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_=+[]{}!@#$%^&*()|;:,<.>/?";

  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      str[n] = charset[rand() % ((int) (sizeof(charset) - 1))];
    }
    str[size] = '\0';
  }

  return str;
}

static void compare_rbt_bst_all_attributes(FILE *fd)
{
  const int BUFFER_LEN = 6;
  const int MAX_SHIFTS = 20;

  char value[BUFFER_LEN];
  char keys[1<<MAX_SHIFTS][BUFFER_LEN];
  char key[BUFFER_LEN];
  int n_keys;

  clock_t t, rbt_time, bst_time;
  int shifts, i;

  red_black_tree_t *rbt;
  search_tree_t *bst;

  rbt = red_black_tree_create();
  bst = search_tree_create();

  fprintf(fd, "n_keys,max_height_rbt,max_height_bst,insert_rbt,insert_bst,search_existent_rbt,search_existent_bst,serch_non_existent_rbt,serch_non_existent_bst,remove_keys_rbt,remove_keys_bst\n");

  for (shifts = 2; shifts <= MAX_SHIFTS; ++shifts) {
    n_keys = 1 << shifts;
    fprintf(fd, "%d,", n_keys);

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
      }
    }
    fprintf(fd, "%zu,", red_black_tree_height(rbt));
    fprintf(fd, "%zu,", search_tree_height(bst));
    fprintf(fd, "%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    fprintf(fd, "%f,", ((double) bst_time)/CLOCKS_PER_SEC);

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
    fprintf(fd, "%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    fprintf(fd, "%f,", ((double) bst_time)/CLOCKS_PER_SEC);

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
    fprintf(fd, "%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    fprintf(fd, "%f,", ((double) bst_time)/CLOCKS_PER_SEC);

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
    fprintf(fd, "%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    fprintf(fd, "%f\n", ((double) bst_time)/CLOCKS_PER_SEC);
  }

  // Delete everything from tree
  red_black_tree_delete(rbt, delete_key, delete_value, NULL);
  search_tree_delete(bst, delete_key, delete_value, NULL);
}

static void compare_rbt_bst(FILE *fd)
{
  const int BUFFER_LEN = 6;
  const int MAX_SHIFTS = 22;

  char value[BUFFER_LEN];
  char key[BUFFER_LEN];
  int n_keys;

  clock_t t, rbt_time, bst_time;
  int shifts, i;

  red_black_tree_t *rbt;
  search_tree_t *bst;

  rbt = red_black_tree_create();
  bst = search_tree_create();

  fprintf(fd, "n_keys,max_height_rbt,max_height_bst,insert_rbt,insert_bst\n");

  for (shifts = 2; shifts <= MAX_SHIFTS; ++shifts) {
    n_keys = 1 << shifts;
    fprintf(fd, "%d,", n_keys);

    rbt_time = bst_time = 0;
    for (i = 0; i < n_keys; ++i) {
      rand_string(key, BUFFER_LEN);

      if (red_black_tree_search(rbt, key, compare_key, NULL) == NULL) {
        rand_string(value, BUFFER_LEN);

        t = clock();
        red_black_tree_insert(rbt, key, value, compare_key, copy_key, copy_value, NULL);
        rbt_time += (clock() - t);

        t = clock();
        search_tree_insert(bst, key, value, compare_key, copy_key, copy_value, NULL);
        bst_time += (clock() - t);
      }
    }
    fprintf(fd, "%zu,", red_black_tree_height(rbt));
    fprintf(fd, "%zu,", search_tree_height(bst));
    fprintf(fd, "%f,", ((double) rbt_time)/CLOCKS_PER_SEC);
    fprintf(fd, "%f\n", ((double) bst_time)/CLOCKS_PER_SEC);
  }

  // Delete everything from tree
  red_black_tree_delete(rbt, delete_key, delete_value, NULL);
  search_tree_delete(bst, delete_key, delete_value, NULL);
}

static void print_wait_message(int n)
{
  int j, k;

  printf("Running %d", n);

  j = n % 10;
  k = n % 100;

  if (j == 1 && k != 11) printf("st");
  else if (j == 2 && k != 12) printf("nd");
  else if (j == 3 && k != 13) printf("rd");
  else printf("th");

  printf(" comparison...\n");
}

static void random_generation(void)
{
  const int N_FILES = 10;
  FILE *fptr;
  char all_filename[]  = "all_attributes_results/result**.csv";
  char some_filename[] = "some_attributes_results/result**.csv";
  char *t;
  int move_files;

  if ((move_files = mkdir("all_attributes_results", 0744)) != 0) {
    fprintf(stderr, "Error creating \"all_attributes_results\" folder, %d\n", errno);
    printf("Note: Make sure that \"all_attributes_results\" folder doesn't exists before running this code.\n");
    exit(1);
  }

  if ((move_files = mkdir("some_attributes_results", 0744)) != 0) {
    fprintf(stderr, "Error creating \"some_attributes_results\" folder, %d\n", errno);
    printf("Note: Make sure that \"some_attributes_results\" folder doesn't exists before running this code.\n");
    exit(1);
  }

  for (int i = 1; i <= N_FILES; ++i) {
    srand(time(NULL));

    // All attributes
    t = &all_filename[29] + sprintf(&all_filename[29], "%d", i);
    sprintf(t, "%s", ".csv");
    fptr = fopen(all_filename, "w");
    print_wait_message(i);
    compare_rbt_bst_all_attributes(fptr);
    fclose(fptr);

    // Some attributes
    t = &some_filename[30] + sprintf(&some_filename[30], "%d", i);
    sprintf(t, "%s", ".csv");
    fptr = fopen(some_filename, "w");
    print_wait_message(i);
    compare_rbt_bst(fptr);
    fclose(fptr);
  }
}

int main(void)
{
  // rbt_menu();

  random_generation();

  return 0;
}

