
#include "futil.h"
#include "common.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// This represents the initial buffer size (256 bytes in hexadecimal), and must be postive and larger than zero.
#define INITIAL_BUFSIZE 0x100

// This will check if a integer (c) is equal to a new line character '\n'.
// 1. Return 1, if integer (c) is equal to to the new line character '\n'.
// 2. Return 0, if integer (c) is NOT equal to to the new line character '\n'.
inline int isnewline(int c) {
    return (c == '\n');
}

// This function will tokenize text inside a given file. (Every parameter is explained inside 'futil.h'.)
int ftokenize(FILE *f, list_t *list, size_t strlen_min, int (*csplitfn)(int), int (*cfilterfn)(int), int (*ctransformfn)(int)) {
    
    // If the file is NULL, return -1.
    if (f == NULL) {
        printf("Error: The file pointer is NULL. \n");
        return -1;
    }

    int rv = 0; // Integer for the 'return value'.
    size_t list_len_before = list_length(list); // Check the list length of the list before the tokenization.
    size_t bufsize = INITIAL_BUFSIZE; // Set the buffer size to the initial buffer size. (256 bytes.)

    // Allocate memory for a temporary buffer.
    char *buffer = malloc(bufsize);
    
    // Check if the memory allocation for the temporary buffer failed.
    if (buffer == NULL) {
        printf("Error: Memory could not be allocated for the temporary buffer. \n");
        return -1;
    }

    char *s = buffer; // Set 's' to point to the start of the buffer.
    size_t len = 0; // Initialize the length of the token stored inside the buffer to be zero.

    // While the end of the file has NOT been reached, run the code inside the loop.
    while (!feof(f)) {
        
        // Read the next character from the file, that character is assigned as 'c'.
        int c = fgetc(f);

        // If the end of the file has been reached, set the integer 'c' as the return value.
        if (c < EOF) {
            printf("Error: Failed to read from the file. \n");
            rv = c;
            break;
        }

        // If the end of the file is reached or check if a split function is provided and whether it returns a non-zero value.
        if (c == EOF || (csplitfn && csplitfn(c))) {
            
            // If the length is bigger or equal to the string length minumum, set the pointer to point to the start again.
            if (len >= strlen_min) {
                *s = 0;
                
                // Create (Copy) a new string with the content of the buffer.
                char *cpy = strdup(buffer);
                
                // If the copying failed, return -1 as the return value.
                if (cpy == NULL) {
                    printf("Error: Failed to allocate memory for the copy of a new string. \n");
                    rv = -1;
                    break;
                }

                // Add the copied string as the return value and add it last inside the list.
                rv = list_addlast(list, cpy);

                // If the return value is less than zero, free the memory allocated for the copied string.
                if (rv < 0) {
                    printf("Error: Adding the copied string last inside the list failed. \n");
                    free(cpy); // Free the memory for the copied string.
                    break;
                }
            }

            s = buffer; // Reset the 's' to point to the start of the buffer.
            len = 0; // Reinitialize the length of the token stored inside the buffer to be zero.
        } 

        // Check whether the character 'c' should be included in the token.
        else if (cfilterfn == NULL || cfilterfn(c)) {
            
            // This will transform the character 'c' to a token if a transformation function is provided.
            *s = ctransformfn ? ctransformfn(c) : c;
            len++;
            s++;

            // If the length is equal to the buffer size - 1, procced.
            if (len == bufsize - 1) {
                
                // The buffer is full, so double its size to make room for more characters as one byte is needed for null-terminator.
                bufsize *= 2;
                
                // Reallocate the memory for a buffer double its size for a buffer named 're-buf'.
                char *re_buf = realloc(buffer, bufsize);
                
                // Check if the memory allocation failed for the double buffer size.
                if (re_buf == NULL) {
                    printf("Error: Failed to reallocate memory for the buffer: %s\n", strerror(errno));
                    rv = -1;
                    break;
                }

                buffer = re_buf;
            }
        }
    }

    free(buffer); // Free the memory allocated for the buffer.

    // Either complete the operation or revert the list state if it failed.
    if (rv < 0) {

        // While the list length is bigger then the list length before the operation, free the tokens by using 'list_poplast'.
        while (list_length(list) > list_len_before) {
            free(list_poplast(list));
        }
    } 
    
    // If the list length is equal to the list length before the operation, print a warning.
    else if (list_len_before == list_length(list)) {
        printf("The list of words is empty. If the file contains tokens, 'list_addlast' is not working. \n");
    }

    return rv;
}