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
        if (SIFS_mkvolume(volumename, atoi(argvalue[2]), atoi(argvalue[3])) != 0)
        {
            SIFS_perror(argvalue[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(order, "writefile") == 0)
    {
        FILE *vol = fopen(pathname, "r");
        if (vol == NULL)
        {
            printf("%s: CAN'T OPEN THE FILE. \n", argvalue[0]);
            return 1;
        }
        printf("OPENED! \n");
        // GET THE SIZE OF FILE TO WHITE
        fseek(vol, 0L, SEEK_END);
        int size = ftell(vol);
        printf("GET SIZE! %i \n",size);

        if (SIFS_writefile(volumename, argvalue[4], vol, size) != 0)
        {
            SIFS_perror(argvalue[0]);
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(order, "pathmatch") == 0)
    {
        printf("Block ID of Parent is : %i \n", SIFS_pathmatch(volumename, pathname, 2));
        printf("Block ID is : %i \n", SIFS_pathmatch(volumename, pathname, 0));
    }
    else if (strcmp(order, "defrag") == 0)
    {
        SIFS_defrag(volumename);
    }

    return 0;
}
