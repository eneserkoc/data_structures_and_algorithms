#levensthein_distance

NOTE : Comment lines in the code are Turkish, it will be translated later.

For each of the two given words, a word game will be written which, by changing only one letter at each step, finds out whether the first word has changed to the second word, and if so, which words have been passed. The following example shows the conversion of the word "brook" to the word "croon".

brook -> crook -> croon

The graph will be used to reach a different word by changing a given word, one letter at a time.If another word is obtained by changing only 1 letter of a word, there is a link between the two words. For example, in the above example there is a link between "brook" and "crook".

Process Steps : 

1 - The words in the "kelime.txt" file, consisting of words in lower case, with 5 letters, will be used in the code.

2 - First, the words in the file are placed in to a string array. Then, using these words, the graph is obtained. An "adjacency matrix" in sizes of "NxN" is used to create the graph. In this case, for example, the 0th digit in string represents the 0th node. If there is a single letter change between two words, the value at the relevant address of the matrix is set to 1, since this is the link between the two words

3 - (UNNECESSARY) A case is written to let you know whether there is a single letter conversion between two words.


4- For the two given words, the "queue" data structure is used to find out if there are conversions.
	
	A - In the first step, the node of the first word is placed in the "queue".

	B - The following operations are performed until the "queue" is completely emptied or until it reaches the output node (second word).

		 The node at the beginning of "queue" is pulled.

			a )If this node is the second word, the operation is completed.
			b ) If this node is not the second word, the word belonging to the node is printed on the screen. This node is added to the "queue" of all neighbors that have not previously been queued. A redundant array is used to see if a node has already entered the queue.
			c ) Information about which word is added to "queue" via the other word is kept in the "parent" array