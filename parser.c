#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

//functions used throughout
char *errorMessage(int errorNumber);
void emit(int opcode, char op[], int l, int m);

//global counters
int counter = 0;
int symbolIndex = 0; //counter when looping through symbol table as well
int codeIndex = 0;
int jumpOnCommandIndex = 0;

//additional global counters
int numberOfProcedure = 0;
int procedureCountInSymbolTable = 0;

int findProcedure(int indexOfProcedureCounterInLoop, symbol *symbolTable, input *tokenList, char *nameOfProcedure, int kind);
void mark(int currentCount, symbol *symbolTable, input *tokenList);
int symbolTableCheck(char *nameOfString, int lexLevel, symbol *symbolTable);
int symbolTableSearch(char *nameOfString, int lexLevel, symbol *symbolTable, int kind);
int findAddress(int kind, symbol *symbolTable, char *nameOfProcedure);

void block(int lexLevel, int paramter, int procedureIndex, symbol *symbolTable, input *tokenList);

int constDeclarationFunction(int lexLevel, symbol *symbolTable, input *tokenList);
int varDeclarationFunction(int lexLevel, int parameter, symbol *symbolTable, input *tokenList);
int procedureDeclarationFunction(int lexLevel, symbol *symbolTable, input *tokenList);

void isStatement(int lexLevel, symbol *symbolTable, input *tokenList);
void isExpression(int lexLevel, symbol *symbolTable, input *tokenList);
void isCondition(int lexLevel, symbol *symbolTable, input *tokenList);
void isTerm(int lexLevel, symbol *symbolTable, input *tokenList);
void isFactor(int lexLevel, symbol *symbolTable, input *tokenList);

instruction *code;

instruction *parse(input *tokenList, int print)
{

    //memory allocation
    code = malloc(1000 * sizeof(instruction));
    symbol *symbolTable = malloc(1000 * sizeof(symbol));
    printf("\n\n");

    //initialized the struct array to -9999 values
    for (int i = 0; i < 1000; i++)
    {

        symbolTable[i].kind = -9999;
        strcpy(symbolTable[i].name, "-9999");
        symbolTable[i].val = -9999;
        symbolTable[i].level = -9999;
        symbolTable[i].address = -9999;
        symbolTable[i].mark = -9999;
        symbolTable[i].parameter = -9999;
    }

    //PROGRAM
    numberOfProcedure = 1; //number of procedure = 1 because "main" is a procedure that is declared
    emit(7, "JMP", 0, 0);  //default values setting it up where we will jump for now it is at 0

    for (int i = 0; i < tokenList[0].sizeOfList; i++)
    {

        if (tokenList[i].tokenType == procsym)
        {
            numberOfProcedure++;
            emit(7, "JMP", 0, numberOfProcedure); // setting it up to where we will jump for now it is at 0
        }
    }

    symbolTable[symbolIndex].kind = 3;
    strcpy(symbolTable[symbolIndex].name, "main");
    symbolTable[symbolIndex].val = 0;
    symbolTable[symbolIndex].level = 0;
    symbolTable[symbolIndex].address = 0;
    symbolTable[symbolIndex].mark = 0;
    symbolTable[symbolIndex].parameter = 0;
    procedureCountInSymbolTable++; //the first row is filled out in the symbol table
    symbolIndex++;

    //BLOCK:
    //lexLevel is currently 0
    //parameter is currently 0, not passing in any parameters
    //procedureIndex is currently 0, so main is located in procedureIndex 0
    block(0, 0, 0, symbolTable, tokenList); // GOOD

    if (tokenList[counter].tokenType != periodsym)
    {

        //Error : program must end with period

        printf("\n%s\n", errorMessage(1));
        exit(0);
    }

    int i = 0;
    int j = 0;

    //fixes all my jumps by looping through symbol table
    while (i < symbolIndex)
    {

        if (symbolTable[i].kind == 3)
        {

            int tempIndex = findProcedure(i, symbolTable, tokenList, symbolTable[i].name, symbolTable[i].kind);

            code[j].m = symbolTable[tempIndex].address;
            j++;
            i++;
        }

        else
        {

            i++;
        }
    }

    //fixes all my calls by looping through code finding the call and
    //getting the address of that value from the symbol table
    //uses findprocedure to accomplish this
    for (int i = 0; i < codeIndex; i++)
    {

        if (code[i].opcode == 5)

        {

            int k = 1;
            int p = 0;

            while (k < symbolIndex)
            {

                if (symbolTable[k].kind == 3 && strcmp(code[i].tempString, symbolTable[k].name) == 0)
                {
                    int tempIndex = findProcedure(k, symbolTable, tokenList, symbolTable[k].name, 3);
                    code[i].m = symbolTable[tempIndex].address;
                    k++;
                    //p++;
                }

                else
                {

                    k++;
                }
            }
        }
    }

    //emit HALT
    emit(9, "SYS", 0, 3);

    if (print == 1)
    {
        printf("\nGenerated Assembly:\n");
        printf("Line    OP   L     M\n");

        for (int i = 0; i < codeIndex; i++)
            printf("%3d %6s %3d %5d \n", i, code[i].op, code[i].lexiLevel, code[i].m);

        return code;
    }

    else
        return code;
}

