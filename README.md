#ASSIGNMENT 3: INDEXER#
By: Gabriel Gutierrez (gg548)
      Ayush Joshi (aj470)

Group: 1/0

#Usage:#
Input: ./index outputFile fileToIndex
#Description:#
-	Our indexer incorporates a tokenizer that reads in text from the current file character by character using the fgetc() function and when any non-alphanumeric character is reached or when the end of file is reached, which is detected with the feof() function, the token string is returned to then be sorted into our sorted-list structure.

-	Our Sorted-List structure is used to organize each word or “token” according to the specifications of the assignment. Then, it is also used in conjunction with an iterator to traverse through the list when writing to the output file requested.

-	Our indexer uses a recursive function, traverseDirectories, to recursively go through directories until a file is reached. When a file is reached, traverseFile is called to begin indexing the file.

-	Originally the indexer printed out entire path of each token, then according to new specifications, it now only prints the file name in which the token was found.
#Runtime Analysis:#
-	Runtime is based off the input size, that is, the file or directory requested by the user to index. Therefore, the runtime is based off the number of unique tokens (t) found in each of the files to be explored (f) which is O(t*f) or O(n) runtime.
