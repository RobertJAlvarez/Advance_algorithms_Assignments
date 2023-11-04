#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
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

static void print_menu(void)
{
  printf("1. Print tree characteristics.\n");
  printf("2. Insert new node.\n");
  printf("3. Search for node.\n");
  printf("4. Delete node.\n");
  printf("5. Print tree.\n");
  printf("6. Exit\n");
}

static void menu(void)
{
  char key[LINE_BUFFER_LEN];
  char value[LINE_BUFFER_LEN];
  char *temp_key, *temp_value;
  red_black_tree_t *tree;

  tree = red_black_tree_create();

  while (1) {
    print_menu();

    input_string(key, sizeof(key));
    if (key[0] == '6') break;

    switch (atoi(key)) {
    case 1:
      printf("The current search tree has %zu entries.\n", red_black_tree_number_entries(tree));
      printf("The current search tree has height %zu.\n", red_black_tree_height(tree));
      if (is_balanced(tree) < 0) {
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
  const int BUFFER_LEN = 6;
  char key[BUFFER_LEN];
  char value[BUFFER_LEN];
  red_black_tree_t *tree;

  tree = red_black_tree_create();

  for (int i = 1; i < 200; ++i) {
    rand_string(key, BUFFER_LEN);

    if (red_black_tree_search(tree, key, compare_key, NULL) == NULL) {
      rand_string(value, BUFFER_LEN);
      red_black_tree_insert(tree, key, value, compare_key, copy_key, copy_value, NULL);

      if (is_balanced(tree) < 0) {
        printf("---\n");
        fprintf(stderr, "WARNING: tree is not balanced\n");
        print2D(tree, /*print_all = */0);
        printf("---\n");
      }
    }
  }

  // Delete everything from tree
  red_black_tree_delete(tree, delete_key, delete_value, NULL);
}

int main(void)
{
  // menu();

  random_generation();

  return 0;
}

