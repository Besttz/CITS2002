#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int strsum(char item[]){
    int sum = 0;
    int count = 0;
    do {
        char current = item[count];
        if (current == '\0') return sum;
        else if (isupper(current)) sum+= current-'A'+1;
        else if (islower(current)) sum+= current-'a'+1;
    }while(++count);
    return 0;
}

int my_strcmp(char aa[],  char bb[]){
    int suma = strsum(aa);
    int sumb = strsum(bb);
    if (suma>sumb) return 1;
    else if (suma==sumb) return 0;
    else return -1;
    
}

int main(int argc, char *argv[])
{
    if(argc != 3)
        {
                fprintf(stderr, "%s: program expected 2 argument, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            printf("The comparation is: %i\n", my_strcmp(argv[1],argv[2]));
            return 0;
        }

}
