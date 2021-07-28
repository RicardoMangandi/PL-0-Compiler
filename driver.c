#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

int main(int argc, char **argv)
{
    //variable declarations
    int lflag = 0, aflag = 0, vflag = 0;
    FILE *ifp;
    char *inputfile;
    char c;
    int i;
    int sizeOfTokenList;

    //"In" and "out" operation performed when no directives are given
    if (argc == 2)
    {

        printf("\nPlease enter an integer: ");

        int value = 0;

        scanf("%d", &value);

        printf("Top of Stack Value: %d\n", value);
        return 0;
    }

    //Error message if no file name is provided
    if (argc < 2)
    {
        printf("Error : please include the file name");
        return 0;
    }

    //Turns on all directives
    else if (argc > 4)
    {
        lflag = 1;
        aflag = 1;
        vflag = 1;
    }

    //Turns on some directives
    else if (argc == 3 || argc == 4)
    {
        if (argv[2][1] == 'l')
            lflag = 1;
        else if (argv[2][1] == 'a')
            aflag = 1;
        else
            vflag = 1;

        if (argc == 4)
        {
            if (argv[3][1] == 'l')
                lflag = 1;
            else if (argv[3][1] == 'a')
                aflag = 1;
            else
                vflag = 1;
        }
    }

    ifp = fopen(argv[1], "r");

    inputfile = malloc(1000 * sizeof(char));
    c = fgetc(ifp);
    i = 0;

    //reads file input
    while (1)
    {

        inputfile[i++] = c;
        c = fgetc(ifp);
        if (c == EOF)
            break;
    }

    inputfile[i] = '\0';

    //closes file
    fclose(ifp);

    //executions begin
    input *list = lex_analyze(inputfile, lflag);

    //printf("\n%d\n", list[0].sizeOfList);

    instruction *code = parse(list, aflag);
    execute(code, vflag);

    return 0;
}