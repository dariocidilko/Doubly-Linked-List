
#ifndef LIST_H
#define LIST_H
#include "common.h"
#include <stdlib.h>

// This is a struct for the list.
struct list;

// Use 'list_t' as an alias for struct list.
typedef struct list list_t;

// This is a struct for the list iterator, and use 'list_iter_t' as the alias.
typedef struct list_iter list_iter_t;

// This is a definition for a function that will create a new and empty list. 
// This function will use a comparison function to compare list items in relevant functions.
list_t *list_create(cmp_fn cmpfn);

// This is a definition for a function that will destroy a list and its items.
void list_destroy(list_t *list, free_fn item_free);

// This is a definition for a function to get the number of items inside the list. (Get the lenght of the list.)
size_t list_length(list_t *list);

// This is a definition for a function that will add an item to the start of the list.
int list_addfirst(list_t *list, void *item);

// This is a definition for a function that will add an item to the end of the list.
int list_addlast(list_t *list, void *item);

// This is a definition for a function to remove the first item from the list.
void *list_popfirst(list_t *list);

// This is a definition for a function to remove the last item from the list.
void *list_poplast(list_t *list);

// This is a definition for a function to search for an item that is inside the list. (If the item is inside the list.)
int list_contains(list_t *list, void *item);

// This is a definition for a function that will sort the entire list. 
// It will sort the list by using the comparison function of the list to determine the ordering of the items.
void list_sort(list_t *list);

// This is a definition for a function that will create an list iterator.
list_iter_t *list_createiter(list_t *list);

// This is a definition for a function that will destroy the iterator.
void list_destroyiter(list_iter_t *iter);

// This is a definition for a function that will check if the given list iterator has reached the end of the list.
// It does this by seeing if there are any items next, if not then this is the last item.
int list_hasnext(list_iter_t *iter);

// This is a definition for a function that will get the next item from the list.
void *list_next(list_iter_t *iter);

// This is a definition for a function that will reset the iterator to be on the first item in the list.
void list_resetiter(list_iter_t *iter);

#endif /* End the head file */