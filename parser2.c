#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"



char *errorMessage (int errorNumber);

int scansSymbolTable(char *c, symbol *symbolTable);
void isconst(input *list, symbol *symbolTable, int sizeOfList);
int isvar(input *list, symbol *symbolTable, int sizeOfList);
void isExpression(input *list, symbol *symbolTable, int sizeOfList);

void isStatement(input *list, symbol *symbolTable, int sizeOfList);

void isCondition(input *list, symbol *symbolTable, int sizeOfList);
void isTerm(input *list, symbol *symbolTable, int sizeOfList);
void isFactor(input *list, symbol *symbolTable, int sizeOfList);

void emit(int opcode, char op[], int l, int m);



int counter = 0;
int symbolIndex = 0;
int codeIndex = 0;
int jumpOnCommandIndex = 0;

instruction *code;

int main(int argc, char **argv) {


    input list[14];
    symbol *symbolTable = malloc(500 * sizeof(symbol));
    code =  malloc(500 * sizeof(instruction));
    

    list[0].tokenType = 29;
    strcpy(list[0].str,"var");

    list[1].tokenType = 2;
    strcpy(list[1].str,"x");

     list[2].tokenType = 17;
    strcpy(list[2].str,",");

     list[3].tokenType = 2;
    strcpy(list[3].str,"y");


     list[4].tokenType = 18;
    strcpy(list[4].str,";");


     list[5].tokenType = 21;
    strcpy(list[5].str,"begin");


     list[6].tokenType = 2;
    strcpy(list[6].str,"x");

    list[7].tokenType = 20;
    strcpy(list[7].str, ":=");


     list[8].tokenType = 2;
    strcpy(list[8].str,"y");


     list[9].tokenType = 6;
    strcpy(list[9].str,"*");


     list[10].tokenType = 3;
    strcpy(list[10].str,"2");


     list[11].tokenType = 18;
    strcpy(list[11].str,";");


     list[12].tokenType = 22;
    strcpy(list[12].str,"end");

    list[13].tokenType = 19;
    strcpy(list[13].str,".");



    for(int i = 0; i < 14; i++) {
        if(list[i].tokenType == 2 || list[i].tokenType == 3)
        printf("%d %s ",list[i].tokenType, list[i].str );

        else 
             printf("%d ",list[i].tokenType);
    }


    int sizeOfList = 14;
    int numVars = 0;
    


    //PROGRAM
        //BLOCK
        isconst(list,symbolTable,sizeOfList);
        numVars = isvar(list,symbolTable,sizeOfList);
        emit(6,"INC",0,4+numVars);
        isStatement(list,symbolTable,sizeOfList);

        if(list[counter].tokenType != periodsym)
            printf("\n%s\n", errorMessage(1));
        
        emit(9,"SYS",0,3);


    printf("\nSymbol Index: %d\n", symbolIndex);

    for(int i = 0; i < symbolIndex+1; i++) {
        printf("\nIndex: %d %d %s %d %d %d\n", i, symbolTable[i].kind, symbolTable[i].name, symbolTable[i].val, symbolTable[i].level,
                                                    symbolTable[i].address);
    }


    printf("\nThe current codeIndex: %d\n", codeIndex);

    printf("Generated Assembly:\n");
    printf("Line    OP   L     M\n");

    for(int i = 0; i < codeIndex; i++)
        printf("%3d %6s %3d %5d \n",i ,code[i].op, code[i].lexiLevel,code[i].m);


    



}



void emit(int opcode, char op[], int l, int m) {


    code[codeIndex].opcode = opcode;
    strcpy(code[codeIndex].op,op);
    code[codeIndex].lexiLevel = l;
    code[codeIndex].m = m;
    codeIndex++;



}




