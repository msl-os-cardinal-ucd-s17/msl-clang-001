/* Team Cardinal Programmers:
 *      Brett Gedvilas
 *      Joshua McAllister
 *      Rachel Popo
 *      David Ward
 * Date: 1-31-17
 * Class: CSCI 3453
 */

/* PROGRAM USAGE:
 *     Launch the compiled binary with the following command-line argument(s):
 *          PATH_TO_FILE
 *
 *          PATH_TO_FILE: Quotation-mark enclosed string directing to a text file containing the desired words.
 *              Path may be explicit (e.g. C:\wordlist.txt)
 *              ...or implicit if the file is located in the same folder as the executable (e.g. .\wordlist.txt)
 *              The list of words may be space-separated or newline-separated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_WORD_LENGTH 20
#define SCANF_STRING_LENGTH_PREP(S) "%" #S "s"
#define SCANF_STRING_LENGTH_FORMAT(S) SCANF_STRING_LENGTH_PREP(S)

#define ERRNUM_IO 1
#define ERRNUM_ARGS 2
#define ERRNUM_HEAP_ALLOCATE_NODE 3
#define ERRNUM_HEAP_ALLOCATE_WORD 4

typedef struct node str_node;

// The basic definition of the nodes in the Binary Tree
struct node {
    char* word;
    str_node* left;
    str_node* right;
    int count;
};

// Global returned by main; 0 if main completed successfully, a defined error code otherwise.
int errorState = 0;

/* insertNode function documentation
 * Libraries used: string.h
 *
 * This function recursively performs a pre-order traversal through the nodes of the binary tree
 * and either creates a new node if the word is not found,
 * or increments the word count if a node is found.
 *
 * NOTE: the node needs to be passed by reference in order to correctly assign the
 *       parent pointer to the new node
 *
 * Preconditions:
 *     inputWord must point to a non-null c-string with 1 or more characters.
 *     currentNodePtr can point to null or to the root of a subtree.
 *
 * Postconditions:
 *      currentNodePtr points to the root node of a (sub)tree that counted the inputWord.
 *      insertNode returns 0 if successful, a predefined error constant if failed.
 */

int insertNode(char* inputWord, str_node** currentNodePtr) {
    assert(currentNodePtr != NULL);
    int comparison;

    if ((*currentNodePtr) == NULL) {
        // we have reached the end of a tree branch which means a new word needs to be inserted
        // Allocate memory for the new node and assign the location to leafptr

        (*currentNodePtr) = (str_node*)malloc(sizeof(str_node));
        if ((*currentNodePtr) == NULL) {
            return ERRNUM_HEAP_ALLOCATE_NODE;
        }

        //Set the node pointers to NULL since we followed the paradigm of adding new nodes beyond the leaf without rearranging
        (*currentNodePtr)->left = NULL;
        (*currentNodePtr)->right = NULL;

        (*currentNodePtr)->word = inputWord;

        //Set the count to an intial value
        (*currentNodePtr)->count = 1;
    }

        // Otherwise, compare value to the current node
    else {
        comparison = strcmp(inputWord, (*currentNodePtr)->word);

        if (comparison == 0) {
            // if words are the same, increment count
            ++((*currentNodePtr)->count);
        }
        else if (comparison < 0) {
            // enter the left subtree
            insertNode(inputWord, &((*currentNodePtr)->left));
        }
        else {
            // enter the right subtree
            insertNode(inputWord, &((*currentNodePtr)->right));
        }
    }

    return 0;
}

// This function prints the tree and frees all the memory through a post-order traversal.
void printAndDelete(str_node **node_) {
    // Try to visit left branch
    if ((*node_)->left != NULL) {
        printAndDelete(&((*node_)->left));
    }

    // Try to visit right branch
    if ((*node_)->right != NULL) {
        printAndDelete(&((*node_)->right));
    }

    // Print the word and word count
    printf("%s %d\n", (*node_)->word, (*node_)->count);

    // Free memory
    free((*node_)->word);
    free(*node_);

}

