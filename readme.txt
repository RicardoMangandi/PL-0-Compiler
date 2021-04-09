Author: Ricardo Mangandi

What my program contains is a driver.c file, lex.c file, parser.c file, and vm.c file.

The driver.c reads in the inputs from a file that is read in as a command line argument.
The driver.c also reads in the argument count to tell how it will print items to screen.

The lex.c will read in the inputs passed from driver.c and produce an output.

The parser.c will read the output from lex.c as it's input and produce an output.

The vm.c will read the output from parser.c as it's input and produce an output.

Instructions:

	1.Type the following in the command line: make
	Ensure that driver.c lex.c parser.c and vm.c are the only source c files.
	
	2. It will then generate an executable file that can be execucted called a.out
	
	3.Type the following in the command line: ./a.out <input file name> -l -a -v
		a. The above command will print the lex.c output, parser.c output, and vm.c output
		   if there are no errors.
	4.Type the following in the command line: ./a.out <input file name> -l -a
		a. The above command will print the lex.c output, and parser.c output
		   if there are no errors.
	5.Type the following in the command line: ./a.out <input file name> -l
		a. The above command will print the lex.c output only.


To print any combination of output follow below:	
-l ===> lex.c output
-a ===> parser.c output
-v ===> vm.c output