//DONE
void isStatement(input *tokenList, symbol *symbolTable, int sizeOfList) {
    
    int goodSymbolIndex = scansSymbolTable(tokenList[counter].str, symbolTable);

        

       if(tokenList[counter].tokenType == identsym) {

            symbolIndex = scansSymbolTable(tokenList[counter].str, symbolTable);
            

       
            
            if(symbolIndex == -1) {

                //Error : undeclared symbol
                printf("\n%s\n", errorMessage(7));
                return;

            }


            if(symbolTable[symbolIndex].kind != 2) {

                    //Error : only variable values may be altered
                    printf("\n%s\n", errorMessage(8));
                    return;
                    

            }

            else {

                counter++;
            }


            if(tokenList[counter].tokenType != becomessym) {

                    //Error : assignment statements must use :=
                    printf("\n%s\n", errorMessage(9));

            }


            else {

                counter++;
                isExpression(tokenList,symbolTable,sizeOfList);
                emit(4,"STO",0,symbolTable[goodSymbolIndex].address);
                return;
                
            }
            
           
        }

////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == beginsym) {

            do {

                counter ++;
                isStatement(tokenList,symbolTable,sizeOfList);




            }while(tokenList[counter].tokenType == semicolonsym);

            if(tokenList[counter].tokenType != endsym) {

                //Error : begin must be followed by end
                printf("\n%s\n", errorMessage(10));
                return;
            }

            else {

                counter++;
                return;
            }
        }


    ////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == ifsym) {

            counter++;
            isCondition(tokenList,symbolTable,sizeOfList);
            jumpOnCommandIndex = codeIndex;
            emit(8,"JPC",0,jumpOnCommandIndex); //NOT SURE what the last parameter is supposed to be
                                                //more than likely the correct value

            if(tokenList[counter].tokenType != thensym) {

                printf("\n%s\n", errorMessage(11));
                return;

            }

            else {
                counter++;
                isStatement(tokenList,symbolTable,sizeOfList);
                code[jumpOnCommandIndex].m = codeIndex;
                return;
            }

        }


        ////////////////////////////////////////////////////////////////

        if(tokenList[counter].tokenType == whilesym) {

            counter++;
            int loopindex = codeIndex;
            isCondition(tokenList,symbolTable,sizeOfList);

            if(tokenList[counter].tokenType != dosym) {

                    printf("\n%s\n", errorMessage(12));
                    return;

            }

            else {

                counter++;
                jumpOnCommandIndex = codeIndex;
                emit(8,"JPC",0,jumpOnCommandIndex);
                code[jumpOnCommandIndex].m = codeIndex;
                return;
            }
        }


        ////////////////////////////////////////////////////////////////


        if(tokenList[counter].tokenType == readsym) {

            counter++;
            if(tokenList[counter].tokenType != identsym) {

                //ERROR :     
                printf("\n%s\n", errorMessage(0));
            }


            else {
            symbolIndex = scansSymbolTable(tokenList[counter].str,symbolTable);
            }


            if(symbolIndex == -1) {

                printf("\n%s\n", errorMessage(7));
            }


            if(symbolTable[symbolIndex].kind != 2) {

                printf("\n%s\n", errorMessage(0));
            }

            else {

                counter++;
                //emit Read or prints to the screen
                emit(9,"SYS",0,1);
                emit(4,"STO", 0, symbolTable[goodSymbolIndex].address);

            }

            return;


        }

    //////////////////////////////////////////////////////////////////////
        if(tokenList[counter].tokenType == writesym) {

                counter++;
                isExpression(tokenList,symbolTable,sizeOfList);
                
                //emit Write or take input from user
                emit(9,"SYS",0,2);
                return;
        }













}


//DONE
void isExpression(input *tokenList, symbol *symbolTable, int sizeOfList) {


        if(tokenList[counter].tokenType == minussym) {

            counter++;
            isTerm(tokenList,symbolTable,sizeOfList);
            emit(2,"NEG",0,1); //NEG
            while(tokenList[counter].tokenType == plussym || 
                    tokenList[counter].tokenType == minussym) {

                        if(tokenList[counter].tokenType == plussym) {

                            counter ++;
                            isTerm(tokenList,symbolTable,sizeOfList);
                            emit(2,"ADD",0,2); //ADD
                        }

                        else {


                            counter++;
                            isTerm(tokenList,symbolTable,sizeOfList);
                            emit(2,"SUB",0,3); //SUB
                        }
                    }
        }

        else    { 

            if (tokenList[counter].tokenType == plussym) {
                counter++;
            }
            isTerm(tokenList,symbolTable,sizeOfList);
                while(tokenList[counter].tokenType == plussym || 
                    tokenList[counter].tokenType == minussym) {

                        if(tokenList[counter].tokenType == plussym) {

                            counter ++;
                            isTerm(tokenList,symbolTable,sizeOfList);
                            emit(2,"ADD",0,2); //ADD
                        }


                         else {


                            counter++;
                            isTerm(tokenList,symbolTable,sizeOfList);
                            emit(2,"SUB",0,3); //SUB
                        }

                    }
        }
}



//DONE
void isTerm(input *tokenList, symbol *symbolTable, int sizeOfList) {


    isFactor(tokenList,symbolTable,sizeOfList);

    while(tokenList[counter].tokenType == multsym || 
           tokenList[counter].tokenType == slashsym ||
           tokenList[counter].tokenType == modsym) {


               if(tokenList[counter].tokenType == multsym) {
                   counter++;
                   isFactor(tokenList,symbolTable,sizeOfList);
                   emit(2,"MUL",0,4);
               }


               else if(tokenList[counter].tokenType == slashsym) {

                   counter++;
                   isFactor(tokenList,symbolTable,sizeOfList);
                   emit(2,"DIV",0,5);
               }

                else {

                    counter++;
                    isFactor(tokenList,symbolTable,sizeOfList);
                    emit(2,"MOD",0,6);
                }


           }


}









