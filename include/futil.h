
#ifndef FUTIL_H
#define FUTIL_H
#include "common.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// This is a definition for a function that will return the filename of the file inside the filepath.
// For example: /home/main/file.txt = 'file.txt'.
char *basename(const char *fpathlike);

// This is a definition for a function that will check if there is a new line character '\n':
// 1. Return 1, if there is a new line character.
// 2. Return 0, if there is not a new line character.
int isnewline(int c);

// This is a definition for a function that will tokenize text inside a given file.
int ftokenize(
    FILE *f, // Point to the given file.
    list_t *list, // Add tokens last in the list, but add them in the order they appear inside the given file.
    size_t strlen_min, // Exclude tokens of a lenght that is lower than this.
    int (*csplitfn)(int), // This is a pointer to the function that is called on each character, split tokens if a value is non-zero.
    int (*cfilterfn)(int), // This function is called on each character if it is present, however, if the value is zero, exclude that token.
    
    // This function is called on each character if it is present,
    // The returned character is added to the token in place of the original character. Applied after filter, if present.
    int (*ctransformfn)(int)); 
    
#endif /* End the head file */