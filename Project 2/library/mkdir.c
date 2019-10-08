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
    if (volumename == NULL || pathname == NULL)
    {
        SIFS_errno = SIFS_EINVAL;
        return 1;
    }

    //ATTEMPT TO OPEN THE VOLUME - OPEN FOR WRITING
    FILE *vol = fopen(volumename, "r+");

    //ENSURE THAT THE FILE IS OPENED
    if (vol == NULL)
    {
        SIFS_errno = SIFS_ENOTFILE;
        return 1;
    }

    // Read the first header of file.
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    // Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //Check which block is unused
    int index = 0;
    for (index = 0; index < volHeader.nblocks; index++)
        if (bitmap[index] == 'u')
            break;
    if (index == volHeader.nblocks)
    {
        SIFS_errno = SIFS_ENOSPC;
        return 1;
    }

    //Change the bitmap as 'd'
    bitmap[index] = 'd';

    //Generate new dir block
    SIFS_DIRBLOCK dir_block;
    memset(&dir_block, 0, sizeof dir_block); // cleared to all zeroes

    dir_block.name[0] = pathname;
    dir_block.modtime = time(NULL);
    dir_block.nentries = 0;

    // FIND THE PARENTS DIR BLOCK
    int parentBlockID = 0;
    // CHECK IF IT'S A SUBDIRECTORY
    char * currentChar = pathname;
    while(*currentChar == '/') ++currentChar; //SKIP THE FIRST '/'
    while (*currentChar != '/0') // Check any '/' left
    {
        if (*currentChar == '/')
        {
            parentBlockID = -1;
            break;
        }
        ++currentChar;
    }
    currentChar = pathname;
    while(*currentChar == '/') ++currentChar;
    int currentCheckingBlock = 0;
    while (parentBlockID == -1) //THIS DIR IS NOT WITHIN ROOTDIR
    {
        //  GET THE FIRST DIR NAME
        char newFolder[SIFS_MAX_NAME_LENGTH];
        for (int i = 0; i < SIFS_MAX_NAME_LENGTH; i++)
        {
            *(newFolder+i) = *currentChar;
            currentChar++;
            if (*currentChar = '/')
            {
               *(newFolder+i+1) = '\0';
               break;
            }
        } // NOW WE HAVE THE NAME OF CURRENT DIR TO FIND
        //CHECK THE CURRENT CHECKING BLOCK AND FIND ALL SUB
        SIFS_DIRBLOCK checking_dir_block;
        fseek(vol,sizeof volHeader+sizeof bitmap+volHeader.blocksize*currentCheckingBlock,SEEK_SET);
        fread(&checking_dir_block,sizeof checking_dir_block, 1, vol);
        // NOW CHECK ITS ENTRIES WHICH IS DIRCTORY
        for (int i = 0; i < SIFS_MAX_ENTRIES; i++)
        {
            if (bitmap[checking_dir_block.entries[i].blockID]=='d')
            {
                /* code */
            }
             
        }
        
        
         
        //  GET INTO THE FIRST DICTORY
    }

    // GET THIS BLOCK
    // ADD THE NEW ENTRIES
    // CHANGE THE MODTIME
    // WRITE IT INTO THE FILE
    // MOVE TO THE NEW BLOCK
    // WRITE THE NEW BLOCK

    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
