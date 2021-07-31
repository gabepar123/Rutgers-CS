Gabriel Parente
gnp32

To run:
    * Both Normal arguements [e.g. Hello.txt, Directory1] and Optional arguments [e.g.: -s.html, -d10] may be given in any order, repeating optional arguments wil result in them being overwritten.
    * Any invalid optional arguments will cause an error message and end the program [e.g -x10, -d0, -a-7]
    * Having less than 1 argument will not cause the program to run [e.g. ./compare]
    * If a provided directory beings with a '.' it will be ignored
    * If less than 2 files are found after the collection phase, the program will halt
    * An directories or files that were unable to be opened will be skipped, we return EXIT_FAILURE in these cases
    * Any failed mutex locks or memory allocations with cause the program to end.
        * We print to stdout in these cases

Structure:
    * This program first parses all optional arguments and continues to the collection phase
    * During the Collection Phase
        * File Threads are created
            * File Threads consist of popping from the File Queue, and adding that files tokens to the WFD Repo
                * The File Queue is a BOUNDED Queue that reicieves files from the programs input and the Directory Queue
            * File Thread is created first in order to prevent too many files from being given as program input  and filling up the Queue
        * Insert the given input into the Directory Queue or File Queue
        * Directory Threads are created
            * Directory threads open and search the Directories given from the programs input
                * Any Found files are pushed to the File Queue
                * Any Found directories are pushed to the Directory Queue
            * Directory threads continue until there are no more Directories found
                * Directory threads signal the File Threads that there will be no more files pushed
        * Join Directory Threads and File Threads
    * Analysis Phase
        * Analysis Threads are created
            * Analysis threads split up the number of comparisons needed to compute the JSD of each pair of files
            * Adds the output to an array of JSD values
    * WFD Repo is a 2d Linked List that contains Each file, its number of tokens, and A linked list that contains that files words and frequencies
    * File Queue is a Bounded Queue
    * Directory Queue is an Unbounded Queue, doubles when full

Testing:
    * I believe to have tested most scenarios that may occur
        * Inputs that have missing files or directories
        * Empty files or Directories
        * Invalid arugments 
        * Various numbers of threads for each set of threads
        * Deeply nested directories and files
        * Lack of inputs
        * Files with strange contents [e.g. only whitespace, only symbols]
    * All invidiual parts of the program were virgorously tested before being added to the overall program
    * I ran the program hundreds of times (for i in {1..100 } ; do ... ; done) inorder to detect any unexpected race conditions
    * I believe my program to be fully complete and ready for testing!


