#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Compile this program with:
//      cc -std=c99 -Wall -Werror -pedantic -o rotate rotate.c

//  The rotate function returns the character ROT positions further along the
//  alphabetic character sequence from c, or c if c is not lower-case

char rotate(char rot, char c)
{
        // Check if c is lower-case or not
        if (islower(c))
        {
                // The ciphered character is ROT positions beyond c,
                // allowing for wrap-around
                return ('a' + (c - 'a' + rot) % 26);
        }
        else
        {
                return  ('A' + (c - 'A' + rot) % 26);
        }
}

//  Execution of the whole program begins at the main function

int main(int argcount, char *argvalue[])
{
        FILE * fp;
        // Exit with an error if the number of arguments (including
        // the name of the executable) is not precisely 2
        if(argcount <= 2)
        {
                fprintf(stderr, "%s: program expected at least 2 argument, but instead received %d\n",
					argvalue[0], argcount-1);
                exit(EXIT_FAILURE);
        }
        else
        {
                int rott = 13;
                // =atoi(argvalue[1]);
                // printf("Current rot: %i\n", rott);
                for (int i = 1; i < argcount; i++)
                {
                        if (!isalpha(argvalue[i][0]))
                        {
                                rott = atoi(argvalue[i]);
                                printf("Changed rot to %i\n\n",rott);

                                continue;

                        }
                        if (fopen(argvalue[i],"r")!=NULL)
                        {
                                fp = fopen(argvalue[i],"r");
                                printf("Opened file %s\n",argvalue[i]);
                                int j = 0;
                                while (1)
                                {
                                        char c = fgetc(fp);
                                        if (feof(fp))
                                        {
                                                break;
                                        }
                                        printf("%i %c %c\n",j++, c, rotate((char)rott,c));

                                }
                                printf("\n");
                                continue;
                        }
                        
                        // Calculate the length of the first argument
                        int length = strlen(argvalue[i]);

                        // Loop for every character in the text
                        for(int j = 0; j < length; j++)
                        {
                                // Determine and print the ciphered character
                                printf("%i %c %c\n",j, argvalue[i][j], rotate((char)rott,argvalue[i][j]));
                        }
                        printf("\n");

                }

                        // Exit indicating success
                        exit(EXIT_SUCCESS);
        }
        return 0;
}
