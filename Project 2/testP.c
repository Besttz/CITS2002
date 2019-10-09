#include <stdio.h>
#include <stdlib.h>
#include "sifs.h"
#include "library/sifs-new.h"


int main(int argcount, char *argvalue[])
{
    char *volumename; // filename storing the SIFS volume
    char *pathname;

    //  ATTEMPT TO OBTAIN THE volumename FROM AN ENVIRONMENT VARIABLE
    if (argcount == 3)
    {
        volumename = argvalue[1];
        pathname = argvalue[2];
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    printf("Block ID of Parent is : %i \n", SIFS_pathmatch(volumename,pathname, 2));    
    printf("Block ID is : %i \n", SIFS_pathmatch(volumename,pathname, 0));
}