/* Reads a file from a provided pointer to a c-string of a file path
 * and constructs a binary tree one word at a time (expected run-time of O(n*lg(n)))
 *
 * Preconditions:
 *      fileLocation cannot be NULL & it must point to an initialized c-string.
 *
 * Postconditions:
 *      If the file could not be read,
 *          returns NULL and sets the global errorState variable to ERRNUM_IO.
 *      If the file could be read but it began with a terminating character,
 *          returns NULL while errorState is 0.
 *      If the file could be read and valid words exist & precede any invalid words,
 *          returns root pointer to lexicographically sorted binary tree.
 */

str_node* createBinaryTreeFromFile(const char** fileLocation) {
    assert((fileLocation != NULL) && ((*fileLocation) != NULL));
    FILE* fp = NULL;
    str_node* root = NULL;
    char* currentWord = NULL;

    // NOTE: fopen looks for the file in one of two ways:
    //      1) An explicit path to the file (e.g. "C:\wordlist.txt")
    //      2) An implicit search in the current execution folder of the binary through the dot qualifier (e.g. ".\wordlist.txt")
    //      You will need to copy the supplied wordlist.txt file to wherever it is convenient
    //            (either in your clone of the cmake-build folder for an implicit search,
    //             or to anywhere else for an explicit search).
    fp = fopen((*fileLocation), "r");

    // This checks to make sure the file was successfully loaded (fopen() returns a NULL pointer if unsuccessful)
    if (fp == NULL) {
        errorState = ERRNUM_IO;
        return NULL;
    }

    while (!feof(fp)) {
        // Space allocated for additional null-character element
        currentWord = (char*)malloc(sizeof(char) * (MAX_WORD_LENGTH+1));

        if (currentWord == NULL) {
            errorState = ERRNUM_HEAP_ALLOCATE_WORD;
            break;
        }

        if (fscanf(fp, SCANF_STRING_LENGTH_FORMAT(MAX_WORD_LENGTH), currentWord) != 1) {
            free(currentWord);
            break;
        }

        if (insertNode(currentWord, &root) != 0) {
            errorState = ERRNUM_HEAP_ALLOCATE_NODE;
            break;
        }
    }

    fclose(fp);
    return root;
}

int main(int argc, const char *argv[]) {
    printf("Number of user-specified command-line arguments: %d\n\n", (argc-1));
    if (argc != 2) {
        printf("ERROR: Illegal number of arguments.\n");
        printf("COMMAND-LINE ARGUMENTS:\n"
                       "PATH_TO_FILE\n\n"
                       "PATH_TO_FILE: Quotation-mark enclosed string directing to a text file containing the desired words.\n"
                       "\tPath may be explicit (e.g. \"C:\\wordlist.txt\")\n"
                       "\t...or implicit if text file located in same folder as executable (e.g. \".\\wordlist.txt\")");
        exit(ERRNUM_ARGS);
    }

    printf("Now constructing binary tree from provided list of words...\n");
    str_node* root = NULL;
    root = createBinaryTreeFromFile(&(argv[1]));

    if (root == NULL) {
        if (errorState == ERRNUM_IO) {
            printf("I/O ERROR: Cannot read provided file path.\n");
        }
        else {
            printf("ERROR: File is empty or begins with illegal characters.\n");
        }
        exit(errorState);
    }

    if (errorState == ERRNUM_HEAP_ALLOCATE_NODE) {
        printf("WARNING: Not enough memory on heap to allocate another node of the binary tree.\n");
    }

    if (errorState == ERRNUM_HEAP_ALLOCATE_WORD) {
        printf("WARNING: Not enough memory on heap to allocate another word;\n"
                       "\tfile may have unread words.\n");
    }

    printf("Construction of binary tree finished.\n"
                   "\nNow printing each word of the tree & its count (post-order traversal):\n");

    // Print tree and delete dynamically allocated memory
    printAndDelete(&root);

    return errorState;
}