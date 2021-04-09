//The parser file begins

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

//functions used throughout
char *errorMessage (int errorNumber);

int scansSymbolTable(char *c, symbol *symbolTable);

int isvar(input *tokenList, symbol *symbolTable);
void isconst(input *tokenList, symbol *symbolTable);
void isExpression(input *tokenList, symbol *symbolTable);
void isStatement(input *tokenList, symbol *symbolTable);
void isCondition(input *tokenList, symbol *symbolTable);
void isTerm(input *tokenList, symbol *symbolTable);
void isFactor(input *tokenList, symbol *symbolTable);

void emit(int opcode, char op[], int l, int m);

//global counters
int counter = 0;
int symbolIndex = 0;
int codeIndex = 0;
int jumpOnCommandIndex = 0;

instruction *code;

instruction *parse(input *tokenList, int print) {


    
    //memory allocation
    code =  malloc(500 * sizeof(instruction));
    symbol *symbolTable = malloc(500 * sizeof(symbol));
    int numVars = 0;

    printf("\n\n");

    

    
    //PROGRAM
        //BLOCK
        isconst(tokenList,symbolTable);
        numVars = isvar(tokenList,symbolTable);
        emit(6,"INC",0,4+numVars);
        isStatement(tokenList,symbolTable);

        
        if(tokenList[counter].tokenType != periodsym) {
            printf("\n%s\n", errorMessage(1));
            //exit(0);
        }
        
        emit(9,"SYS",0,3);

if(print == 1) {
    printf("\nGenerated Assembly:\n");
    printf("Line    OP   L     M\n");

    for(int i = 0; i < codeIndex; i++)
        printf("%3d %6s %3d %5d \n",i ,code[i].op, code[i].lexiLevel,code[i].m);
        


    return code;
}

else 
    return code;

}


//function will place all code inside struct array
void emit(int opcode, char op[], int l, int m) {
    code[codeIndex].opcode = opcode;
    strcpy(code[codeIndex].op,op);
    code[codeIndex].lexiLevel = l;
    code[codeIndex].m = m;
    codeIndex++;
}


//DONE
void isStatement(input *tokenList, symbol *symbolTable) {
    
    int goodSymbolIndex = scansSymbolTable(tokenList[counter].str, symbolTable);

        
       if(tokenList[counter].tokenType == identsym) {

            symbolIndex = scansSymbolTable(tokenList[counter].str, symbolTable);
            

       
            
            if(scansSymbolTable(tokenList[counter].str, symbolTable) == -1) {

                //Error : undeclared symbol
                printf("\n%s\n", errorMessage(7));
                exit(0);

            }


            if(symbolTable[symbolIndex].kind != 2) {

                    //Error : only variable values may be altered
                    printf("\n%s\n", errorMessage(8));
                    exit(0);
                    

            }

            else {

                counter++;
            }


            if(tokenList[counter].tokenType != becomessym) {

                    //Error : assignment statements must use :=
                    printf("\n%s\n", errorMessage(9));
                    exit(0);

            }


            else {
                
                counter++;
                isExpression(tokenList,symbolTable);
                emit(4,"STO",0,symbolTable[goodSymbolIndex].address);
                return;
                
            }
            
           
        }

////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == beginsym) {

            do {

                counter++;
                isStatement(tokenList,symbolTable);




            }while(tokenList[counter].tokenType == semicolonsym);

            if(tokenList[counter].tokenType != endsym) {

                //Error : begin must be followed by end
                printf("\n%s\n", errorMessage(10));
                exit(0);
            }

            else {

                counter++;
                return;
            }
        }


    ////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == ifsym) {

            counter++;
            isCondition(tokenList,symbolTable);
            jumpOnCommandIndex = codeIndex;
            emit(8,"JPC",0,jumpOnCommandIndex); //NOT SURE what the last parameter is supposed to be
                                                //more than likely the correct value

            if(tokenList[counter].tokenType != thensym) {

                printf("\n%s\n", errorMessage(11));
                exit(0);
                

            }

            else {
                counter++;
                isStatement(tokenList,symbolTable);
                code[jumpOnCommandIndex].m = codeIndex;
                return;
            }

        }


        ////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == whilesym) {

            counter++;
            int loopindex = codeIndex;
            isCondition(tokenList,symbolTable);

            if(tokenList[counter].tokenType != dosym) {

                    printf("\n%s\n", errorMessage(12));
                    exit(0);
                    

            }

            else {  

                //check if it correct?
                counter++;
                jumpOnCommandIndex = codeIndex;
                emit(8,"JPC",0,jumpOnCommandIndex);
                isStatement(tokenList,symbolTable);
                emit(9,"JMP",0, loopindex);
                code[jumpOnCommandIndex].m = codeIndex;
                return;
            }
        }


        ////////////////////////////////////////////////////////////////


        if(tokenList[counter].tokenType == readsym) {

            counter++;
            if(tokenList[counter].tokenType != identsym) {

                //ERROR : Const, var, and read keywords must be followed by identifier
                printf("\n%s\n", errorMessage(2));
                exit(0);

            }


            else {
            symbolIndex = scansSymbolTable(tokenList[counter].str,symbolTable);
           
            
            
            }


            if(scansSymbolTable(tokenList[counter].str, symbolTable) == -1) {
                
                //Error : undeclared symbol
                printf("\n%s\n", errorMessage(7));
                exit(0);

            }


            if(symbolTable[symbolIndex].kind != 2) {
                
                //NOT A VAR
                //Error : only variable names may be altered
                printf("\n%s\n", errorMessage(8));
                exit(0);

            }

            else {
                int goodSymbolIndex = scansSymbolTable(tokenList[counter].str, symbolTable);
                counter++;
                //emit Read or prints to the screen
                emit(9,"SYS",0,2);
                emit(4,"STO", 0, symbolTable[goodSymbolIndex].address);
                return;
            }

            


        }

    //////////////////////////////////////////////////////////////////////
        if(tokenList[counter].tokenType == writesym) {

                counter++;
                isExpression(tokenList,symbolTable);
                
                //emit Write or take input from user
                emit(9,"SYS",0,1);
                return;
        }



}



