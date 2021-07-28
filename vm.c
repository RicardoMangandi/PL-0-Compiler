
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define STACK_HEIGHT 500

int base(int stack[], int level, int BP);

void execute(instruction *code, int print)
{

        //initialize variables
    int sp = -1;
    int bp = 0;
    int pc = 0;
    int i = 0;        //counter
    char opString[4]; //to hold instruction name

    instruction *ir = NULL;
    //struct Instruction text[100];
    int stack[STACK_HEIGHT];

    for (int i = 0; i < STACK_HEIGHT; i++)
        stack[i] = 0;

    if (print == 1)
    {
        printf("\n\n");

        printf("                PC   BP   SP   stack");
        printf("\n");
        printf("Initial values: %d %4d %5d", pc, bp, sp);
        printf("\n");
    }

    //some flags to control the printing of '|'
    int halt = 1, callFlag = -1, savesSP[STACK_HEIGHT], oldpc;
    for (int i = 0; i < STACK_HEIGHT; i++)
        savesSP[i] = 0;

    //the instruction register begins to point to the text file or "code"
    //ir = text;
    ir = code;

    while (halt == 1)
    {

        //fetch, copies the content of text into the ir with
        //the following fields: opcode, lexicographical level, and m
        //increments program counter
        ir->opcode = code[pc].opcode;
        ir->lexiLevel = code[pc].lexiLevel;
        ir->m = code[pc].m;
        oldpc = pc;
        pc = pc + 1;

        //execute begins depending on the opcode

        //instruction LIT places value m on top of stack
        if (ir->opcode == 1)
        {

            sp = sp + 1;
            stack[sp] = ir->m;
        }

        //instruction OPR which is dependent on variable m
        if (ir->opcode == 2)
        {

            //instruction RTN pops the AR which is on top of the stack
            if (ir->m == 0)
            {
                stack[bp - 1] = stack[sp];
                sp = bp - 1;
                bp = stack[sp + 2];
                pc = stack[sp + 3];
            }

            //instruction NEG makes the top of the stack negative
            if (ir->m == 1)
            {
                stack[sp] = -1 * stack[sp];
            }

            //instruction ADD adds top of two numbers of stack and
            //places result on top
            if (ir->m == 2)
            {
                sp = sp - 1;
                stack[sp] = stack[sp] + stack[sp + 1];
            }

            //instruction SUB subtracts top of two numbers of stack and
            //places result on top
            if (ir->m == 3)
            {

                sp = sp - 1;
                stack[sp] = stack[sp] - stack[sp + 1];
            }

            //instruction MUL subracts top of two numbers of stack and
            //places result on top
            if (ir->m == 4)
            {

                sp = sp - 1;
                stack[sp] = stack[sp] * stack[sp + 1];
            }

            //instruction DIV divides top of two numbers of stack and
            //places result on top
            if (ir->m == 5)
            {

                sp = sp - 1;
                stack[sp] = stack[sp] / stack[sp + 1];
            }

            //instruction ODD does MOD operation of top value by 2 and
            //places result on top
            if (ir->m == 6)
            {
                stack[sp] = stack[sp] % 2;
            }

            //instruction MOD does the MOD operation OF top values and
            //places result on top
            if (ir->m == 7)
            {

                sp = sp - 1;
                stack[sp] = stack[sp] % stack[sp + 1];
            }

            //instruction EQL checks if two top values on stack are equal
            //if not then places zero on stack or one as true
            if (ir->m == 8)
            {

                sp = sp - 1;

                if (stack[sp] == stack[sp + 1])
                    stack[sp] = 1;
                else
                {
                    stack[sp] = 0;
                }
            }

            //instruction NEQ checks if two top values are not equal
            //if not then places zero on stack or one as true
            if (ir->m == 9)
            {

                sp = sp - 1;

                if (stack[sp] != stack[sp + 1])
                    stack[sp] = 1;

                else
                {
                    stack[sp] = 0;
                }
            }

            //instruction LSS checks if top of stack is less than value below it
            //if not then places zero on stack or one as true
            if (ir->m == 10)
            {
                sp = sp - 1;

                if (stack[sp] < stack[sp + 1])
                    stack[sp] = 1;

                else
                {
                    stack[sp] = 0;
                }
            }

            //instruction LEQ checks if top of stack is less or equal than value below it
            //if not then places zero on stack or one as true
            if (ir->m == 11)
            {

                sp = sp - 1;

                if (stack[sp] <= stack[sp + 1])
                    stack[sp] = 1;

                else
                {
                    stack[sp] = 0;
                }
            }

            //instruction GTR checks if top of stack is greater than value below it
            //if not then places zero on stack or one as true
            if (ir->m == 12)
            {

                sp = sp - 1;

                if (stack[sp] > stack[sp + 1])
                    stack[sp] = 1;

                else
                {
                    stack[sp] = 0;
                }
            }

            //instruction GTR checks if top of stack is greater than or equal to value below it
            //if not then places zero on stack or one as true
            if (ir->m == 13)
            {

                sp = sp - 1;

                if (stack[sp] >= stack[sp + 1])
                    stack[sp] = 1;

                else
                {
                    stack[sp] = 0;
                }
            }
        }

        //instruction LOD loads value from a certain lexilevel down
        //and places it on top of the stack
        if (ir->opcode == 3)
        {

            sp = sp + 1;
            stack[sp] = stack[base(stack, ir->lexiLevel, bp) + ir->m];
        }

        //instruction STO stores value top of the stack and stores it certain
        //levels down based on lexigraphical level
        if (ir->opcode == 4)
        {

            stack[base(stack, ir->lexiLevel, bp) + ir->m] = stack[sp];
            sp = sp - 1;
        }

        //instruction CAL creates a new AR to be placed on top of the stack
        if (ir->opcode == 5)
        {
            stack[sp + 1] = base(stack, ir->lexiLevel, bp);
            stack[sp + 2] = bp;
            stack[sp + 3] = pc;
            stack[sp + 4] = stack[sp];

            bp = sp + 1;
            pc = ir->m;
        }

        //instruction INC increments the sp based on variable m
        if (ir->opcode == 6)
        {
            sp = sp + ir->m;
        }

        //instruction JMP jumps to the command indicated by variable m
        //this changes the pc to variable m
        if (ir->opcode == 7)
        {
            pc = ir->m;
        }

        //instruction JPC jumps to the command indicated by variable m
        //if the top of the stack is zero
        if (ir->opcode == 8)
        {
            if (stack[sp] == 0)
            {
                pc = ir->m;
                sp = sp - 1;
            }
            else
                sp = sp - 1;
        }

        //instruction SYS dependent on variable m
        if (ir->opcode == 9)
        {

            //prints to the user what is on top of the stack
            if (ir->m == 1)
            {

                if (print == 1)
                {
                    printf("Top of Stack Value: %d\n", stack[sp]);
                }

                sp = sp - 1;
            }

            //allows user to enter an integer and stores it on top of stack
            else if (ir->m == 2 && ir->opcode != 5)
            {

                sp = sp + 1;

                if (print == 1)
                {
                    printf("Please Enter an Integer: ");
                    scanf("%d", &stack[sp]);
                    printf("\n");
                }

                else
                {

                    stack[sp] = 0;
                }
            }

            //ends the program setting flag to zero
            else if (ir->m == 3)
            {
                halt = 0;
            }
        }

        //end of execution cycle print statements:

        if (print == 1)
        {
            if (ir->opcode == 1)
            {
                strcpy(opString, "LIT");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 0)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 1)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 2)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 3)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 4)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 5)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 6)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 7)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 8)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 9)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 10)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 11)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 12)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 2 && ir->m == 13)
            {
                strcpy(opString, "OPR");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 3)
            {
                strcpy(opString, "LOD");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 4)
            {
                strcpy(opString, "STO");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 5)
            {
                strcpy(opString, "CAL");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 6)
            {
                strcpy(opString, "INC");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 7)
            {
                strcpy(opString, "JMP");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 8)
            {
                strcpy(opString, "JPC");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 9 && ir->m == 1)
            {
                strcpy(opString, "SYS");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 9 && ir->m == 2)
            {
                strcpy(opString, "SYS");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            if (ir->opcode == 9 && ir->m == 3)
            {
                strcpy(opString, "SYS");
                printf("%2d %s %2d %2d %5d %4d %4d   ", oldpc, opString, ir->lexiLevel, ir->m, pc, bp, sp);
            }

            //to ensure the proper AR records are separated
            int k = 0;
            if (callFlag > -1)
            {

                for (int i = 0; i <= sp; i++)
                {
                    if (savesSP[k] == i && savesSP[k] != 0)
                    {
                        printf("%d ", stack[i]);
                        if (i != sp)
                            printf(" | ");
                        k++;
                    }
                    else
                        printf("%d ", stack[i]);
                }
                printf("\n");
            }

            else
            {
                for (int i = 0; i <= sp; i++)
                    printf("%d ", stack[i]);
                printf("\n");
            }

        } // end of print statement if(print == 1)

        //conditional flag statements that
        //help me print out the stack with the correct position for AR
        if (ir->opcode == 2 && ir->m == 0)
        {
            callFlag = callFlag - 1;
            for (int i = STACK_HEIGHT; i > callFlag; i--)
                savesSP[i] = 0;
        }

        if (ir->opcode == 5)
        {
            callFlag = callFlag + 1;
            savesSP[callFlag] = sp;
        }

    } //end of while loop

} //end of main

//function given by professor Montagne
int base(int stack[], int level, int BP)
{

    int base = BP;
    int counter = level;

    while (counter > 0)
    {

        base = stack[base];
        counter--;
    }

    return base;
}