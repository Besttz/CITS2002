#include "sifs-new.h"
/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

//  TO FIND THE NAME OF THE FOLDER OR FILE OF THIS PATH
int SIFS_lastname(const char *pathname, char *lastname)
{
    //  GET THE TEMP NAME IN REVERSE
    int len = strlen(pathname) - 1;
    char tempName[SIFS_MAX_NAME_LENGTH];
    char newName[SIFS_MAX_NAME_LENGTH];
    //  SKIP THE '/' AT THE END
    while (pathname[len] == '/')
        len--;
    //  GET THE NAME FROM THE END TO THE '/'
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
    //  REVERSE THE TEMP NAME TO GET RIGHT NAME
    int newlen = strlen(tempName) - 1;
    for (int i = 0; i <= newlen; i++)
        newName[i] = tempName[newlen - i];
    newName[newlen + 1] = '\0';
    //  PUT IT INTO THE POINTER.
    strcpy(lastname, newName);

    return 0;
}
