
#include "list.h"
#include <stdlib.h>

// Define a struct for the nodes inside the linked list.
typedef struct lnode lnode_t;

// This is the struct for the individual nodes.
struct lnode {
    lnode_t *next; // This is a pointer to the next node inside the list.
    lnode_t *prev; // This is a pointer to the previous node inside the list.
    void *item; // This makes it possible to store any item inside a node by using a pointer.
};

// This is a struct for the list.
struct list {
    lnode_t *head; // This is a pointer to the head (First node) of the list.
    lnode_t *tail; // This is a pointer to the tail (Last node) of the list.
    size_t length; // This is the size of the list, give the size by looking at its lenght. (How many nodes inside the list.)
    cmp_fn cmpfn; // This is the comparison function that will be used.
};

// This is a struct for the list iterators.
struct list_iter {
    list_t *list; // This is a pointer to the list being iterated over.
    lnode_t *node; // This is a pointer to the current node in the iteration.
};

// This is a function to create a new empty list.
list_t *list_create(cmp_fn cmpfn) {

    // Allocate memory for a new list data structure.
    list_t *list = (list_t*) malloc(sizeof(list_t));

    // Check if memory allocation is successful.
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL; // Set the first node to have a value of "NULL".
    list->tail = NULL; // Set the last node to have a value of "NULL".
    list->length = 0; // Start the list off by it having zero items inside.
    list->cmpfn = cmpfn;

    return list; // Return the list.    
}

// This is a function to destroy a list.
void list_destroy(list_t *list, free_fn item_free) {
    
    // Check if the list is empty, if so return.
    if (list == NULL) {
        return;
    }

    lnode_t *current = list->head; // Make the pointer point at the head of the list.
    while (current != NULL) {
        lnode_t *temp = current; // Store the current node in a temporary pointer.
        current = current->next; // Move to the next node while holding the data from the current pointer.

        // This will free the contents inside the node itself.
        if (item_free) {
            item_free(temp->item);
        }

        free(temp); // Free the previous node.
    }

    free(list); // This will free the list, since memory was allocated in 'list_create'.
}

// This is a function to get the length of a list. (By looking at how many nodes are inside the list.)
size_t list_length(list_t *list) {
    
    size_t count = 0; // Set the counter to start at zero.
    lnode_t *current = list->head; // Have a current pointer point to the head of the list, because we want to start at the first node.

    // While the node is not empty, count the node by incrementing the counter and go to the next node.
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Return the counter.
    return count;
}

// This is a function to add a element first inside the list.
int list_addfirst(list_t *list, void *item) {

    // Allocate memory for a new node, this new node will be the first.
    lnode_t *lnode = (lnode_t*)malloc(sizeof(lnode_t));

    // Check if memory allocation was successful, if not return -1.
    if (lnode == NULL) {
        return -1;
    }

    lnode->item = item; // Initilize the item inside the node.
    lnode->next = list->head; // Set the node to be the first in the list.
    lnode->prev = NULL; // There is no previous node, since this is the first.

    // If the list is empty, set the tail to be the new node.
    if (list->tail == NULL) {
        list->tail = lnode;
    }

    // If the list is not empty, move the pointer from the previous head node to the new node.
    else {
        list->head->prev = lnode;
    }

    list->head = lnode; // Set the list head to the new node.

    list->length++; // Increment the list to increase its length.
    return 0; // Return 0, because if the operation was successful, so was the memory allocation.
}

// This is a function to add a element last inside the list.
int list_addlast(list_t *list, void *item) {

    // Allocate memory for the new node.
    lnode_t *lnode = (lnode_t*)malloc(sizeof(lnode_t));

    // Check if the memory allocation failed.
    if (lnode == NULL) {
        return -1;
    }

    lnode->item = item; // Initilize the item inside the new node.
    lnode->next = NULL; // There is no next node, since this will be the tail.
    lnode->prev = list->tail; // This will be the last node.

    // If the list is empty (no last node), set the head node to be the new node.
    if (list->tail == NULL) {
        list->head = lnode;
    }
    // If the list is not empty, set the pointer of the previous tail to the new node.
    else {
        list->tail->next = lnode;
    }

    list->tail = lnode; // Set the tail to the new node.
    list->length++; // Increment the list lenght.
    return 0; // Since the operation was successful, the memory allocation was successfull too.
}

// This is a function to remove the first element inside the list.
void *list_popfirst(list_t *list) {
    
    // Check if the list is empty.
    if (list->head == NULL) {
        return NULL;
    }

    lnode_t *temp = list->head; // Make a temporary pointer that will point at the head node.
    void *item = temp->item; // Save the current data inside the current node.
    list->head = list->head->next; // Move the pointer from the head node to the next node.
    
    // If the list head is not empty, then set the previous head node to zero, thereby making the next node the head node.
    if (list->head != NULL) {
        list->head->prev = NULL; // If the list is not empty, set the previous head data to NULL. (Thereby removing it.)
    }
    else {
        list->tail = NULL; // If the head is empty, then so must the tail be, set it to NULL.
    }

    free(temp); // Free the node, aka. remove the head node.
    list->length--; // Decrement the list lenght by one.
    return item; // Return item.
}