//find the procedure in symbol table
int findProcedure(int indexOfProcedureCounterInLoop, symbol *symbolTable, input *tokenList, char *nameOfProcedure, int kind)
{

    for (int i = indexOfProcedureCounterInLoop; i < symbolIndex; i++)
    {

        if (strcmp(symbolTable[i].name, nameOfProcedure) == 0 && kind == 3)
            return i;
    }

    return -1;
}

//goes through all the values that are not mark
void mark(int currentCount, symbol *symbolTable, input *tokenList)
{

    int i = symbolIndex - 1;
    while (currentCount > 0 && i >= 0)
    {

        if (symbolTable[i].mark == 0)
        {
            symbolTable[i].mark = 1;
            currentCount--;
            i--;
        }

        else
        {
            i--;
        }
    }
}

//goes through symbol table ensuring the name of string and level match
int symbolTableCheck(char *nameOfString, int lexLevel, symbol *symbolTable)
{
    for (int i = 0; i < 1000; i++)
    {
        if (strcmp(nameOfString, symbolTable[i].name) == 0 && symbolTable[i].level == lexLevel && symbolTable[i].mark == 0)
        {
            return i;
        }
    }
    return -1;
}

int symbolTableSearch(char *nameOfString, int lexLevel, symbol *symbolTable, int kind)
{
    int index = -1;

    for (int i = 0; i < symbolIndex; i++)
    {

        if (strcmp(nameOfString, symbolTable[i].name) == 0 && symbolTable[i].kind == kind && symbolTable[i].mark == 0)
        {

            if (index == -1)
            {

                index = i;

                continue;
            }

            if (abs(lexLevel - symbolTable[index].level) > abs(lexLevel - symbolTable[i].level))
            {
                index = i;
            }
        }
    }
    return index;
}

//block function
void block(int lexLevel, int paramter, int procedureIndex, symbol *symbolTable, input *tokenList)
{
    int c = 0;
    int v = 0;
    int p = 0;

    c = constDeclarationFunction(lexLevel, symbolTable, tokenList);

    v = varDeclarationFunction(lexLevel, paramter, symbolTable, tokenList);

    p = procedureDeclarationFunction(lexLevel, symbolTable, tokenList);

    symbolTable[procedureIndex].address = codeIndex;

    emit(6, "INC", 0, 4 + v);

    isStatement(lexLevel, symbolTable, tokenList);

    mark(c + v + p, symbolTable, tokenList);
}

int constDeclarationFunction(int lexLevel, symbol *symbolTable, input *tokenList)
{

    int numConst = 0;

    if (tokenList[counter].tokenType == constsym)
    {

        do
        {

            numConst++;
            counter++;
            if (tokenList[counter].tokenType != identsym)
            {

                //Error : const,var,procedure,call, and read keywords must be followed by identifier

                printf("\n%s\n", errorMessage(2));
                exit(0);
            }

            char savesIdentifier[50];

            strcpy(savesIdentifier, tokenList[counter].str);
            counter++;
            if (tokenList[counter].tokenType != eqsym)
            {

                //Error : constants must be assigned with =

                printf("\n%s\n", errorMessage(4));
                exit(0);
            }

            if (tokenList[counter].tokenType != numbersym)
            {
                //Error : constants must be assigned an integer value

                printf("\n%s\n", errorMessage(5));
                exit(0);
            }

            else
            {

                symbolTable[symbolIndex].kind = 1;
                strcpy(symbolTable[symbolIndex].name, savesIdentifier);
                symbolTable[symbolIndex].val = atoi(tokenList[counter].str);
                symbolTable[symbolIndex].level = lexLevel;
                symbolTable[symbolIndex].address = 0;
                symbolTable[symbolIndex].mark = 0;
                symbolTable[symbolIndex].parameter = 0;

                symbolIndex++;
                counter++;
            }

        } while (tokenList[counter].tokenType == commasym);

        if (tokenList[counter].tokenType != semicolonsym)
        {
            //Error : constant and variable declarations must be followed by a semicolon

            printf("\n%s\n", errorMessage(7));
            exit(0);
        }

        counter++;
    }

    return numConst;
}

