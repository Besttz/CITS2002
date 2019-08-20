#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int indexOfMax = 0;
int size = 0;

int findMax(int array[]){
    int max = 0;
    int* new = array;
    printf("POINTER LEENGTH %lu ",sizeof(new));
    // int size = sizeof(new)/sizeof(array[0]);
    for (int i = 0; i < size; i++)
    {
        if (array[i]>max) {
            max = array[i];
            indexOfMax = i;
        }
    }
    printf("%i ",max);
    return max;
    
}

int main(int argc, char *argv[])
{
    int array[10];
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        array[i] = rand();
        printf("%i ",array[i]);
    }
    printf("\n");
    int new[10];
    size = sizeof(array)/sizeof(array[0]);
    new[0]=findMax(array);
    for (int i = 0; i < indexOfMax; i++)
    {
        new[i+1] = array[i];
        printf("%i ",new[i+1]);
    }
    int size = sizeof(array)/sizeof(array[0]);
    for (int i = indexOfMax+1; i < size; i++) {
        new[i] = array[i];
        printf("%i ",new[i]);
    }
    printf("\n");
    return 0;
    
}
