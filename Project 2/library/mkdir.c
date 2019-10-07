#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */


// make a new directory within an existing volume
int SIFS_mkdir(const char *volumename, const char *pathname)
{
    //  ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if(volumename == NULL || pathname == NULL) {
	SIFS_errno	= SIFS_EINVAL;
	return 1;
    }

    //ATTEMPT TO OPEN THE VOLUME - OPEN FOR WRITING
    FILE *vol	= fopen(volumename, "r+");

    //ENSURE THAT THE FILE IS OPENED
    if(vol == NULL) {
	SIFS_errno	= SIFS_ENOTFILE;
	return 1;
    }

//  FINISHED, CLOSE THE VOLUME
    fclose(vol);

//  AND RETURN INDICATING SUCCESS
    return 0;
}
