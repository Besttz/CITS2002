#include "sifs-new.h"

int SIFS_lastname(const char *pathname, char* lastname)
{
    int len = strlen(pathname) - 1;
    char tempName[SIFS_MAX_NAME_LENGTH];
    char newName[SIFS_MAX_NAME_LENGTH];
    while (pathname[len] == '/')
        len--;
    for (int i = len; i >= 0; i--)
    {
        if (pathname[i] == '/')
        {
            tempName[len - i] = '\0';
            break;
        }
        tempName[len - i] = pathname[i];
    }
    tempName[len + 1] = '\0';
    int newlen = strlen(tempName) - 1;
    for (int i = 0; i <= newlen; i++)
        newName[i] = tempName[newlen - i];
    newName[newlen + 1] = '\0';
    strcpy(lastname, newName);

    return 0;
}
