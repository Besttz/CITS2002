#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
        {
                fprintf(stderr, "%s: program expected at least 1 argument, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            for (int i = 1; i < argc; i++)
            {
                int year = atoi(argv[i]);
                printf ("Year %i :", year);
                if (year%4==0)
                {
                    if (year%400==0 || year%100!=0)
                    {
                        printf ("is a leap year. \n");
                    } else {
                        printf ("is not a leap year. \n");
                    }
                    
                } else{
                    printf ("is not a leap year. \n");
                }
                
            }
            
        }
}
