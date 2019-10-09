#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

// make a new directory within an existing volume
int SIFS_mkdir(const char *volumename, const char *pathname)
{
    //  ENVIRONMENT CHECK
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
    int len = strlen(pathname) - 1;
    // printf("LENTH OF PATHNAME -1 : %i \n", len); // FOR DEBUG HAVE TO DELETEEEEEEEEEEEEEEEEE!
    char tempName[SIFS_MAX_NAME_LENGTH];
    char newName[SIFS_MAX_NAME_LENGTH];
    while (pathname[len] == '/') len--;
    for (int i = len; i >= 0; i--)
    {
        if (pathname[i] == '/')
        {
            tempName[len - i] = '\0';
            break;
        }
        tempName[len - i] = pathname[i];
    }
    tempName[len+1] = '\0';
    // printf("tempName: %s \n", tempName); // FOR DEBUG HAVE TO DELETEEEEEEEEEEEEEEEEE!
    int newlen = strlen(tempName) - 1;
    for (int i = 0; i <= newlen; i++)
        newName[i] = tempName[newlen - i];
    newName[newlen+1] = '\0';

    strcpy(dir_block.name, newName);
    printf("NEW NAME IS: %s \n", newName); // FOR DEBUG HAVE TO DELETEEEEEEEEEEEEEEEEE!
    dir_block.modtime = time(NULL);
    dir_block.nentries = 0;

    //  DONE
    // FIND THE PARENTS DIR BLOCK
    int parentBlockID = SIFS_pathmatch(volumename, pathname, SIFS_PATH_PARENT_FOR_NEW);
    //我们现在假设现在得到的 parentBlockID 就是需要建立新文件夹的目录
    // GET THIS BLOCK
    SIFS_DIRBLOCK parentBlock;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentBlockID, SEEK_SET);
    fread(&parentBlock, sizeof parentBlock, 1, vol);
    // ADD THE NEW ENTRIES
    parentBlock.entries[parentBlock.nentries].blockID = index;
    // CHANGE THE MODTIME
    parentBlock.modtime = time(NULL);
    parentBlock.nentries++;
    // WRITE IT INTO THE FILE
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentBlockID, SEEK_SET);
    fwrite(&parentBlock, sizeof parentBlock, 1, vol); // write rootdir
    // MOVE TO THE NEW BLOCK
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * index, SEEK_SET);
    // WRITE THE NEW BLOCK
    fwrite(&dir_block, sizeof dir_block, 1, vol); // write rootdir
    //  WRITE BIT MAP
    fseek(vol, sizeof volHeader, SEEK_SET);
    fwrite(&bitmap, sizeof bitmap, 1, vol);
    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
