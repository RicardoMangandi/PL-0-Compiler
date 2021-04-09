

typedef enum
{
	modsym = 1, identsym, numbersym, plussym, minussym, 
	multsym, slashsym, oddsym, eqsym, neqsym, 
	lessym, leqsym, gtrsym, geqsym, lparentsym, 
	rparentsym, commasym, semicolonsym, periodsym, becomessym, 
	beginsym, endsym, ifsym, thensym, whilesym, 
	dosym, callsym,	constsym, varsym, procsym, 
	writesym, readsym, elsesym, returnsym
} token_type;




typedef struct input {

	
	char str[50];
    int tokenType;
    char *errorOrNoerror;
}input;




typedef struct instruction {

	char op[4];

	int opcode;
    int lexiLevel;
    int m;
}instruction;



typedef struct symbol
{
	int kind;
	char name[12];
	int val;
	int level;
	int address;
} symbol;


input *lex_analyze(char *inputfile, int print);
void execute(instruction *code, int print);
instruction *parse(input *tokenList, int print);