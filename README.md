# In-class Assignment 1
## Building a lexicographically sorted binary tree

About:
This program tackles the interesting and mainly pedagogical task of counting the occurrences of all the words in some input, with the constraint that the list of words isn't known in advance. This program efficiently maintains a unbalanced binary representation of the list of words. Each incoming word is placed into its proper position when it arrives. The tree nodes are maintained so that at any node the left subtree contains only words that are lexicographically less than the word at the node, and the right subtree contains only words that are greater. When inserting new nodes in an already partially constructed tree, the new nodes were inserted as leaves. This paradigm was maintained consistently. 

The binary tree was implemented with the following simple node structure:
~~~~
struct node {  
    char* word;  
    str_node* left;  
    str_node* right;  
    int count;  
};  
~~~~
The count maintains the occurrences of the given words. The left and right pointers point to the left and right branches of the binary tree. 

The program constructs a lexicographically sorted binary tree and outputs its concordance through an in-order traversal. 
Example output of the program is seen below:

~~~~
aid 1  
all 1  
come 1  
for 1  
good 1  
is 1  
men 1  
now 1  
of 1  
party 1  
the 2  
their 1  
time 1  
to 2  
~~~~

The program handles many common error cases: corrupt, missing, or incorrect file paths, and not enough memory due to large input sizes.

Running the Program:

Provide a text file containing a list of words (fixed maximum length = 20 characters) that may be space-separated or newline-separated. Not comma separated. The encoding TODO: specify encoding?? UTF-8?
Launch the compiled binary with a command-line argument that contains a path to the text file in the form of a c-string (char*). The argument can be a string-literal directing to a text file containing the desired words. The path may be an explicit, absolute filepath(e.g. C:\wordlist.txt) or an implicit if the file is located in the same directory as the executable (e.g. .\wordlist.txt).

Build Requirements:

The project was built using C99 compiler.

Bugs and Feature Request:

If you encounter a bug, please read and determine if the problem or idea is not addressed yet. Then open a new issue.

Creators:

This project was made possible by the joint efforts of Brett Gedvilas, Joshua McAllister, Rachel Popo, and David Ward. 

(Last Revised 2/2/17)