int varDeclarationFunction(int lexLevel, int parameter, symbol *symbolTable, input *tokenList)
{

    int numberVars = 0;

    if (parameter == 1)
    {
        numberVars = 1;
    }

    else
    {
        numberVars = 0;
    }

    if (tokenList[counter].tokenType == varsym)
    {

        do
        {

            numberVars++;
            counter++;

            if (tokenList[counter].tokenType != identsym)
            {
                //Error : const,var,procedure,call, and read keywords must be followed by identifier

                printf("\n%s\n", errorMessage(2));
                exit(0);
            }

            //check if the var is already declared in symbol table
            if (symbolTableCheck(tokenList[counter].str, lexLevel, symbolTable) != -1)
            {
                //Error : undeclared variable or constant in equation
                //incorrect error, error should be already declared in symbol table with same name

                printf("\n%s\n", errorMessage(6));
                exit(0);
            }

            //IF THE ITEM WAS NOT FOUND THEN WE WILL INSERT THE VALUE INSIDE THE SYMBOL TABLE
            else
            {

                symbolTable[symbolIndex].kind = 2;
                strcpy(symbolTable[symbolIndex].name, tokenList[counter].str);
                symbolTable[symbolIndex].val = 0;
                symbolTable[symbolIndex].level = lexLevel;
                symbolTable[symbolIndex].address = numberVars + 3;
                symbolTable[symbolIndex].mark = 0;
                symbolTable[symbolIndex].parameter = 0;
                symbolIndex++;
                counter++;
            }

        } while (tokenList[counter].tokenType == commasym);

        if (tokenList[counter].tokenType != semicolonsym)
        {
            //Error : constant and variable declarations must be followed by a semicolon

            printf("\n%s\n", errorMessage(7));
            exit(0);
        }

        else
            counter++;
    }

    return numberVars;
}

int procedureDeclarationFunction(int lexLevel, symbol *symbolTable, input *tokenList)
{

    int numberOfProcedure = 0;

    if (tokenList[counter].tokenType == procsym)
    {

        do
        {

            numberOfProcedure++;
            counter++;
            if (tokenList[counter].tokenType != identsym)
            {
                //Error : const,var,procedure,call, and read keywords must be followed by identifier

                printf("\n%s\n", errorMessage(2));
                exit(0);
            }

            if (symbolTableCheck(tokenList[counter].str, lexLevel, symbolTable) != -1)
            {

                //Errpr : competing symbol declarations at the same level
                printf("\n%s\n", errorMessage(3));
                exit(0);
            }

            int procedureIndex = symbolIndex; // end of table

            symbolTable[symbolIndex].kind = 3;
            strcpy(symbolTable[symbolIndex].name, tokenList[counter].str);
            symbolTable[symbolIndex].val = procedureCountInSymbolTable;

            symbolTable[symbolIndex].level = lexLevel;
            symbolTable[symbolIndex].address = 0;
            symbolTable[symbolIndex].mark = 0;
            symbolTable[symbolIndex].parameter = 0;
            procedureCountInSymbolTable++;
            symbolIndex++;
            counter++;

            if (tokenList[counter].tokenType == lparentsym)
            {
                counter++;
                if (tokenList[counter].tokenType != identsym)
                {

                    //Error : parameter may only be specified by an identifier

                    printf("\n%s\n", errorMessage(17));
                    exit(0);
                }

                symbolTable[symbolIndex].kind = 2;
                strcpy(symbolTable[symbolIndex].name, tokenList[counter].str);
                symbolTable[symbolIndex].val = 0;

                symbolTable[symbolIndex].level = lexLevel + 1;
                symbolTable[symbolIndex].address = 3;
                symbolTable[symbolIndex].mark = 0;
                symbolTable[symbolIndex].parameter = 0;
                symbolIndex++;

                symbolTable[procedureIndex].parameter = 1;

                counter++;

                if (tokenList[counter].tokenType != rparentsym)
                {

                    //Error : right parenthesis must follow left parenthesis

                    printf("\n%s\n", errorMessage(14));
                    exit(0);
                }

                counter++;

                if (tokenList[counter].tokenType != semicolonsym)
                {
                    //Error : constant and variable declarations must be followed by a semicolon
                    //maybe wrong with error type

                    printf("\n%s\n", errorMessage(7));
                    exit(0);
                }

                counter++;

                block(lexLevel + 1, 1, procedureIndex, symbolTable, tokenList);
            }

            else
            {

                if (tokenList[counter].tokenType != semicolonsym)
                {

                    //Error : symbol declarations must be followed by a semicolon
                    printf("\n%s\n", errorMessage(7));
                    exit(0);
                }

                counter++;

                block(lexLevel + 1, 0, procedureIndex, symbolTable, tokenList);
            }

            if (code[codeIndex - 1].opcode != 2 && code[codeIndex - 1].m != 0)

            {

                emit(1, "LIT", 0, 0);

                emit(2, "OPR", 0, 0);
            }

            if (tokenList[counter].tokenType != semicolonsym)
            {

                //Error : symbol declarations must be followed by a semicolon

                printf("\n%s\n", errorMessage(7));
                exit(0);
            }

            counter++;

        } while (tokenList[counter].tokenType == procsym);
    }

    return numberOfProcedure;
}

