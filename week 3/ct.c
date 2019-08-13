#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
        {
                fprintf(stderr, "%s: program expected 3 arguments, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            char type = argv[1][0];
            int count = atoi(argv[2]);
            switch (type)
            {
            case 'a':
                for (int i = 1; i <= count; i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        printf("*");
                    }
                    printf("\n");
                }
                break;
            case 'b':
                for (int i = 1; i <= count; i++)
                {
                    for (int j = 0; j < (count-i); j++)
                    {
                        printf(" ");
                    }
                    for (int j = 0; j < i; j++)
                    {
                        printf("*");
                    }
                    printf("\n");
                }
                break;
            case 'c':
                for (int i = 0; i < count; i++)
                {
                    for (int j = 0; j < (count-i); j++)
                    {
                        printf("*");
                    }
                    printf("\n");
                }
                break;
            case 'd':
                for (int i = 1; i <= count; i++)
                {
                    for (int j = 0; j < (count-i); j++)
                    {
                        printf(" ");
                    }
                    printf("*");
                    for (int j = 1; j < i; j++)
                    {
                        printf("**");
                    }
                    printf("\n");
                }
                break;
            default:
                break;
            }
        }
}
