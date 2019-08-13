#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 4)
        {
                fprintf(stderr, "%s: program expected 3 arguments, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            int max = 0 ;
            for (int i = 1; i < argc; i++)
            {
                int number = atoi(argv[i]);
                if (number > max)
                {
                   max = number;
                }                
                
            }
            printf("The max number is %i \n",max);
        }
}
