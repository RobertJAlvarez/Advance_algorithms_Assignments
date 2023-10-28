#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "linkedlists.h"
#include "hash.h"
#include "hashtable.h"

#define HASHTABLE_SIZE     (((size_t) 1) << 4)
#define LINE_BUFFER_LEN    ((size_t) 4096)

static void error_no_mem(void)
{
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

static void delete_list_entry(void *entry, void *data)
{
  char *pvt_entry = entry;

  free(pvt_entry);
}

static void delete_key(void *key, void *data)
{
  char *pvt_key = key;

  free(pvt_key);
}

static void delete_value(void *value, void *data)
{
  list_t *pvt_value = value;

  delete_list(pvt_value, delete_list_entry, NULL);
}

static void *copy_english_word(void *word, void *data)
{
  char *pvt_word = word;
  size_t len;
  char *copied_word;

  len = strlen(pvt_word);
  copied_word = (char *) calloc(len + ((size_t) 1), sizeof(*copied_word));
  if (copied_word == NULL) error_no_mem();
  strcpy(copied_word, pvt_word);

  return copied_word;
}

static list_t *read_english_meanings(char *english_words)
{
  list_t *list;
  char *head;
  char *tail;

  list = create_list();

  tail = english_words;
  for (;;) {
    head = tail;
    if (*head == '\0') break;
    tail = strchr(head, ',');
    if (tail == NULL) {
      tail = "";
    } else {
      *tail = '\0';
      tail++;
    }
    if (*head == ' ') head++;
    append_to_list(list, head, copy_english_word, NULL);
  }

  return list;
}

static uint32_t hash_key(void *key, void *data)
{
  char *pvt_key = key;
  
  return hash_str(pvt_key);
}

static void *copy_key(void *key, void *data)
{
  char *pvt_key = key;
  size_t len;
  char *copied_key;

  len = strlen(pvt_key);
  copied_key = (char *) calloc(len + ((size_t) 1), sizeof(*copied_key));
  if (copied_key == NULL) error_no_mem();
  strcpy(copied_key, pvt_key);

  return copied_key;
}

static void *copy_value(void *value, void *data)
{
  return value;
}

static int read_dictionary_line(hashtable_t *hashtable, char *line)
{
  char *spanish_word;
  char *english_words;
  list_t *english_meanings;

  spanish_word = line;
  english_words = strchr(line, '|');
  if (english_words == NULL) return -1;
  *english_words = '\0';
  english_words++;
  if (strlen(spanish_word) == ((size_t) 0)) return -1;
  if (strlen(english_words) == ((size_t) 0)) return -1;

  english_meanings = read_english_meanings(english_words);

  add_to_hashtable(hashtable,
       spanish_word,
       english_meanings,
       copy_key,
       copy_value,
       hash_key,
       NULL);

  return 0;
}

static int read_dictionary_file(hashtable_t *hashtable, char *filename)
{
  FILE *file;
  size_t pos;
  char line[LINE_BUFFER_LEN];
  int c;
  char ch;

  file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\" for reading: %s\n", filename, strerror(errno));
    return -1;
  }

  memset(line, '\0', sizeof(line));
  pos = (size_t) 0;
  while ((c = fgetc(file)) != EOF) {
    *((unsigned char *) &ch) = (unsigned char) c;
    if (c == '\n') {
      if (read_dictionary_line(hashtable, line) < 0) {
        fprintf(stderr, "Could not add line \"%s\" from file \"%s\" to dictionary\n", line, filename);
        if (fclose(file) != 0) {
          fprintf(stderr, "Could not close file \"%s\": %s\n", filename, strerror(errno));
        }
        return -1;
      }
      memset(line, '\0', sizeof(line));
      pos = (size_t) 0;
    } else {
      if (pos < (sizeof(line) - ((size_t) 1))) {
        line[pos] = ch;
      } else {
        fprintf(stderr, "Could not read a line completely from file \"%s\"\n", filename);
        if (fclose(file) != 0) {
          fprintf(stderr, "Could not close file \"%s\": %s\n", filename, strerror(errno));
        }
        return -1;
      }
      pos++;
    }
  }
 
  if (fclose(file) != 0) {
    fprintf(stderr, "Could not close file \"%s\": %s\n", filename, strerror(errno));
    return -1;
  }

  return 0;
}

int main(int argc, char **argv)
{
  hashtable_t *hashtable;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <dictionary file>\n", ((argc > 0) ? argv[0] : "dictionary"));
    exit(1);
  }

  hashtable = create_hashtable(HASHTABLE_SIZE);

  printf("&hashtable = %p\n", &hashtable);
  printf("hashtable = %p\n", hashtable);

  if (read_dictionary_file(hashtable, argv[1]) < 0) {
    delete_hashtable(hashtable, delete_key, delete_value, NULL);
    return 1;
  }

  delete_hashtable(hashtable, delete_key, delete_value, NULL);

  return 0;
}

