#include <stdio.h>
#include <stdlib.h>

// NOTE: For now, the number of words & maximum lengths of each word is predefined.
// TODO: Dynamically reallocate 2D array as file is parsed word-by-word (move semantics instead of copying?!)
// TODO:   ...and figure out if lengths of each dimension of array are necessary (possibly get away with just checking for null-terminating character?)

const int NUMBER_OF_WORDS = 7;
const int MAX_WORD_LENGTH = 20;

int ERROR_FLAG = 0; // Let ERROR_FLAG = 0 when no NULL exceptions occur, and let ERROR_FLAG = 1 when NULL exceptions occur.
// TODO: Substitute ERROR_FLAG for exception handling if it exists in some capacity in C.

char** readFile(char* fileLocation) {
    FILE* fp = NULL;

    // NOTE: fopen looks for the file in one of two ways:
    //      1) An explicit path to the file (e.g. "C:\wordlist.txt")
    //      2) An implicit search in the current execution folder of the binary through the dot qualifier (e.g. ".\wordlist.txt")
    //      You will need to copy the supplied wordlist.txt file to wherever it is convenient
    //            (either in your clone of the cmake-build folder for an implicit search,
    //             or to anywhere else for an explicit search).
    //  TODO: Don't crash program if fileLocation not found...
    fp = fopen(fileLocation, "r");

    // TODO: Potentially replace with single-line malloc/free & use pointer arithmetic for navigation.
    // TODO: Determine the effects of having a 2D array allocated as such being utilized in subscript format in other functions (is it even possible?)
    char** wordArray = (char**)malloc(NUMBER_OF_WORDS * sizeof(char*));
    for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
        wordArray[i] = (char*)malloc(MAX_WORD_LENGTH * sizeof(char));
    }

    int i = 0;
    while (!feof(fp) && i < NUMBER_OF_WORDS) {
        if (fscanf(fp, "%s", wordArray[i]) != 1) {
            break;
        }
        ++i;
    }

    fclose(fp);

    return wordArray;
}

/* USAGE NOTES:
 *  Two arguments will be handled by main:
 *      1) Path to binary after file is built (i.e. simply launching the program produces an argument)
 *      2) Path to text file containing space- or newline-delimited words.
 */
int main(int argc, char *argv[]) {
    printf("Number of arguments: %d\n", argc);
    if (argc != 2) {
        printf("ERROR: Illegal number of arguments.\n");
        exit(1);
    }
    char** returnedArray = NULL;
    returnedArray = readFile(argv[1]);

    if (returnedArray != NULL) {
        for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
            if (returnedArray[i] != NULL) {
                printf("%s\n", returnedArray[i]);
            }
            else {
                ERROR_FLAG = 1;
                break;
            }
        }

        if (ERROR_FLAG == 0) {
            // INSERT ARRAY PARSING FUNCTIONALITY HERE (use predefined globals for now to determine each dimension's length, if necessary)
        }
    }
    else {
        ERROR_FLAG = 1;
    }

    if (ERROR_FLAG == 1) {
        printf("ERROR: Either returnedArray itself is NULL, or one of its pointers is NULL.");
    }

    // TODO: Replace with the TODO's noted in the readFile function, if necessary for optimization.
    if (returnedArray != NULL) {
        for (int i = 0; i < NUMBER_OF_WORDS; ++i) {
            if (returnedArray[i] != NULL) {
                free(returnedArray[i]);
            }
        }
        free(returnedArray);
    }

    return ERROR_FLAG;
}