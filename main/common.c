
#include "common.h"
#include <string.h>
#include <stdio.h>

// This is a comparison function that will compare two integers.
int intcmp(const int *a, const int *b) {
    return *a - *b;
}

// This is a comparison function that will compare two characters.
int charcmp(const char *a, const char *b) {
    return (int) (*a - *b);
}

// This function will return the filename of the file inside the filepath.
// For example: /home/main/file.txt = 'file.txt'.
char *basename(const char *fpathlike) {
    char *s = strrchr(fpathlike, '/');

    if (s && ++s) {
        return s;
    }
    return (char *) fpathlike;
}