## Source files

### C Source Files
The following are source files: driver.c, lex.c, parser.c, and vm.c

### Make File
The following is a make file: Makefile

### Header File
The following is a header file: compiler.h

### Test Files
The following are test files: test1.txt, test2.txt

### Output Files
The following are output files to the above test: output1.txt, output2.txt

### Tester file
The following is a testing script to compare outputs: tester.sh.sh


## Run the Program

* Ensure that you have a linux/unix environment to run this code.

1. Run the following command: `make`

   Output in terminal: `gcc driver.c lex.c parser.c vm.c -lm`
   The output above generates an executable file by the name of `./a.out`
   

2. Run the following command: `./a.out test1.txt -l -a -v`
  
   The program takes in command line arguments therefore a file name is always expected.
   The above command tests file test1.txt and outputs all outputs.
   
   The command `./a.out test1.txt -l` will print the lexical analyzer outputs of only test1.txt.
   The command `./a.out test1.txt -a` will print the parsers outputs of only test1.txt.
   The command `./a.out test1.txt -v` will print the virtual machine outputs of only test1.txt.

3. To run the tester file run the following command: `sh tester.sh.sh`
   
   The output if both test cases pass is: `-n Testing test1.txt : pass
                                           -n Testing test2.txt (this one is super long, but it covers everything) : pass`
  
   The script automatically generates a file called output.txt every time it runs.
   Note that the script only works for these two test files. If the test files are not
   present in the same directory an error will occur. 
