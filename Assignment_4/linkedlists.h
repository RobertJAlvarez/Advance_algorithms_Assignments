#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

#include <stdlib.h>

typedef struct __node_struct_t {
  void *data;
  struct __node_struct_t *prev;
  struct __node_struct_t *next;
} node_t;

typedef struct __list_struct_t {
  node_t *head;
  node_t *tail;
} list_t;

/* Creates a new empty list with no elements 

   O(1)
*/
list_t *create_list(void);

/* Deletes a list, freeing all memory, calling the function 
   in argument on all elements in order to free them

   O(n)
*/
void delete_list(list_t *, void (*)(void *, void *), void *);

/* Returns 0 if the list is not empty, non-zero otherwise 

   O(1)
*/ 
int is_empty_list(list_t *);

/* Returns the length of the list 

   O(n)
*/
size_t length_list(list_t *);

/* Iterates over all elements of the list, 
   calling the function in argument on each element.

   O(n) 
*/
void iterate_over_list(list_t *, void (*)(void *, void *), void *);

/* Searches the list for an element, comparing 
   with the function in argument.

   The function in argument must return 0 if the 
   elements are indeed equal.

   Returns the first element that is found 
   equal.

   Returns NULL if no element matches.

   O(n)
*/
void *search_list(list_t *, void *, int (*)(void *, void *, void *), void *);

/* Returns the i-th element of a list.

   Returns NULL if the list does not have an i-th element.

   O(n)
*/
void *get_ith_element_of_list(list_t *, size_t);

/* Modifies a list so that a given element is its new
   first element. 

   Calls the function in argument to copy the element.

   O(1)
*/
void prepend_to_list(list_t *, void *, void *(*)(void *, void *), void *);

/* Modifies a list so that a given element is its new
   last element. 

   Calls the function in argument to copy the element.

   O(1)
*/
void append_to_list(list_t *, void *, void *(*)(void *, void *), void *);

#endif
