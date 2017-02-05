/* Team Cardinal Programmers:
 *      Brett Gedvilas
 *      Joshua McAllister
 *      Rachel Popo
 *      David Ward
 * Date: 1-31-17
 * Class: Operating Systems Spring 2017 (CSCI 3453-001)
 */

/* PROGRAM USAGE:
 *     Launch the compiled binary with the following command-line argument(s):
 *          PATH_TO_FILE
 *
 *          PATH_TO_FILE: Quotation-mark enclosed string directing to a text file containing the desired words.
 *              Path may be explicit (e.g. C:\wordlist.txt)
 *              ...or implicit if the file is located in the same folder as the executable (e.g. .\wordlist.txt)
 *              The list of words may be space-separated or newline-separated.
 *
 *    The program will construct a lexicographically sorted binary tree from the user-specified wordlist
 *    and output the concordance.
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
        // We have reached the end of a tree branch which means a new word needs to be inserted
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

// This function prints the tree and frees all the memory through an in-order traversal.
void printAndDelete(str_node **node_, FILE*fp) {
    // Try to visit left branch
    if ((*node_)->left != NULL) {
        printAndDelete(&((*node_)->left),fp);
    }

    char* newLine;

    //Determine what OS is running to handle carriage return differently
#ifdef _WIN32
    newLine = "\r\n";
#else
    newLine = "\n";
#endif

    //Write word to the file
    fprintf(fp,"%s %s", (*node_)->word, newLine);

    // Try to visit right branch
    if ((*node_)->right != NULL) {
        printAndDelete(&((*node_)->right),fp);
    }

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

str_node* createBinaryTreeFromFile(char** fileLocation) {
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
// This function generates an output file name based on the user given input file name
char* generateOutputFileName(const char*infileName) {
    //NOTE: Assumes that the input file name will ONLY contain up to 5 number characters (e.g. input00001.txt) at the end, followed by .txt file extension
    //It safely handles numbers in other positions in the file name by keeping a count of the desired number
    //of digit characters at the end (2) and by detecting if it reaches a '/' character

    //Flag indicating that we passed a '.' character (file extension)
    int passedDot = 0;

    //Variable to store the size of the c-string of digits from input file name
    int size = 5;

    //Keeps track of the number of digit characters appended to the input file name (assumes 2 is the max)
    int count = 0;

    //Calculate and initialize loop variable
    int i = (strlen(infileName) - 1);

    //String temporary that holds index string (up to 5 digits possible e.g. input02345.txt)
    char indexStr [size];
    indexStr[0] = 0x0;
    indexStr[1] = 0x0;
    indexStr[2] = 0x0;
    indexStr[3] = 0x0;
    indexStr[4] = 0x0;

    //Loop through all of the characters in the input file path name (working backwards, starting from end)
    --size;
    for(i; i >= 0; --i) {
        if (infileName[i] == 0x2E) { //If character is '.'
            passedDot = 1;
        } else if((passedDot) && (infileName[i] <= 0x39) && (infileName[i] >= 0x30)) { //If character is 0-9
            indexStr[size] = infileName[i];
            //Move to the next index
            --size;
            //Increment the count of the number of digits in the input file name
            ++count;
        } else if(infileName[i] == 0x2F) { //Character is a '/' character
            break;
        } else if(passedDot) { //Encountered a character that isn't a digit
            break;
        }
    }

    //Initialize an ouput c-string to the output file name + the number of index digits (stored in count variable)
    char outStr [count + 14 + 1]; //Add 1 for null character

    //Copy the string literal to the output array
    strcpy(outStr, "./myoutput");

    //Initialize loop variables
    i=0;
    size = 10;

    //Loop through all of digit characters from input file name
    for(i; i < 5; ++i) {
        if(indexStr[i] != 0x0) {
            outStr[size] = indexStr[i];
            ++size;
        }
    }

    //Allocate memory in heap so string  safely and defnitely persists after function returns
    char* output = (char*)malloc(sizeof(char) * (count + 14 + 1));

    //Append the file extension
    size = count + 10;
    outStr[size] = '.';
    outStr[++size] = 't';
    outStr[++size] = 'x';
    outStr[++size] = 't';
    outStr[++size] = 0x0;

    //Copy the contents of the output array to a heap allocated memory locations
    strcpy(output, outStr);

    return output;
}

int main(int argc, char **argv) {
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

    //Construct the output file name
    char*outfileName;
    outfileName = generateOutputFileName(argv[1]);

    //Free the memory associated with the file path string, as it is no longer needed
    free(outfileName);

    //Open output file in read-only mode just to determine if file exists
    FILE*ofp = fopen(outfileName, "r");

    //Determine if the output file already exists, if so delete it
    if (ofp){
        //Close and Delete file
        fclose(ofp);
        remove(outfileName);

        //Open the file in append mode so that recursive calls don't overwrite previous file writes
        ofp = fopen(outfileName, "a");

    }else{
        //file doesn't exists, so open the file in append mode so that recursive calls don't overwrite previous file writes
        ofp = fopen(outfileName, "a");
    }

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
                   "\nNow printing each word of the tree & its concordance (in-order traversal):\n");

    // Print tree and delete dynamically allocated memory
    printAndDelete(&root,ofp);

    exit(errorState);
}