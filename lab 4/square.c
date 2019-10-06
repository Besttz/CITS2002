#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

//  Compile with:  cc -std=c99 -Wall -Werror -pedantic -o square square.c

void square(int number)
{
    printf("The square of %i is %i\n", number, number*number);
}

int my_atoi(char * string){
    int result = 0;
    int nega = 0;
    if (string[0] == '-')  nega = 1;
    int len = strlen(string);
    int ten = 1;
    for (int i = len-1; i >=0; i--)
    {
        int current = (int)(string[i]-48);
        if (!nega)
       {
           if (i==len-1) result += current;
           else result += current*ten;
           
       } else
       {
           if (i==0) break;
           if (i==len-1) result -= current;
           else result -= current*ten;
       }
       ten *=10;
    }
    
    return result;
}

void randCard(){
    int cards[52];
    srand(time(0)); 
    for (int i = 0; i < 52; i++)
    {
        int new;
        while (1)
        {
            new = rand()%52;
            int j = 0;
            for (; j < i; j++)
                if (new==cards[j]) break;
            if (j==i) break;
            else continue;  
        }
        cards[i] =new;

    }
    for (int i = 0; i < 13; i++)
    {
        int first = cards[i]%13; //0-12
        int last = cards[i]/13; // 0 1 2 3
        if (first <11&&first>0)  printf("%i",first);
        else if (first==0) printf("K");
        else if (first==11) printf("J");
        else if (first==12) printf("Q");
        switch (last)
        {
        case 0:
            printf("D");
            break;
        case 1:
            printf("C");
            break;
        case 2:
            printf("S");
            break;
        case 3:
            printf("H");
            break;        
        default:
            break;
        }
        printf(" ");
    }
        printf(" \n");

}

int main(int argcount, char *argvalue[])
{
    // Check the number of command-line arguments
    if(argcount < 2)
    {
        fprintf(stderr, "Usage: %s value1 [value2 ...]\n", argvalue[0]);
        exit(EXIT_FAILURE);         // Exit indicating failure
    }
    else
    {
        // Loop over each argument passed to the program
        for(int a = 1 ; a < argcount ; a = a+1)
        {
            if (my_atoi(argvalue[a])==2)
            {
                randCard();
                return 0;
            }
            
            int n = my_atoi(argvalue[a]);  // Convert the a'th argument

            square(n);              // Call our function
        }
    }
    return 0;
}
