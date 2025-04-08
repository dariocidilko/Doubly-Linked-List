
#include "common.h"
#include "futil.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// This is a struct that represents a single word-frequency pair. The alias is 'word_freq_t'.
typedef struct word_freq {
    char *word; // This is a pointer to the word.
    size_t count; // This is how many times that word appears.
} word_freq_t;

// This is a function to sort the 'word_freq_t'.
int compare_word_freq_by_count(word_freq_t *a, word_freq_t *b) {
    
    // If the counter to 'a' is larger than the counter to 'b', return -1. 
    if (a->count > b->count) {
        return -1;
    }

    // If the counter to 'a' is smaller than the counter to 'b', return 1. 
    if (a->count < b->count) {
        return 1;
    }

    return 0;
}

// Free the 'word_freq_t', deallocate its memory.
void word_freq_free(word_freq_t *freq) {
    free(freq->word);
    free(freq);
}

// This is where a list is created to hold word-frequency pairs.
static list_t *create_wordfreqs_list(list_t *words) {

    // Call the 'list_create' function to create a new list.
    list_t *freqs = list_create((cmp_fn) compare_word_freq_by_count);

    // Check if the list was created successfully.
    if (freqs == NULL) {
        printf("Error: Failed to create a list for the frequency pairs. \n");
        goto err_cleanup;
    }

    // Create a iterator to iterate over the words inside the list.
    list_iter_t *words_iter = list_createiter(words);

    // Check if the iterator for the words was created successfully.
    if (words_iter == NULL) {
        printf("Error: Failed to create list iterator for the frequency pairs. \n");
        goto err_cleanup;
    }

    word_freq_t *freq = NULL;

    // Call 'list_hasnext' and while there are next nodes, keep running the loop.
    while (list_hasnext(words_iter)) {

        // Get the next word from the iterator.
        char *word = list_next(words_iter);

        // If 'freq' is not NULL and the word in 'freq' matches the current word:
        if (freq && strcmp(freq->word, word) == 0) {
            freq->count++; // Increment the count of that word.
            continue;
        }

        // Allocate memory for a new word-frequency pair.
        freq = malloc(sizeof(word_freq_t));
        if (freq == NULL) {
            printf("Error: Cannot allocate memory for a new wor-frequency pair. \n");
            goto err_cleanup;
        }

        freq->count = 1; // Initialize the frequency count of the new word to 1, because its the first time the word appears.
        freq->word = strdup(word); // Duplicate the word. (Allocate memory and copy the word.)

        // Check if the memory allocation for the word failed.
        if (freq->word == NULL) {
            printf("Cannot allocate memory\n");
            free(freq);
            goto err_cleanup;
        }

        // Add the newly created word-frequency pair first in the 'freqs' list.
        if (list_addfirst(freqs, freq) < 0) {
            printf("Error: 'list_addfirst' failed. \n");
            word_freq_free(freq); // Free the memory for 'freq'.
            goto err_cleanup;
        }
    }

    list_destroyiter(words_iter); // Free the iterator.

    // Sort the list.
    list_sort(freqs);

    return freqs;

// This is a cleanup function that will handle memory deallocation. (This is where almost every 'if-statement' goes to.)
err_cleanup:

    // If there is a list, destroy it and free its memory.
    if (freqs) {
        list_destroy(freqs, (free_fn) word_freq_free);
    }

    return NULL;
}

// This is a function that will print out the word frequency list, shows the result.
static int print_wordfreqs_list(list_t *freqs, size_t min_wc, size_t lim_nres) {
    
    // Create a iterator, so that we can display the results.
    list_iter_t *freqs_iter = list_createiter(freqs);
    
    // Check if the memory allocation was successful.
    if (freqs_iter == NULL) {
        printf("Error: Failed to create an iterator. \n");
        return -1;
    }

    /* --- These are all of the prints required to display the results in command prompt. */

    printf("Number of distinct words: %zu\n\n", list_length(freqs));

    printf("--- Words that occured at least %zu times", min_wc);
    
    if (lim_nres) {
        printf("Error: Limiting to max %zu results. \n", lim_nres);
    }

    printf(" ---\n");

    printf("%-30s   %s\n", "TERM", "COUNT");

    size_t n_printed = 0; // Initilize the printed count.

    // This is a loop required to print out the results to the command prompt:
    while (list_hasnext(freqs_iter) && (lim_nres == 0 || n_printed < lim_nres)) {
        word_freq_t *freq = list_next(freqs_iter);
        if (freq->count >= min_wc) {
            printf("%-30s | %zu\n", freq->word, freq->count);
            n_printed++;
        }
    }

    list_destroyiter(freqs_iter);
    return 0;
}