void isStatement(int lexLevel, symbol *symbolTable, input *tokenList)
{

    if (tokenList[counter].tokenType == identsym)
    {

        int symbolIndexReturned = symbolTableSearch(tokenList[counter].str, lexLevel, symbolTable, 2);

        if (symbolIndexReturned == -1)

        {

            printf("\n%s\n", errorMessage(3));
            exit(0);
        }

        counter++;

        if (tokenList[counter].tokenType != becomessym)
        {
            //Error : assignment statements must use :=

            printf("\n%s\n", errorMessage(9));
            exit(0);
        }

        counter++;

        isExpression(lexLevel, symbolTable, tokenList);

        emit(4, "STO", lexLevel - symbolTable[symbolIndexReturned].level, symbolTable[symbolIndexReturned].address);

        return;
    }

    if (tokenList[counter].tokenType == callsym)
    {

        counter++;

        if (tokenList[counter].tokenType != identsym)
        {

            //const,var,procedure,call, and read keywords must be followed by identifier

            printf("\n%s\n", errorMessage(2));
            exit(0);
        }

        int tempsymbolIndex = symbolTableSearch(tokenList[counter].str, lexLevel, symbolTable, 3);
        char savesIdentifierName[50];

        strcpy(savesIdentifierName, tokenList[counter].str);

        if (tempsymbolIndex == -1)
        {
            //Error : undeclared procedure for call

            printf("\n%s\n", errorMessage(16));
            exit(0);
        }

        counter++;

        if (tokenList[counter].tokenType == lparentsym)
        {
            counter++;
            if (symbolTable[tempsymbolIndex].parameter != 1)
            {
                //Error : parameter may only be specified by an identifier

                printf("\n%s\n", errorMessage(16));
                exit(0);
            }

            isExpression(lexLevel, symbolTable, tokenList);

            if (tokenList[counter].tokenType != rparentsym)
            {
                //Error : right parenthesis must follow left parenthesis

                printf("\n%s\n", errorMessage(14));
                exit(0);
            }

            counter++;
        }

        //if the next token is not a left parenthesis then do literal
        else
        {
            emit(1, "LIT", 0, 0);
        }

        emit(5, "CAL", lexLevel - symbolTable[tempsymbolIndex].level, symbolTable[tempsymbolIndex].val);
        strcpy(code[codeIndex - 1].tempString, savesIdentifierName);

        return;
    }

    if (tokenList[counter].tokenType == returnsym)
    {

        if (lexLevel == 0)
        {

            //Error : competing symbol declarations at the same level 3
            //OR
            //Error : cannot return from main 19

            printf("\n%s\n", errorMessage(19));
            exit(0);
        }

        counter++;

        if (tokenList[counter].tokenType == lparentsym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 0);

            if (tokenList[counter].tokenType != rparentsym)
            {

                //Error : right parenthesis must follow left parenthesis

                printf("\n%s\n", errorMessage(14));
                exit(0);
            }

            counter++;
        }

        else
        {

            emit(1, "LIT", 0, 0);
            emit(2, "OPR", 0, 0);
        }

        return;
    }

    if (tokenList[counter].tokenType == beginsym)
    {

        do
        {

            counter++;
            isStatement(lexLevel, symbolTable, tokenList);

        } while (tokenList[counter].tokenType == semicolonsym);

        if (tokenList[counter].tokenType != endsym)
        {
            //Error : begin must be followed by end

            printf("\n%s\n", errorMessage(10));
            exit(0);
        }

        counter++;

        return;
    }

    if (tokenList[counter].tokenType == ifsym)
    {

        counter++;
        isCondition(lexLevel, symbolTable, tokenList);

        jumpOnCommandIndex = codeIndex;
        emit(8, "JPC", 0, jumpOnCommandIndex);

        if (tokenList[counter].tokenType != thensym)
        {
            //Error : if must be followed by then

            printf("\n%s\n", errorMessage(11));
            exit(0);
        }

        counter++;

        isStatement(lexLevel, symbolTable, tokenList);

        if (tokenList[counter].tokenType == elsesym)
        {

            counter++;

            int jumpIndex = codeIndex;
            emit(7, "JMP", 0, jumpIndex);

            code[jumpOnCommandIndex].m = codeIndex;
            isStatement(lexLevel, symbolTable, tokenList);
            code[jumpIndex].m = codeIndex;
        }

        else
        {

            code[jumpOnCommandIndex].m = codeIndex;
        }

        return;
    }

    if (tokenList[counter].tokenType == whilesym)
    {

        counter++;
        int loopindex = codeIndex;
        isCondition(lexLevel, symbolTable, tokenList);

        if (tokenList[counter].tokenType != dosym)
        {

            //Error : while must be followed by do

            printf("\n%s\n", errorMessage(12));
            exit(0);
        }

        counter++;

        jumpOnCommandIndex = codeIndex;

        emit(8, "JPC", 0, jumpOnCommandIndex);

        isStatement(lexLevel, symbolTable, tokenList);

        emit(7, "JMP", 0, loopindex);

        code[jumpOnCommandIndex].m = codeIndex;

        return;
    }

    if (tokenList[counter].tokenType == readsym)
    {

        counter++;
        if (tokenList[counter].tokenType != identsym)
        {

            //Error : const,var,procedure,call, and read keywords must be followed by identifier

            printf("\n%s\n", errorMessage(2));
            exit(0);
        }

        symbolIndex = symbolTableSearch(tokenList[counter].str, lexLevel, symbolTable, 2);

        if (symbolIndex == -1)
        {

            //Error : undeclared variable or constant in equation

            printf("\n%s\n", errorMessage(6));
            exit(0);
        }

        counter++;
        emit(9, "SYS", 0, 2);

        emit(4, "STO", lexLevel - symbolTable[symbolIndex - 1].level, symbolTable[symbolIndex - 1].address);

        return;
    }

    if (tokenList[counter].tokenType == writesym)
    {

        counter++;

        isExpression(lexLevel, symbolTable, tokenList);
        emit(9, "SYS", 0, 1);
        return;
    }
}

