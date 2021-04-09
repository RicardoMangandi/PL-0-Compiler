
//The beginning of the lex.c file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"



//functions used
int isSpecialSymbol(char c);
int isReservedWord (char *string);
char *errorType (char *string);


//start of main
input *lex_analyze (char *getsCharList, int print) {

    //will use to store token information
    input *list = malloc(500 * sizeof(input));
    int i = 0;


    //variable declaration
    char *error1 = "Error : Identifiers cannot begin with a digit";
    char *error2 = "Error : Identifier names cannot exceed 11 characters";
    char *error3 = "Error : Numbers cannot exceed 5 digits";
    char *error4 = "Error : Invalid Symbol";
   
    


//helps decide where to loop to, to eliminate invisible characters
    int loopTo = strlen(getsCharList);
    int k = loopTo - 1;
    for(int i = k; i > 0; i--) {

        if(iscntrl(getsCharList[i]) || isspace(getsCharList[i]) || getsCharList[i] == EOF || getsCharList[i] == '\0')
            loopTo--;
        
        else
            break;   
    }





    
    //initialized all chars as white space and tokenType to -9999
    for(int i = 0; i < 500; i++) {
        list[i].str[0] = 32;
        list[i].tokenType = -9999;
        
    }

    //variables to keep track of everything in my data structure
    int counter = 0;
    int a = 0;
    int b = 0;


    

   
    //loop reogranizes everything in a data structure struct array called list containing a string variable,
    //int length of string, and the token type
    while(counter < loopTo) {


        if(isalpha(getsCharList[counter]) || isdigit(getsCharList[counter])) {

               
                list[a].str[b] = getsCharList[counter];

                counter++;
                b++;
                

                while (isalpha(getsCharList[counter]) || isdigit(getsCharList[counter])) {
                    
                   
                    list[a].str[b] = getsCharList[counter];
                    b++;
                    counter++;
                  

                   
                }

                list[a].str[b] = '\0';
                a++;
                b = 0;

                }



        //takes care of all invisible characters
        if(isspace(getsCharList[counter]) || iscntrl(getsCharList[counter]) || getsCharList[counter] == EOF) {
                
            while(isspace(getsCharList[counter]) || iscntrl(getsCharList[counter]) || getsCharList[counter] == EOF) {
                    if(!(isspace(getsCharList[counter]) || iscntrl(getsCharList[counter]) || getsCharList[counter] == EOF)) {
                        a++;
                        b = 0;
                        break;
                    }

                    else {

                        counter++;
                        
                    }
                }

             
                     
            }

        //takes care of special symbols
        if(isSpecialSymbol(getsCharList[counter])) {
            
            if(!isspace(getsCharList[counter])) {
            list[a].str[b] = getsCharList[counter];
            counter++;

            }
            
            if(list[a].str[0] == ':' || list[a].str[0] == '<' || list[a].str[0] == '>') {

                    //takes care of := 
                    if(list[a].str[0] == ':' && getsCharList[counter] == '=') {
                        
                        if(!isspace(getsCharList[counter])) {
                        list[a].str[1] = '=';
                        a++;
                        b = 0;
                        counter++;
                        }
                    }

                    //takes care of <=
                    if(list[a].str[0] == '<' && getsCharList[counter] == '=') {
                        if(!isspace(getsCharList[counter])) {
                        list[a].str[1] = '=';
                        a++;
                        b = 0;
                        counter++;
                        }
                    }

                    //takes care of >=
                    if(list[a].str[0] == '>' && getsCharList[counter] == '=') {
                        if(!isspace(getsCharList[counter])) {
                        list[a].str[1] = '=';
                        a++;
                        b = 0;
                        counter++;
                        }
                    }

                
                    //takes care of <>
                    if(list[a].str[0] == '<' && getsCharList[counter] == '>') {
                        if(!isspace(getsCharList[counter])) {
                        list[a].str[1] = '>';
                        a++;
                        b = 0;
                        counter++;
                        }

                    }
            }

            //takes care of comments
            if(list[a].str[0] == '/') {

                if(getsCharList[counter] == '*') {

                    while(getsCharList[counter] != '/') {
                        counter++;
                    }

                    counter++;
                }

                else {
                    a++;
                    b = 0;
                }
            }

            
            else if(list[a].str[0] != 32) {
                            a++;
                            b = 0;
                
            }
            

        }

       


        //catches any invalid symbols
        if(getsCharList[counter] != '\0' && !isSpecialSymbol(getsCharList[counter]) && !(isspace(getsCharList[counter]) || iscntrl(getsCharList[counter])) && !(isalpha(getsCharList[counter]) || isdigit(getsCharList[counter]))) {

            if(!isspace(getsCharList[counter]) && getsCharList[counter] != '\0') {
            list[a].str[0] = getsCharList[counter];
            a++;
            b = 0;
            counter++;
            }
        }
    }
    //end of while loop




   //loop checks for errors in the struct array that contains all reserved words, special symbols,
   //digits, invalid symbols etc.
    for(int i = 0; i < a; i++) {


        

        //identifier cannot begin with a digit, tokenType is -9999 because it is an error
        if(strcmp(errorType(list[i].str),error1) == 0) {
            list[i].tokenType = -9999;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));



        }

        //variable does start with a letter
        else if(isalpha(list[i].str[0]) && strlen(list[i].str) <= 11) {

            list[i].tokenType = identsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));



        }

        //name is too long
        if(strcmp(errorType(list[i].str),error2) == 0) {
            
            list[i].tokenType = -9999;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));


        }


        //name is good length
        else if(isalpha(list[i].str[0]) && strlen(list[i].str) <= 11) {
            
            list[i].tokenType = identsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));


        }

        //number is too long
        if(strcmp(errorType(list[i].str),error3)==0) {

            list[i].tokenType = -9999;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));

        }

        //number is good length
        else if (strlen(list[i].str) <= 5 && isdigit(list[i].str[0]) && !isalpha(list[i].str[1])) {

            list[i].tokenType = numbersym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));

        }

        //invalid symbols
        if(strcmp(errorType(list[i].str),error4)==0) {
            list[i].tokenType = -9999;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));

        }

        //valid symbols
        else  {

                 if(strcmp(list[i].str, ":=") == 0) {

                list[i].tokenType = becomessym;
                list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                strcpy(list[i].errorOrNoerror,errorType(list[i].str));

                }
        

                if(strcmp(list[i].str, "<>") == 0) {

                    list[i].tokenType = neqsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                 }


                if(strcmp(list[i].str, "<=") == 0) {

                    list[i].tokenType = leqsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, ">=") == 0) {

                    list[i].tokenType = geqsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, "%") == 0) {

                    list[i].tokenType = modsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, "+") == 0) {

                    list[i].tokenType = plussym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, "-") == 0) {

                    list[i].tokenType = minussym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }
                

                if(strcmp(list[i].str, "*") == 0) {

                    list[i].tokenType = multsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, "/") == 0) {

                    list[i].tokenType = slashsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, "odd") == 0) {

                    list[i].tokenType = oddsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, "=") == 0) {

                    list[i].tokenType = eqsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, "<") == 0) {

                    list[i].tokenType = lessym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, ">") == 0) {

                    list[i].tokenType = gtrsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, "(") == 0) {

                    list[i].tokenType = lparentsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, ")") == 0) {

                    list[i].tokenType = rparentsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, ",") == 0) {

                    list[i].tokenType = commasym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }


                if(strcmp(list[i].str, ";") == 0) {

                    list[i].tokenType = semicolonsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                if(strcmp(list[i].str, ".") == 0) {

                    list[i].tokenType = periodsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

                }

        if(isReservedWord(list[i].str) == 0) {
                    list[i].tokenType = constsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

        if(isReservedWord(list[i].str) == 1) {
                    list[i].tokenType = varsym;
                    list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
                    strcpy(list[i].errorOrNoerror,errorType(list[i].str));
                }

        if(isReservedWord(list[i].str) == 2) {
            list[i].tokenType = beginsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

        if(isReservedWord(list[i].str) == 3) {
            list[i].tokenType = endsym; 
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

        
        if(isReservedWord(list[i].str) == 4) {
            list[i].tokenType = procsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }


        if(isReservedWord(list[i].str) == 5) {
            list[i].tokenType = callsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

        if(isReservedWord(list[i].str) == 6) {
            list[i].tokenType = ifsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }
 
        if(isReservedWord(list[i].str) == 7) {
            list[i].tokenType = thensym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }


        if(isReservedWord(list[i].str) == 8) {
            list[i].tokenType = elsesym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

 
        if(isReservedWord(list[i].str) == 9) {
            list[i].tokenType = whilesym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }


        if(isReservedWord(list[i].str) == 10) {
            list[i].tokenType = dosym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

        if(isReservedWord(list[i].str) == 11) {
            list[i].tokenType = readsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));

        }


        if(isReservedWord(list[i].str) == 12 ) {
            list[i].tokenType = writesym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

        if(isReservedWord(list[i].str) == 13) {
            list[i].tokenType = oddsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }
        

        if(isReservedWord(list[i].str) == 14) {
            list[i].tokenType = returnsym;
            list[i].errorOrNoerror = (char*) malloc (sizeof(char) * (strlen(errorType(list[i].str)) + 1));
            strcpy(list[i].errorOrNoerror,errorType(list[i].str));
        }

    }
 



if(print == 1) {
printf("Lexeme Table:\n");
printf("       lexeme    token type\n");


//a is the value that was the counter in our while loop which was a < loopTo,
//a is the size of my struct array 
for(int i = 0; i < a; i++) {
    
   if(list[i].tokenType > 0) 
        printf("%11s %10d\n", list[i].str, list[i].tokenType);
   
   else {

       if(strcmp(list[i].errorOrNoerror,"invisible") != 0)
            printf("%s\n", list[i].errorOrNoerror);
            exit(0);
   }
    

}


printf("\n");
printf("Token List:\n");




for(int i = 0; i < a; i++) {

    //if it an identifier or number 
    if(list[i].tokenType == 2 || list[i].tokenType == 3) {
         printf(" %d %s ", list[i].tokenType, list[i].str);

    }

    else  if(list[i].tokenType != -9999){

        printf(" %d ", list[i].tokenType);



    }        
    
}

return list;
}

else {
return list;
}


} //end of main


//if there is no error to detect then it will return the original string passed in
//if the passed in string is an error it will return the error string type
char *errorType (char *string) {


    char *error1 = "Error : Identifiers cannot begin with a digit";
    char *error2 = "Error : Identifier names cannot exceed 11 characters";
    char *error3 = "Error : Numbers cannot exceed 5 digits";
    char *error4 = "Error : Invalid Symbol";

    if(isdigit(string[0]) && isalpha(string[1])) 
        return error1;

    if(isalpha(string[0]) && strlen(string) > 11)
            return error2;

    if(isdigit(string[0]) && strlen(string) > 5)
            return error3;

    if(strlen(string) == 1 && !isalpha(string[0]) && !isdigit(string[0]) && !isSpecialSymbol(string[0])
        && !isspace(string[0]) && !iscntrl(string[0]) && string[0] != EOF) 
            return error4;



    return string; //no errors found
}

//function will check if the char value passed in is a special symbol
//will return 1 if true or 0 if false
int isSpecialSymbol (char c) {

    char symbols[]= {'+', '-', '*', '/', '(', ')', '=', ',' , '.', '<', '>',  ';'  , '%',':' };

    for(int i = 0; i < strlen(symbols); i++) {

        if(c == symbols[i])
            return 1;
    }
    return 0;
}


//function will check if the string if the string value passed in is a reserved word
//will return the index where it is located if true or -9999 if false
int isReservedWord (char *string) {

    
    char reservedWords[][15] = {"const", "var", "begin", "end", "procedure", "call", "if", "then", "else", "while","do", "read","write", "odd", "return"};

    for(int i = 0; i < 13; i++) {
        if(strcmp(string,reservedWords[i]) == 0)
            return i;
    }

    return -9999;
}



