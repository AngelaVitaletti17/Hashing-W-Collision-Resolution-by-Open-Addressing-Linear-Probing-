//ANGELA VITALETTI	cs435 mp

Option Chosen: 2, Hashing
Language: C++
PLEASE COMPILE WITH THE FOLLOWING COMMAND: g++ -std=c++0x hashtable.cpp
RUN WITH THE COMMAND ./a.out testfile.txt, where testfile.txt is a batch file containing commands

Files: The ONLY file submitted is hashtable.cpp which adheres to Handout 2 Section 2's naming conventions, as well as Section 3.

Bugs: I did not find any, this program outputs as the cases would in the Mini Project Handout
I did NOT include "HANDOUT 2 ADHERED; NO BUGS TO REPORT" because Handout 2 specified to contain that OR "compilation and execution instructions, a bug report, and other useful information"

Assumptions I made about the project/more useful information:
1. ALL classes contain the last 4 digits of my student ID at the end, such as my lexicon_ class that contains the following two classes discussed below
2. To represent Hash Table T, I made two classes
	a. A Hash Table class to represent that actual table, which a vector was used for
	b. A Hash Entry class to represent each entry that has an index, a value, and a reference number to Array A
3. Array A (an array of characters) was represented by a string
	a. C++ does not support arrays of dynamic size, and as such, resizing them (which A needs to be resized on occasion) is not allowed
	b. A string is an array of characters, and behaves as such
4. The documentation mentions the ability to resize the hash table, but doesn't specify when to implement it
	a. My program DOES feature the capability to resize a hash table, AS WELL AS Array A
5. Some of my functions (HashInsert, for example) pass Lexicon L by reference, using pointers in order to be able to affect the actual lexicon
6. I interpreted the line "You DO NOT DO a linear search to find out if it exists already in A; it is inefficient" as follows:
	a. This applies to insertion, meaning that the insertion function will not check if it already exists, it will just add it regardless
7. This was compiled and written on AFS. 
8. The zip file (mp_.zip) contains ONLY this file and hashtable.cpp. NO SUB DIRECTORIES
9. The command "zip mp_ hashtable.cpp mp_.txt" was used to archive the file, which was successful in my environment 
10. The command "unzip mp_.zip" was used to unzip the file, and the compiler/execution command mentioned above can then be used along with a test file in the .txt format, which was successful in my environment
11. My code DOES NOT allow for the creation of a second Lexicon, meaning that once one is created (command 14 xx where xx is the size) will be ignored
12. My code also outputs when an element cannot be inserted, due to the probing index reaching it's limit (value Michael)
13. Any vector/string operations (I only used resize) that are of linear time cannot be performed in constant time, to my knowledge
14. The program will detects whether or not the correct syntax was detected (i.e., the execution command as above)

ADDITIONAL COMPILING NOTES:
1. Again, please compile using g++ -std=c++0x hashtable.cpp
2. The compiler version is GNU C++ (GCC) version 4.8.5 