//DONE
void isExpression(input *tokenList, symbol *symbolTable) {


        if(tokenList[counter].tokenType == minussym) {

            counter++;
            isTerm(tokenList,symbolTable);
            emit(2,"OPR",0,1); //NEG
            while(tokenList[counter].tokenType == plussym || 
                    tokenList[counter].tokenType == minussym) {

                        if(tokenList[counter].tokenType == plussym) {

                            counter ++;
                            isTerm(tokenList,symbolTable);
                            emit(2,"OPR",0,2); //ADD
                        }

                        else {


                            counter++;
                            isTerm(tokenList,symbolTable);
                            emit(2,"OPR",0,3); //SUB
                        }
                    }
        }

        else    { 

            if (tokenList[counter].tokenType == plussym) {
                counter++;
            }
            isTerm(tokenList,symbolTable);
                while(tokenList[counter].tokenType == plussym || 
                    tokenList[counter].tokenType == minussym) {

                        if(tokenList[counter].tokenType == plussym) {

                            counter ++;
                            isTerm(tokenList,symbolTable);
                            emit(2,"OPR",0,2); //ADD
                        }


                         else {


                            counter++;
                            isTerm(tokenList,symbolTable);
                            emit(2,"OPR",0,3); //SUB
                        }

                    }
        }
}


//DONE
void isTerm(input *tokenList, symbol *symbolTable) {

    
    isFactor(tokenList,symbolTable);

    while(tokenList[counter].tokenType == multsym || 
           tokenList[counter].tokenType == slashsym ||
           tokenList[counter].tokenType == modsym) {


               if(tokenList[counter].tokenType == multsym) {
                   counter++;
                   isFactor(tokenList,symbolTable);
                   emit(2,"OPR",0,4);
               }


               else if(tokenList[counter].tokenType == slashsym) {

                   counter++;
                   isFactor(tokenList,symbolTable);
                   emit(2,"OPR",0,5);
               }

                else {

                    counter++;
                    isFactor(tokenList,symbolTable);
                    emit(2,"OPR",0,6);
                }


           }


}

//DONE
void isCondition(input *tokenList, symbol *symbolTable) {

    //newly added
    isFactor(tokenList,symbolTable);


    if(tokenList[counter].tokenType == oddsym) {

            counter++;
            isExpression(tokenList,symbolTable);
            emit(2,"OPR",0,6);
    }

    else {


        if(tokenList[counter].tokenType == eqsym) {

            counter++;
            isExpression(tokenList,symbolTable);
            emit(2,"OPR",0,8);

        }


       else if(tokenList[counter].tokenType == neqsym) {

            counter++;
            isExpression(tokenList, symbolTable);
            emit(2,"OPR",0,9);
        }

        

        else if(tokenList[counter].tokenType == lessym) {

            counter++;
            isExpression(tokenList, symbolTable);
            emit(2,"OPR",0,10);
        }


        else if(tokenList[counter].tokenType == leqsym) {

            counter++;
            isExpression(tokenList, symbolTable);
            emit(2,"OPR",0,11);
        }



        else if(tokenList[counter].tokenType == gtrsym) {

            counter++;
            isExpression(tokenList, symbolTable);
            emit(2,"OPR",0,12);
        }


        else if(tokenList[counter].tokenType == geqsym) {

            counter++;
            isExpression(tokenList, symbolTable);
            emit(2,"OPR",0,13);
        }


        else {

            //Error : condtion must contain comparsion operator

            printf("\n%s\n", errorMessage(13));
            exit(0);

        }
    }

}