//GOOD
void isCondition(int lexLevel, symbol *symbolTable, input *tokenList)
{

    if (tokenList[counter].tokenType == oddsym)
    {

        counter++;
        isExpression(lexLevel, symbolTable, tokenList);
        emit(2, "OPR", 0, 6);
    }

    else
    {

        isExpression(lexLevel, symbolTable, tokenList);

        if (tokenList[counter].tokenType == eqsym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 8);
        }

        else if (tokenList[counter].tokenType == neqsym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 9);
        }

        else if (tokenList[counter].tokenType == lessym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 10);
        }

        else if (tokenList[counter].tokenType == leqsym)
        {
            counter++;

            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 11);
        }

        else if (tokenList[counter].tokenType == gtrsym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 12);
        }

        else if (tokenList[counter].tokenType == geqsym)
        {

            counter++;
            isExpression(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 13);
        }

        else
        {

            //Error : condtion must contain comparsion operator

            printf("\n%s\n", errorMessage(13));
            exit(0);
        }
    }
}

//GOOD
void isExpression(int lexLevel, symbol *symbolTable, input *tokenList)
{

    if (tokenList[counter].tokenType == minussym)
    {

        counter++;
        isTerm(lexLevel, symbolTable, tokenList);
        emit(2, "OPR", 0, 1);

        while (tokenList[counter].tokenType == plussym ||
               tokenList[counter].tokenType == minussym)
        {

            if (tokenList[counter].tokenType == plussym)
            {

                counter++;
                isTerm(lexLevel, symbolTable, tokenList);
                emit(2, "OPR", 0, 2);
            }

            else
            {

                counter++;
                isTerm(lexLevel, symbolTable, tokenList);
                emit(2, "OPR", 0, 3);
            }
        }
    }

    else
    {

        if (tokenList[counter].tokenType == plussym)
        {
            counter++;
        }

        isTerm(lexLevel, symbolTable, tokenList);
        while (tokenList[counter].tokenType == plussym ||
               tokenList[counter].tokenType == minussym)
        {

            if (tokenList[counter].tokenType == plussym)
            {

                counter++;
                isTerm(lexLevel, symbolTable, tokenList);
                emit(2, "OPR", 0, 2); //ADD
            }

            else
            {

                counter++;
                isTerm(lexLevel, symbolTable, tokenList);
                emit(2, "OPR", 0, 3); //SUB
            }
        }
    }
}

