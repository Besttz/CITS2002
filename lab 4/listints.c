#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  comInt(const void *p, const void *q){
    return ( * ( int * ) p - * ( int * ) q);
}
void listint(char * string) {
    int from[10];
    int to[10];
    int pair = 0;
    int len = strlen(string);
    for (int i = 0; i < len; i++)
    {
        if (string[i]==',') continue;
        int j = 0;
        while (string[i+j]!=','&&string[i+j]!='-'&&string[i+1]=='\0') j++;
        int current = 0;
        int ten = 1;
        for (int k = 1; i <= j; i++)
        {
            current += ((int)(string[i+j-k])-48)*ten;
            ten*=10;
        }
        from[pair] = current;
        i+=j;
        if (string[i]==','||string[i]=='\0')
        {
            to[pair] = current;
            pair++;
            
            
        } else if (string[i]=='-')
        {
            j = 0;i++;
            while (string[i+j]!=','&&string[i+j]!='-'&&string[i+1]=='\0') j++;
            current = 0;
            ten = 1;
            for (int k = 1; i <= j; i++)
            {
                current += ((int)(string[i+j-k])-48)*ten;
                ten*=10;
            }
            to[pair] = current;
            pair++;
            i+=j;
        }  

    }
    qsort(from,pair,sizeof(int), comInt);
    qsort(to,pair,sizeof(int), comInt);

    for (int i = 0; i < pair; i++)
    {
        printf("%i ",from[i]);
    }
    printf("\n");
    for (int i = 0; i < pair; i++)
    {
        printf("%i ",to[i]);
    }
    printf("\n");

    
    
}

int main(int argcount, char *argvalue[])
{
    // Check the number of command-line arguments
    if(argcount != 2)
    {
        fprintf(stderr, "Usage: %s value1 [value2 ...]\n", argvalue[0]);
        exit(EXIT_FAILURE);         // Exit indicating failure
    }
    else listint(argvalue[1]);
    return 0;
}
