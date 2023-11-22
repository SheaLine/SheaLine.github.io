// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa8
// ----------------------------------------------------------------------------

Dictionary.h        - Header file for Dictionary ADT using a Red-Black tree
Dictionary.cpp      - implementation of Dicitonary ADT using a Red-Black tree
DictionaryTest.cpp  - Test file for Dictionary ADT using a Red-Black tree
Order.cpp           -  It will read in the lines of an input file, each
                       line being a (unique) single string, then insert these strings (as keys) into a Dictionary. The corresponding
                       values will be the line number in the input file where the string was read. Your program will write two
                       string representations of the Dictionary to the output file. 
                       "key : value\n"
                       "key\n"
WordFrequency.cpp   - It will read in each line of a file, 
                    - parse the individual words on each line, 
                    - convert each word to all lower case characters, then place it (as key) in a Dictionary.  
                    - The number of times a given word is encountered (its frequency) will also be stored (as value) in the Dictionary
Makefile            - Make file to compile, vangrind test, and clean all files for this project (see top comments of file for individual commands)