//GOOD
void isTerm(int lexLevel, symbol *symbolTable, input *tokenList)
{

    isFactor(lexLevel, symbolTable, tokenList);

    while (tokenList[counter].tokenType == multsym ||
           tokenList[counter].tokenType == slashsym ||
           tokenList[counter].tokenType == modsym)
    {

        if (tokenList[counter].tokenType == multsym)
        {
            counter++;
            isFactor(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 4);
        }

        else if (tokenList[counter].tokenType == slashsym)
        {

            counter++;
            isFactor(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 5);
        }

        else
        {

            counter++;
            isFactor(lexLevel, symbolTable, tokenList);
            emit(2, "OPR", 0, 6);
        }
    }
}

void isFactor(int lexLevel, symbol *symbolTable, input *tokenList)
{

    if (tokenList[counter].tokenType == identsym)
    {

        int symbolIndexV = symbolTableSearch(tokenList[counter].str, lexLevel, symbolTable, 2);

        int symbolIndexC = symbolTableSearch(tokenList[counter].str, lexLevel, symbolTable, 3);

        if (symbolIndexV == -1 && symbolIndexC == -1)
        {
            //Error : undeclared variable or constant in equation"

            printf("\n%s\n", errorMessage(6));
            exit(0);
        }

        else if (symbolIndexC == -1 || (symbolIndexV != -1 && symbolTable[symbolIndexV].level > symbolTable[symbolIndexC].level))
        {

            emit(3, "LOD", lexLevel - symbolTable[symbolIndexV].level, symbolTable[symbolIndexV].address);
        }

        else
        {

            emit(1, "LIT", 0, symbolTable[symbolIndexC].val);
        }

        counter++;
    }

    else if (tokenList[counter].tokenType == numbersym)
    {

        emit(1, "LIT", 0, atoi(tokenList[counter].str));
        counter++;
        return;
    }

    else if (tokenList[counter].tokenType == lparentsym)
    {

        counter++;
        isExpression(lexLevel, symbolTable, tokenList);
        if (tokenList[counter].tokenType != rparentsym)
        {
            //Error : right parenthesis must be followed by left parenthesis

            printf("\n%s\n", errorMessage(14));
            exit(0);
        }

        counter++;
    }

    else if (tokenList[counter].tokenType == callsym)
    {

        isStatement(lexLevel, symbolTable, tokenList);
        return;
    }

    else
    {
        //Error : arithmetic equations must contain operands, parentheses, numbers, or symbols

        printf("\n%s\n", errorMessage(15));
        exit(0);
    }
}

//function will place all code inside struct array
void emit(int opcode, char op[], int l, int m)
{
    code[codeIndex].opcode = opcode;
    strcpy(code[codeIndex].op, op);
    code[codeIndex].lexiLevel = l;
    code[codeIndex].m = m;
    codeIndex++;
}

//function receives the errorNumber and sends the user the string
char *errorMessage(int errorNumber)
{

    char *errorStrings[] = {
        "index zero",
        "Error : program must end with period",
        "Error : const,var,procedure,call, and read keywords must be followed by identifier",
        "Error : competing symbol declarations at the same level",
        "Error : constants must be assigned with =",
        "Error : constants must be assigned an integer value",
        "Error : undeclared variable or constant in equation",
        "Error : symbol declarations must be followed by a semicolon",
        "Error : only variable values may be altered",
        "Error : assignment statements must use :=",
        "Error : begin must be followed by end",
        "Error : if must be followed by then",
        "Error : while must be followed by do",
        "Error : condition must contain comparison operator",
        "Error : right parenthesis must follow left parenthesis",
        "Error : arithmetic equations must contain operands, parentheses, numbers, or symbols",
        "Error : undeclared procedure for call",
        "Error : parameter may only be specified by an identifier",
        "Error : parameter must be declared",
        "Error : cannot return from main"

    };

    return errorStrings[errorNumber];
}