// This is a function that will print out how to use the arguments and the program, incase someone fails.
static int parse_args(int argc, char **argv, char **fpath, size_t *min_wc, size_t *min_wl, size_t *lim_nres) {
   
    // Check if the argument count is exactly 5.
    if (argc != 5) {

        // These are just all of the print statements that will show up as a guide.
        printf("Error: Missing one or more required positional arguments. \n");
        fprintf(stderr, "Usage: ./%s <fpath> <min_wc> <min_wl> <lim_n_results>\n", basename(argv[0]));
        fprintf(stderr, "* <fpath>: Path to a readable file. The file will never be modified. \n");
        fprintf(stderr, "* <min_wc>: Exclude words that occur less times than this value. 1 to include all. \n");
        fprintf(stderr, "* <min_wl>: Exclude words shorter than this value. 1 to include all. \n");
        fprintf(stderr, "* <lim_n_results>: Print at most this many results. 0 to print all. \n");
        fprintf(stderr, "--- \n");
        fprintf(stderr, "Example 1: %s src/wordfreq.c 10 2 10 \n", argv[0]);
        fprintf(stderr, "Example 2: %s data/oxford_dict.txt 1 13 25 \n", argv[0]);
        fprintf(stderr, "Example 3: make run ARGS=\"data/oxford_dict.txt 100 4 25\" \n");
        return -1;
    }

    errno = 0; // Reset the error to 0 to clear any previous errors.

    // Convert the second argument (min_wc) from a string to long.
    long min_wc_ = strtol(argv[2], NULL, 10);
    if (errno) {
        printf("Error: Bad argument \"%s\" for <min_wc>: %s\n", argv[2], strerror(errno));
        return -1;
    }

    // Convert the third argument (min_wl) from a string to long.
    long min_wl_ = strtol(argv[3], NULL, 10);
    if (errno) {
        printf("Error: Bad argument \"%s\" for <min_wl>: %s\n", argv[3], strerror(errno));
        return -1;
    }

    // Convert the fourth argument (lim_nres) from a string to long.
    long lim_nres_ = strtol(argv[4], NULL, 10);
    if (errno) {
        printf("Error: Bad argument \"%s\" for <lim_n_results>: %s\n", argv[4], strerror(errno));
        return -2;
    }

    // Assign the file path to the fpath pointer.
    *fpath = argv[1];

    // Ensure that min_wc is at least 1, otherwise set it to 1.
    *min_wc = (min_wc_ < 1) ? 1 : (size_t) min_wc_;

    // Ensure that min_wl is at least 1, otherwise set it to 1.
    *min_wl = (min_wl_ < 1) ? 1 : (size_t) min_wl_;

    // Ensure that lim_nres is non-negative, otherwise set it to 0.
    *lim_nres = (lim_nres_ < 0) ? 0 : (size_t) lim_nres_;

    return 0;
}

// This is the main function.
int main(int argc, char **argv) {
    
    char *fpath;
    
    // Declare the size of 'min_wc', 'min_wl' and 'lim_nres'.
    size_t min_wc, min_wl, lim_nres;

    // Parse the command line arguments and assign values to fpath, min_wc, min_wl, and lim_nres.
    // If parsing fails exit, the program.
    int rc = parse_args(argc, argv, &fpath, &min_wc, &min_wl, &lim_nres);
    
    // If 'rc' is less than 0, return.
    if (rc < 0) {
        return -1;
    }

    // Open the file given.
    FILE *infile = fopen(fpath, "r");
    
    // If file opening fails, print an error message and exit.
    if (!infile) { 
        printf("Error: Failed to open %s: %s\n", fpath, strerror(errno));
        return -1;
    }

    // Create a new list to store the words. (This is sorted.)
    list_t *words = list_create((cmp_fn) strcmp);

    // Check if the memory allocation failed.
    if (words == NULL) { 
        printf("Error: Failed to create the list for storing words. \n");
        fclose(infile); // Close the opened file to avoid resource leak.
        return -1;
    }

    // Tokenize the content of the file into words.
    rc = ftokenize(infile, words, min_wl, isspace, isalnum, tolower);
    
    // If tokenization succeeds and there are words in the list.
    if (rc >= 0 && list_length(words)) {
        
        // Sort the words in the list by calling the 'list_sort' function.
        list_sort(words);
    
        // Ensure the list is sorted correctly.
        list_iter_t *iter = list_createiter(words);
        
        if (iter) {
            // Get the first word from the list.
            void *prev = list_next(iter);

            // Iterate through the list and check if it is sorted.
            while (list_hasnext(iter)) {
                void *curr = list_next(iter);

                // If the list is not sorted, print an error.
                if (strcmp(prev, curr) > 0) {
                    printf("Error: The list is not sorted correctly. \n");
                    list_destroyiter(iter);
                    rc = -1;
                    break;
                }

                prev = curr; // Update previous to the current word for the next iteration.
            }

            list_destroyiter(iter); // Destroy the iterator.
        }
    
        // If no errors occurred during sorting.
        if (rc >= 0) {

            // Create the word-frequency list by counting word occurrences.
            list_t *freqs = create_wordfreqs_list(words);
    
            // If frequency list creation is successful.
            if (freqs) {

                // Print the header information about the file and word length requirements.
                printf("\n--- %s | Words consisting of at least %zu chars --- \n", basename(fpath), min_wl);
                printf("Total number of words: %zu\n", list_length(words));
    
                // Print the word frequencies.
                rc = print_wordfreqs_list(freqs, min_wc, lim_nres);

                // Free the frequency list memory.
                list_destroy(freqs, (free_fn) word_freq_free);
            } 
            else {
                rc = -1; // If frequency list creation failed, set 'rc' to -1 to mark it as failure.
            }
        }
    }

    fclose(infile); // Close the file after processing is complete.

    // Return success or failure based on the result code. (Rc)
    return rc < 0 ? EXIT_FAILURE : EXIT_SUCCESS; 
}