//DONE
void isCondition(input *tokenList, symbol *symbolTable, int sizeOfList) {



    if(tokenList[counter].tokenType == oddsym) {

            counter++;
            isExpression(tokenList,symbolTable,sizeOfList);
            emit(2,"ODD",0,6);
    }

    else {


        if(tokenList[counter].tokenType == eqsym) {

            counter++;
            isExpression(tokenList,symbolTable,sizeOfList);
            emit(2,"EQL",0,8);

        }


       else if(tokenList[counter].tokenType == neqsym) {

            counter++;
            isExpression(tokenList, symbolTable,sizeOfList);
            emit(2,"NEQ",0,9);
        }

        

        else if(tokenList[counter].tokenType == lessym) {

            counter++;
            isExpression(tokenList, symbolTable,sizeOfList);
            emit(2,"LSS",0,10);
        }


        else if(tokenList[counter].tokenType == leqsym) {

            counter++;
            isExpression(tokenList, symbolTable,sizeOfList);
            emit(2,"LEQ",0,11);
        }



        else if(tokenList[counter].tokenType == gtrsym) {

            counter++;
            isExpression(tokenList, symbolTable,sizeOfList);
            emit(2,"GTR",0,12);
        }


        else if(tokenList[counter].tokenType == geqsym) {

            counter++;
            isExpression(tokenList, symbolTable,sizeOfList);
            emit(2,"GEQ",0,13);
        }


        else {

            //Error : condtion must contain comparsion operator

            printf("\n%s\n", errorMessage(13));
        }
    }

}




//DONE
void isFactor(input *tokenList, symbol *symbolTable, int sizeOfList) {



    if(tokenList[counter].tokenType == identsym) {
        symbolIndex = scansSymbolTable(tokenList[counter].str,symbolTable);
        if(symbolIndex == -1) {

            //Error : undelclared symbol
            printf("\n%s\n", errorMessage(7));

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
        
        printf("\nThe string stored: %s\n",tokenList[counter].str);
       
       //emit(1,"LIT",0,symbolTable[symbolIndex].val);
        emit(1,"LIT",0,atoi(tokenList[counter].str));
       counter++;
    }

    else if(tokenList[counter].tokenType == lparentsym) {

        counter++;
        isExpression(tokenList,symbolTable,sizeOfList);
        if(tokenList[counter].tokenType != rparentsym) {

                //Error : right parenthesis must be followed by left parenthesis
                printf("\n%s\n", errorMessage(14));

        }

        else {
            counter++;
        }
    } 

    else {

        //Error : arithmetic equations must contain operands, parentheses,
        //numbers, or symbols
        printf("\n%s\n", errorMessage(15));
    }



}


//DONE
int isvar(input *tokenList, symbol *symbolTable, int sizeOfList) {

        int numVars = 0;
    
    if(tokenList[counter].tokenType == varsym) {
        do {

            numVars++;
            counter++;

            if(tokenList[counter].tokenType != identsym) {

                //Error : const, var, and read keywords must be followed by identifier
                printf("\n%s\n", errorMessage(2));
                break;
            }

             if(scansSymbolTable(tokenList[counter].str,symbolTable) != -1) {

                //Error : symbol name has already been declared
                printf("\n%s\n", errorMessage(3));
                break; // ?

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
        }

        else 
            counter++;

    }

        return numVars;


}









//DONE
void isconst(input *tokenList, symbol *symbolTable, int sizeOfList) {

    char savesIdentifier[50];
    int flagIsConst = 0;


    if(tokenList[counter].tokenType == constsym) {
        flagIsConst = 1;
   

        do {

            counter++;

            if(tokenList[counter].tokenType != identsym) {
            
                //Error : const, var, and read keywords must be followed by identifier
                printf("\n%s\n", errorMessage(2));
                break; // ?
            }



            if(scansSymbolTable(tokenList[counter].str,symbolTable) != -1) {

                    //Error : symbol name has already been declared
                    printf("\n%s\n", errorMessage(3));
                    break; // ?

                }

            else {


                strcpy(savesIdentifier,tokenList[counter].str);
                counter++;


                }


            if(tokenList[counter].tokenType != eqsym) {

                //Error : constants must be assigned with =
                printf("\n%s\n", errorMessage(4));
                break;
                }


            else {
                counter++;
                }



            if(tokenList[counter].tokenType != numbersym) {

                //Error : constants must be assigned an integer value
                printf("\n%s\n", errorMessage(5));
                break;
             }


            else {

        

            symbolTable[symbolIndex].kind = 1;
            strcpy(symbolTable[symbolIndex].name,savesIdentifier);
            symbolTable[symbolIndex].val = (int) tokenList[counter].str - '0';
            symbolTable[symbolIndex].level = 0;
            symbolTable[symbolIndex].address = 0;
            symbolIndex;

            //get next token
            counter++;


    }






    }while(tokenList[counter].tokenType == commasym);

        if(tokenList[counter].tokenType != semicolonsym && flagIsConst == 1) {
            //Error : constanst and varaibles must be followed by semicolons.
            printf("\n%s\n", errorMessage(6));
        }


        else if(flagIsConst == 1) {

            counter++;
        }
  
    }


}



int scansSymbolTable(char *c, symbol *symbolTable ) {

    for(int i = 0; i < 500; i++ )
        if(strcmp(symbolTable[i].name, c) == 0)
                return i;
    return - 1;

}





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