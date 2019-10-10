#include <stdio.h>
#include <stdlib.h>
#include "sifs.h"
#include "library/sifs-new.h"

int main(int argcount, char *argvalue[])
{
    char *volumename; // filename storing the SIFS volume
    char *pathname;
    char *order;

    //  ATTEMPT TO OBTAIN THE volumename FROM AN ENVIRONMENT VARIABLE

    order = argvalue[1];
    volumename = argvalue[2];
    pathname = argvalue[3];

    if (strcmp(order, "mkdir") == 0)
    {
        if (SIFS_mkdir(volumename, pathname) != 0)
        {
            SIFS_perror(argvalue[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(order, "rmdir") == 0)
    {

        if (SIFS_rmdir(volumename, pathname) != 0)
        {
            SIFS_perror(argvalue[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(order, "dirinfo") == 0)
    {
        printf("Luanch ./sifs_dirinfo! \n");
        exit(EXIT_FAILURE);
    }
    else if (strcmp(order, "mkvolume") == 0)
    {
        if (SIFS_mkvolume(volumename, argvalue[2], argvalue[3]) != 0)
        {
            SIFS_perror(argvalue[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(order, "writefile") == 0)
    {
        printf("NOT PREPARED! \n");
        exit(EXIT_FAILURE);
    }
    else if (strcmp(order, "pathmatch") == 0)
    {
        printf("Block ID of Parent is : %i \n", SIFS_pathmatch(volumename, pathname, 2));
        printf("Block ID is : %i \n", SIFS_pathmatch(volumename, pathname, 0));
    }
    return 0;
}
