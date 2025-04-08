
#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <stdio.h>

// This is a definition for a comparison function, that will return:
// 1. 0 if two items are equal,
// 2. > 0 if (a > b),
// 3. < 0 if (a < b).
typedef int (*cmp_fn)(const void *, const void *);

// This is a definition for a function that will deallocate resources and free memory.
typedef void (*free_fn)(void *);

// This is a definition for a comparison function, that will compare two integers.
int intcmp(const int *a, const int *b);

// This is a definition for a comparison function, that will compare two characters.
int charcmp(const char *a, const char *b);

#endif /* End the head file */