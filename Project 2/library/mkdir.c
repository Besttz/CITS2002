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

    strcpy(dir_block.name,pathname);
    dir_block.modtime = time(NULL);
    dir_block.nentries = 0;

    // FIND THE PARENTS DIR BLOCK
    int parentBlockID = 0;
    // CHECK IF IT'S A SUBDIRECTORY
    const char * currentChar = pathname;
    while(*currentChar == '/') ++currentChar; //SKIP THE FIRST '/'
    while (*currentChar != '\0') // Check any '/' left
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
//下面的内容是多层目录的支持，先把其他的功能写完之后再写这个
    while (parentBlockID == -1) //THIS DIR IS NOT WITHIN ROOTDIR
    {
        //  GET THE FIRST DIR NAME
        char newFolder[SIFS_MAX_NAME_LENGTH];
        for (int i = 0; i < SIFS_MAX_NAME_LENGTH; i++)
        {
            *(newFolder+i) = *currentChar;
            currentChar++;
            if (*currentChar == '/')
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
                
                
            }
             
        }
        
        
         
        //  GET INTO THE FIRST DICTORY
    }
//我们现在假设现在得到的 parentBlockID 就是需要建立新文件夹的目录
    // GET THIS BLOCK
    SIFS_DIRBLOCK parentBlock;
    fseek(vol,sizeof volHeader+sizeof bitmap+volHeader.blocksize*parentBlockID,SEEK_SET);
    fread(&parentBlock,sizeof parentBlock, 1, vol);
    // ADD THE NEW ENTRIES
    parentBlock.entries[parentBlock.nentries].blockID = index;
    // CHANGE THE MODTIME
    parentBlock.modtime = time(NULL);
    parentBlock.nentries++;
    // WRITE IT INTO THE FILE
    fseek(vol,sizeof volHeader+sizeof bitmap+volHeader.blocksize*parentBlockID,SEEK_SET);
    fwrite(&parentBlock, sizeof parentBlock, 1, vol);	// write rootdir
    // MOVE TO THE NEW BLOCK
    fseek(vol,sizeof volHeader+sizeof bitmap+volHeader.blocksize*index,SEEK_SET);
    // WRITE THE NEW BLOCK
    fwrite(&dir_block, sizeof dir_block, 1, vol);	// write rootdir
    //  WRITE BIT MAP
    fseek(vol,sizeof volHeader,SEEK_SET);
    fwrite(&bitmap, sizeof bitmap, 1, vol);	
    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
