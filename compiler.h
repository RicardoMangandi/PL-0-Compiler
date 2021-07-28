typedef enum
{
    modsym = 1,
    identsym,
    numbersym,
    plussym,
    minussym,
    multsym,
    slashsym,
    oddsym,
    eqsym,
    neqsym,
    lessym,
    leqsym,
    gtrsym,
    geqsym,
    lparentsym,
    rparentsym,
    commasym,
    semicolonsym,
    periodsym,
    becomessym,
    beginsym,
    endsym,
    ifsym,
    thensym,
    whilesym,
    dosym,
    callsym,
    constsym,
    varsym,
    procsym,
    writesym,
    readsym,
    elsesym,
    returnsym
} token_type;

typedef struct input
{
    char str[50];
    int tokenType;
    char *errorOrNoerror;
    int sizeOfList;
} input;

typedef struct instruction
{
    char op[4];
    int opcode;
    int lexiLevel;
    int m;
    char tempString[50];
} instruction;

typedef struct symbol
{
    int kind; //const = 1, var = 2, proc = 3
    char name[12];
    int val;
    int level;
    int address;
    int mark;      //to indicate the code has been generated already for a block
    int parameter; //to indicate if the parameter for a procedure has been declared
} symbol;

input *lex_analyze(char *inputfile, int print);
void execute(instruction *code, int print);
instruction *parse(input *tokenList, int print);