// This is a function to remove the last element inside the list.
void *list_poplast(list_t *list) {
    
    // Checking if the list is empty before popping.
    if (list->head == NULL) {
        return NULL;
    }

    lnode_t *temp = list->tail; // Make a temporary pointer that will point to the tail of the list.
    void *item = temp->item; // Save the current data inside the node. (Tail node.)
    list->tail = list->tail->prev; // Update the pointer to move from the current tail node to the previous node. (The next tail.)

    // Check if the list is empty after removing the current tail.
    if (list->tail != NULL) {
        list->tail->next = NULL; // If it is not, remove the current tail node.
    }
    else {
        list->head = NULL; // If the tail become empty then so must the head.
    }

    free(temp); // Free the node.
    list->length--; // Decrement the length of the list by one.
    return item; // Return item.
}

// This is a function to check if a item is inside the list.
int list_contains(list_t *list, void *item) {
    
    // Check if the list is empty.
    if (list->head == NULL) {
        return 0;
    }

    lnode_t *temp = list->head; // Make a pointer that will point at the head of the list.
    
    // If the item inside the head node is the same as the item we are searching for return 0.
    if (list->cmpfn(temp->item, item) == 0) {
        return 1;
    }

    // While we have an item we are searching for.
    while (temp != NULL) {
        // If the item matches with the item inside the node, return 0.
        if (list->cmpfn(temp->item, item) == 0) {
            return 1;
        } 

        // Move to the next.
        temp = temp->next;
    }

    return 0; // Return 0 if nothing is found.
}

// This is a function to create a list iterator.
list_iter_t *list_createiter(list_t *list) {

    list_iter_t *iter = (list_iter_t *)malloc(sizeof(list_iter_t)); // Allocate memory for a new iterator.

    // Check if the memory for the iterator is allocated successfully.
    if (iter == NULL) {
        return NULL;
    }
    
    // Initilize the iterator.
    iter->list = list; 
    iter->node = list->head; // Make it start at the head node.

    return iter; // Return the iterator.
}

// This is a function to destroy an iterator.
void list_destroyiter(list_iter_t *iter) {

    free(iter); // Free the memory required for the iterator.
}

// This is a function to see if there is a node after the current node.
int list_hasnext(list_iter_t *iter) {

    // Check if the current node is NULL, if so return 0 because we have reached the end.
    if (iter->node == NULL) {
        return 0;
    }
    else {
        return 1; // Return 1 if otherwise.
    }
}

// This is a function to see if an item is inside the current node and move the iterator to the next node.
void *list_next(list_iter_t *iter) {

    // Check if node that the iterator is on exists, if it does not return NULL.
    if (iter->node == NULL) {
        return NULL;
    }

    // Store the item inside the current node, then move the iterator to the next node.
    void *item = iter->node->item;
    iter->node = iter->node->next;
    
    return item; // Return the item.    
}

// This is a function to reset the iterator to the head of the list.
void list_resetiter(list_iter_t *iter) {

    iter->node = iter->list->head; // Reset the iterator to the head node.
}

/* ---- MERGESOFT ALGORITHM ---- */

// CREDITS FOR THE MERGESOFT ALGORITHM, (Line 289 - 363): 
// 1. Odin Bjerke, <odin.bjerke@uit.no>
// 2. Morten Grønnesby, <morten.gronnesby@uit.no>

// This is the Mergesort function.
static lnode_t *merge(lnode_t *a, lnode_t *b, cmp_fn cmpfn) {
    
    // Initilize the pointer for the head and the tail node.
    lnode_t *head, *tail;

    // Choose between the smallest head node by using the comparison function.
    if (cmpfn(a->item, b->item) < 0) {
        head = tail = a;
        a = a->next;
    } 
    else {
        head = tail = b;
        b = b->next;
    }

    // Keep on repeatedly picking the smallest head node.
    while (a && b) {
        if (cmpfn(a->item, b->item) < 0) {
            tail->next = a;
            tail = a;
            a = a->next;
        } 
        else {
            tail->next = b;
            tail = b;
            b = b->next;
        }
    }

    // Append the remaining non-empty list. (If there are any.)
    if (a) {
        tail->next = a;
    } 
    else {
        tail->next = b;
    }

    return head;
}

// Split the given list in two halves and return the head of the second half.
static lnode_t *splitlist(lnode_t *head) {
    
    // Move two pointers, a 'slow' one and a 'fast' one which moves twice as fast.
    // When the fast one reaches the end of the list, the slow one will be at the middle.
    lnode_t *slow = head;
    lnode_t *fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Now split the list and return the second half.
    lnode_t *half = slow->next;
    slow->next = NULL;

    return half;
}

// This is recursive mergesort. 
// This function is named 'mergesort_' to avoid collision with the mergesort function that is defined by the standard library on some platforms.
static lnode_t *mergesort_(lnode_t *head, cmp_fn cmpfn) {
    
    // If there is no next node return this node since this will be the head node.
    if (head->next == NULL) {
        return head;
    }

    lnode_t *half = splitlist(head);
    head = mergesort_(head, cmpfn);
    half = mergesort_(half, cmpfn);

    return merge(head, half, cmpfn);
}

// This is a function to sort the entire list by using the Mergesort algorithm.
void list_sort(list_t *list) {
    
    // Recursively sort the list using the internal mergesort function.
    list->head = mergesort_(list->head, list->cmpfn);

    // Fix the tail and previous links.
    lnode_t *prev = NULL;
    
    for (lnode_t *n = list->head; n != NULL; n = n->next) {
        n->prev = prev;
        prev = n;
    }

    list->tail = prev; // Set the tail of the list to the last node.
}