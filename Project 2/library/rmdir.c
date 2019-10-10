#include "sifs-new.h"

// remove an existing directory from an existing volume
int SIFS_rmdir(const char *volumename, const char *pathname)
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

    //  CHECK IF THIS PATH LEGAL
    int removeID = SIFS_pathmatch(volumename, pathname, SIFS_PATH_THISONE);
    if (removeID == -1) //CANNOT FIND THIS PATHNAME
    {
        SIFS_errno = SIFS_ENOENT;
        return 1;
    }
    //  GET THE PARENT DIR ID
    int parentID = SIFS_pathmatch(volumename, pathname, SIFS_PATH_PARENT);

    // Read the first header of file.
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    // Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //  CHECK IF THIS PATH IS A DIR
    if (bitmap[removeID] != 'd')
    {
        SIFS_errno = SIFS_ENOTDIR;
        return 1;
    }
    //  CHANGE THIS BLOCK TO UNUSED
    bitmap[removeID] = 'u';
    //  WRIT THE NEW BITMAP
    fseek(vol, sizeof volHeader, SEEK_SET);
    fwrite(&bitmap, sizeof bitmap, 1, vol);

    //  CHECK IF THIS DIR IS EMPTY
    SIFS_DIRBLOCK block;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * removeID, SEEK_SET);
    fread(&block, sizeof removeID, 1, vol);
    if (block.nentries != 0)
    {
        SIFS_errno = SIFS_ENOTEMPTY;
        return 1;
    }

    //  FILL THIS BLOCK WITH 0
    // char clearBlock[volHeader.blocksize];
    memset(&block, 0, sizeof block); // reset to all zeroes
    //  WRITE THE NEW BLOCK INTO FILE
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * removeID, SEEK_SET);
    fwrite(&block, sizeof block, 1, vol); // write rootdir

    // GET THE PARENT BLOCK
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentID, SEEK_SET);
    fread(&block, sizeof block, 1, vol);
    block.nentries--;
    int i = 0;
    for (; i < block.nentries; i++)
    {
        if (block.entries[i].blockID == removeID)
            break;
    }
    // MOVE EVERY ENTRIES FROM THIS ONE MOVE ONE POSITION
    for (; i < block.nentries; i++)
        block.entries[i] = block.entries[i + 1];

     //  WRITE THE NEW BLOCK INTO FILE
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentID, SEEK_SET);
    fwrite(&block, sizeof block, 1, vol); // write rootdir

    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
