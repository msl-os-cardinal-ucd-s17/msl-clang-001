## In-class Assignment 1: Building a lexicographically sorted binary tree

### About:
This program tackles the interesting and mainly pedagogical [task](https://github.com/ivogeorg/msl-clang-001) of counting the occurrences of all the words in some input, with the constraint that the list of words isn't known in advance. This program efficiently maintains a unbalanced binary representation of the list of words. Each incoming word is placed into its proper position when it arrives. The tree nodes are maintained so that at any node the left subtree contains only words that are lexicographically less than the word at the node, and the right subtree contains only words that are greater. When inserting new nodes in an already partially constructed tree, the new nodes were inserted as leaves. This paradigm was maintained consistently. 

The **binary tree** was implemented with the following simple node structure:

	struct node {
		char* word;
		str_node* left;
		str_node* right;
		int count;
	};

The count maintains the occurrences of the given words. The left and right pointers point to the left and right branches of the binary tree. 
The program prints (post order traversal) all of the words and their associated occurrences from the given input file to a text file (e.g. **.\myoutput01**), which matches the index of the input file. 

An example snippet from the output of the program is seen below:

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

The program handles many common error cases: corrupt, missing, or incorrect file paths, and not enough memory due to large input sizes. It also expands further on possible inputs than outlined in the [assignment](https://github.com/ivogeorg/msl-clang-001).

### Running the Program:

Provide a text file containing a list of words (fixed maximum length = 20 characters) that may be space-separated or newline-separated --not comma separated. 
Launch the compiled binary with a command-line argument that contains a path to the text file. The argument can be a string-literal directing to a text file containing the desired words. The path may be an explicit, absolute filepath(e.g. **C:\input01.txt**) or implicit if the file is located in the same directory as the executable (e.g. **.\input01.txt**).

### Libraries:

No external libraries were used, except for the ANSI C Standard Library. 

### Build Requirements:

The project was built using **C99** compiler.

### Bugs and Feature Request:

If you encounter a bug, please read and determine if the problem or idea is not addressed yet. Then open a new issue.

### Future Considerations and Limitations:

As the program was written, it allows for the potential for grossly unbalanced binary trees, which expose inefficiencies in traversing and returning the word list in lexographical order. To improve this program, input could be taken and manipulated in "chunks" of words, instead of individually. These chunks could be randomized or sorted to provide a more balanced tree structure. However, future revisions to this program would analyze if a binary tree is the most suitable data structure. 

### Creators:

This project was made possible by the joint efforts of **Brett Gedvilas, Joshua McAllister, Rachel Popo, and David Ward.**
