#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int my_strlen(char item[]){
    int count = 0;
    char current;
    do
    {
        current = item[count++];
    } while (current != '\0');
    return count-1;
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        {
                fprintf(stderr, "%s: program expected 1 argument, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            printf("The length is: %i\n", my_strlen(argv[1]));
            return 0;
        }

}
