#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

// remove an existing file from an existing volume
int SIFS_rmfile(const char *volumename, const char *pathname)
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

    //  CHECK IF THIS PATH IS A FILE
    if (bitmap[removeID] != SIFS_FILE)
    {
        SIFS_errno = SIFS_ENOTDIR;
        return 1;
    }
    //  GET THIS BLOCK
    SIFS_FILEBLOCK fileBlock;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * removeID, SEEK_SET);
    fread(&fileBlock, sizeof fileBlock, 1, vol);

    //  GET THE PARENT BLOCK
    SIFS_DIRBLOCK parentBlock;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentID, SEEK_SET);
    fread(&parentBlock, sizeof parentBlock, 1, vol);
    //  UPDATE MODTIME
    parentBlock.modtime = time(NULL);
    //  GET THIS FILENAME ID
    int fileindex = -1;
    int i = 0;
    for (; i < parentBlock.nentries; i++)
    {
        if (parentBlock.entries[i].blockID == removeID)
        {
            //CHECK IF THIS IS THE RIGHT FILE
            char filename[SIFS_MAX_NAME_LENGTH];
            SIFS_lastname(pathname, filename);
            int thisFileindex = parentBlock.entries[i].fileindex;
            if (strcmp(filename, fileBlock.filenames[thisFileindex]) == 0)
            {
                fileindex = thisFileindex;
                break;
            }
        }
    }
    //  MOVE ALL THE LEFT ENTRIES
    for (; i < parentBlock.nentries - 1; i++)
        parentBlock.entries[i] = parentBlock.entries[i + 1];
    parentBlock.nentries--;
    //  WRITE THE NEW BLOCK INTO FILE
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * parentID, SEEK_SET);
    fwrite(&parentBlock, sizeof parentBlock, 1, vol); // write rootdir

    //  CHECK IF THIS IS THE ONLY FILE ACCESS
    if (fileBlock.nfiles == 1) // NEED TO CLEAR THE FILE AND DATA
    {
        //  CALCULATE HOW MANY BLOCKS IS DATABLOCK
        int dataBlockCount = fileBlock.length / volHeader.blocksize;
        if (volHeader.blocksize * dataBlockCount < fileBlock.length)
            dataBlockCount++;
        //  CHANGE BITMAP FOR FILE AND DATA BLOCK
        for (int i = 0; i <= dataBlockCount; i++)
            bitmap[removeID + i] = 'u';
        //  WRITE THE NEW BITMAP TO FILE
        fseek(vol, sizeof volHeader, SEEK_SET);
        fwrite(&bitmap, sizeof bitmap, 1, vol);

        //  FILL THESE BLOCKS WITH 0
        memset(&fileBlock, 0, sizeof fileBlock); // reset to all zeroes
        //  WRITE THE NEW BLOCKS INTO FILE
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * removeID, SEEK_SET);
        for (int i = 0; i <= dataBlockCount; i++)
            fwrite(&fileBlock, volHeader.blocksize, 1, vol);
        //  FINISHED, CLOSE THE VOLUME
        fclose(vol); // MOVE HERE TO AVIOD FILE BUFFER WRITE INTO FILE AGAIN AFTER defrag

        //  FILE DEFRAG
        if (SIFS_defrag(volumename) != 0)
            return 1;
    }
    else
    {
        //  MOVE ALL THE LEFT FILENAME
        for (int i = fileindex; i < fileBlock.nfiles - 1; i++)
            strcpy(fileBlock.filenames[i], fileBlock.filenames[i + 1]);
        // fileBlock.filenames[fileBlock.nfiles-1] = NULL;
        fileBlock.nfiles--;
        fileBlock.modtime = time(NULL);
        //  WRITE THE FILE BLOCK INTO FILE
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * removeID, SEEK_SET);
        fwrite(&fileBlock, sizeof fileBlock, 1, vol); // write BLOCK

        //  UPDATE ALL THE FILEINDEX TO THIS FILE
        for (i = 0; i < volHeader.nblocks; i++)
        {
            if (bitmap[i] != SIFS_DIR)
                continue;
            //  GET THE BLOCK
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
            fread(&parentBlock, sizeof parentBlock, 1, vol);
            if (parentBlock.nentries == 0)
                continue; // SKIP DIR WITH NO ENTRY
            for (int j = 0; j < parentBlock.nentries; j++)
                if (parentBlock.entries[j].blockID == removeID && parentBlock.entries[j].fileindex > fileindex)
                    parentBlock.entries[j].fileindex--;

            //  WRITE THE BLOCK BACK
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
            fwrite(&parentBlock, sizeof parentBlock, 1, vol);
        }

        //  FINISHED, CLOSE THE VOLUME
        fclose(vol);
    }
    //  AND RETURN INDICATING SUCCESS
    return 0;
}