//DONE
void isFactor(input *tokenList, symbol *symbolTable) {



    if(tokenList[counter].tokenType == identsym) {
        symbolIndex = scansSymbolTable(tokenList[counter].str,symbolTable);

        

        if(symbolIndex == -1) {

            //Error : undelclared symbol
            printf("\n%s %d\n",errorMessage(7),counter);
                exit(0);


        }


        if(symbolTable[symbolIndex].kind == 1) {

            emit(1,"LIT",0,symbolTable[symbolIndex].val);
        }

        else {

            emit(3,"LOD",0,symbolTable[symbolIndex].address);
        }
        counter++;

    }


    else if(tokenList[counter].tokenType == numbersym) {
        
        
       
       
        emit(1,"LIT",0,atoi(tokenList[counter].str));
       counter++;
    }

    else if(tokenList[counter].tokenType == lparentsym) {

        counter++;
        isExpression(tokenList,symbolTable);
        if(tokenList[counter].tokenType != rparentsym) {

                //Error : right parenthesis must be followed by left parenthesis
                printf("\n%s\n", errorMessage(14));
                exit(0);


        }

        else {
            counter++;
        }
    } 

    else {

        //Error : arithmetic equations must contain operands, parentheses,
        //numbers, or symbols
        printf("\n%s\n", errorMessage(15));
                exit(0);

    }



}

//DONE
int isvar(input *tokenList, symbol *symbolTable) {

        int numVars = 0;
    
    if(tokenList[counter].tokenType == varsym) {
        do {

            numVars++;
            counter++;

            if(tokenList[counter].tokenType != identsym) {

                //Error : const, var, and read keywords must be followed by identifier
                printf("\n%s\n", errorMessage(2));
                exit(0);
                
            }

             if(scansSymbolTable(tokenList[counter].str,symbolTable) != -1) {

                //Error : symbol name has already been declared
                printf("\n%s\n", errorMessage(3));
                exit(0);
                

            }

            else {

                    symbolTable[symbolIndex].kind = 2;
                    strcpy(symbolTable[symbolIndex].name,tokenList[counter].str);
                    symbolTable[symbolIndex].val = 0;
                    symbolTable[symbolIndex].level = 0;
                    symbolTable[symbolIndex].address = numVars + 3;
                    symbolIndex++;

                    counter++;
                    

                }




        }while(tokenList[counter].tokenType == commasym);

        if(tokenList[counter].tokenType != semicolonsym) {

             //Error : constanst and varaibles must be followed by semicolons.
            printf("\n%s\n", errorMessage(6));
                exit(0);

        }

        else 
            counter++;

    }

        return numVars;


}


//DONE
void isconst(input *tokenList, symbol *symbolTable) {

    char savesIdentifier[50];
    int flagIsConst = 0;


    if(tokenList[counter].tokenType == constsym) {
        flagIsConst = 1;
   

        do {

            counter++;

            if(tokenList[counter].tokenType != identsym) {
            
                //Error : const, var, and read keywords must be followed by identifier
                printf("\n%s\n", errorMessage(2));
                exit(0);
                
            }



            if(scansSymbolTable(tokenList[counter].str,symbolTable) != -1) {

                    //Error : symbol name has already been declared
                    printf("\n%s\n", errorMessage(3));
                    exit(0);
                    

                }

            else {


                strcpy(savesIdentifier,tokenList[counter].str);
                counter++;


                }


            if(tokenList[counter].tokenType != eqsym) {

                //Error : constants must be assigned with =
                printf("\n%s\n", errorMessage(4));
                exit(0);
                
                }


            else {
                counter++;
                }



            if(tokenList[counter].tokenType != numbersym) {

                //Error : constants must be assigned an integer value
                printf("\n%s\n", errorMessage(5));
                exit(0);
                
             }


            else {

        

            symbolTable[symbolIndex].kind = 1;
            strcpy(symbolTable[symbolIndex].name,savesIdentifier);
            symbolTable[symbolIndex].val = atoi(tokenList[counter].str);
            symbolTable[symbolIndex].level = 0;
            symbolTable[symbolIndex].address = 0;
            symbolIndex++;

            //get next token
            counter++;


    }


    }while(tokenList[counter].tokenType == commasym);

        if(tokenList[counter].tokenType != semicolonsym && flagIsConst == 1) {
            //Error : constanst and varaibles must be followed by semicolons.
            printf("\n%s\n", errorMessage(6));
            exit(0);

        }


        else if(flagIsConst == 1) {

            counter++;
        }
  
    }


}

//will scan the symbol table to ensure a value is there or not
int scansSymbolTable(char *c, symbol *symbolTable ) {

    for(int i = 0; i < 500; i++ )
        if(strcmp(symbolTable[i].name, c) == 0)
                return i;
    return - 1;
}

//function will return a char array which holds an error
//Errors are numbered 1 - 15
char *errorMessage (int errorNumber) {

    char *errorStrings[] = {
        "index zero",
        "Error : program must end with period",
        "Error : const, var, and read keywords must be followed by identifier",
        "Error : symbol name has already been declared",
        "Error : constants must be assigned with =",
        "Error : constants must be assigned an integer value",
        "Error : constant and variable declarations must be followed by a semicolon",
        "Error : undeclared symbol",
        "Error : only variable values may be altered",
        "Error : assignment statements must use :=",
        "Error : begin must be followed by end", 
        "Error : if must be followed by then",
        "Error : while must be followed by do",
        "Error : condition must contain comparison operator",
        "Error : right parenthesis must follow left parenthesis",
        "Error : arithmetic equations must contain operands, parentheses, numbers, or symbols"};
    
    return errorStrings[errorNumber];
}





