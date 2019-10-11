#include "sifs-new.h"

// add a copy of a new file to an existing volume
int SIFS_writefile(const char *volumename, const char *pathname,
                   void *data, size_t nbytes)
{
    //  ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if (volumename == NULL || pathname == NULL || data == NULL || nbytes <= 0)
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

    //  FIND THE PARENTS DIR BLOCK
    int parentBlockID = SIFS_pathmatch(volumename, pathname, SIFS_PATH_PARENT_FOR_NEW);
    if (parentBlockID == -1) //THE PARENT BLOCK ISN'T EXIST
    {
        fclose(vol);
        SIFS_errno = SIFS_ENOENT;
        return 1;
    }

    //  VARIABLES DECLARATION
    int fileBlockID = -1;
    int fileNameID = -1;

    //  Read the first header of file.
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    //  Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //  CHECK IF THE PARENT DIR IS FULL
    SIFS_DIRBLOCK parentBlock;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentBlockID, SEEK_SET);
    fread(&parentBlock, sizeof parentBlock, 1, vol);
    if (parentBlock.nentries == SIFS_MAX_ENTRIES)
    {
        fclose(vol);
        SIFS_errno = SIFS_EMAXENTRY;
        return 1;
    }

    //  GET NAME
    char newName[SIFS_MAX_NAME_LENGTH];
    SIFS_lastname(pathname, newName);

    //  CHECK IF THE NEW NAME IS EXCEED MAX LENGTH FOR NAME
    if (strlen(newName) > (SIFS_MAX_NAME_LENGTH - 1))
    {
        fclose(vol);
        SIFS_errno = SIFS_EINVAL;
        return 1;
    }

    //  GET MD5 OF FILE GOING TO WRITE
    void *newMD5raw = calloc(1, MD5_BYTELEN);
    MD5_buffer(data, nbytes, newMD5raw);
    char *newMD5 = MD5_format(newMD5raw);

    //  CHECK MD5 WITH ALL FILES CHECK IF ALREADY EXIST
    for (int i = 0; i < volHeader.nblocks; i++)
    {
        if (bitmap[i] != 'f')
            continue; // SKIP BLOCK NOT FILE
        //  GET THE FILE BLOCK
        SIFS_FILEBLOCK fileBlock;
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
        fread(&fileBlock, sizeof fileBlock, 1, vol);
        if (strcmp(newMD5, (char *)fileBlock.md5) == 0) //MATCH THE MD5
        {
            fileBlockID = i;
            break;
        }
    }

    //CHECK IF FILE EXIST
    if (fileBlockID == -1) //NOT EXIST
    {
        //  CALCULATE HOW MANY BLOCKS NEED
        int dataBlockCount = nbytes / volHeader.blocksize;
        if (volHeader.blocksize * dataBlockCount < nbytes)
            dataBlockCount++;
        //  FIND THE UNUSED BLOCK FROM BITMAP
        int i = 0;
        for (; i < volHeader.nblocks; i++)
            if (bitmap[i] == 'u')
                break; //i is the first unused block ID
        //  CHECK IF ENHOUGH SPACE LEFT
        if (i + dataBlockCount >= volHeader.nblocks)
        {
            fclose(vol);
            SIFS_errno = SIFS_ENOSPC;
            return 1;
        }

        //  MARK THE FILE BLOCK
        fileBlockID = i;
        //  UPDATE THE NEW BITMAP
        bitmap[i] = SIFS_FILE;
        for (int j = 1; j <= dataBlockCount; j++)
            bitmap[i + j] = SIFS_DATABLOCK;
        //  WRITE THE NEW BITMAP TO FILE
        fseek(vol, sizeof volHeader, SEEK_SET);
        fwrite(&bitmap, sizeof bitmap, 1, vol);

        //  BUILD THE FILE BLOCK
        SIFS_FILEBLOCK fileBlock;
        fileBlock.modtime = time(NULL);
        strcpy((char *)fileBlock.md5, newMD5);
        fileBlock.length = nbytes;
        fileBlock.firstblockID = fileBlockID + 1;
        fileBlock.nfiles = 1;
        strcpy(fileBlock.filenames[0], newName);
        fileNameID = 0;

        //  WRITE THE FILE BLOCK INTO FILE
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * fileBlockID, SEEK_SET);
        fwrite(&fileBlock, sizeof fileBlock, 1, vol); // write BLOCK

        //  WRITE THE DATA BLOCK
        fwrite(data, nbytes, 1, vol);
    }
    else //THE FILE IS EXIST
    {
        SIFS_FILEBLOCK fileBlock;
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * fileBlockID, SEEK_SET);
        fread(&fileBlock, sizeof fileBlock, 1, vol);
        fileBlock.modtime = time(NULL);
        fileNameID = fileBlock.nfiles;
        strcpy(fileBlock.filenames[fileNameID], newName);
        fileBlock.nfiles++;

        //  WRITE THE FILE BLOCK INTO FILE
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * fileBlockID, SEEK_SET);
        fwrite(&fileBlock, sizeof fileBlock, 1, vol); // write BLOCK
    }

    //  MODIFY THE PARENT DIR
    parentBlock.modtime = time(NULL);
    parentBlock.entries[parentBlock.nentries].blockID = fileBlockID;
    parentBlock.entries[parentBlock.nentries].fileindex = fileNameID;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentBlockID, SEEK_SET);
    fwrite(&parentBlock, sizeof parentBlock, 1, vol);

    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
