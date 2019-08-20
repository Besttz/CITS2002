#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int isSafe(char item[]){
    int upper = 0;
    int lower = 0;
    int digit = 0;
    int count = 0;

    while (1) {
        char current = item[count++];
        if (current == '\0') return(upper>=2&&lower>=2&&digit>=2);
        if (isupper(current)) upper++;
        else if (islower(current)) lower++;
        else if (isdigit(current)) digit++;       
    } 
    return 0;
    
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
            printf("The length safety is: %i\n", isSafe(argv[1]));
            return 0;
        }

}
