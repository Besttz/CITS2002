#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

// get information about a requested directory
int SIFS_dirinfo(const char *volumename, const char *pathname,
                 char ***entrynames, uint32_t *nentries, time_t *modtime)
{
    //  ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if (volumename == NULL || pathname == NULL || entrynames == NULL || nentries == NULL || modtime == NULL)
    {
        SIFS_errno = SIFS_EINVAL;
        return 1;
    }

    //ATTEMPT TO OPEN THE VOLUME - OPEN FOR WRITING
    FILE *vol = fopen(volumename, "r");

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

    int blockID = SIFS_pathmatch(volumename, pathname,0);
    if (blockID == -1)
    {
        SIFS_errno = SIFS_ENOENT;
        return 1;
    }
    
    if (bitmap[blockID] != 'd')
    {
        SIFS_errno = SIFS_ENOTDIR;
        return 1;
    }
    
    SIFS_DIRBLOCK block;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * blockID, SEEK_SET);
    fread(&block, sizeof block, 1, vol);
    *nentries = block.nentries;
    *modtime = block.modtime;
    char **entryname = NULL;
    // char ** entryname = (char **)calloc(*nentries,sizeof(char)*SIFS_MAX_NAME_LENGTH);
    if (block.nentries > 0)
    {
        entryname  = realloc(entryname, block.nentries * sizeof(char)*SIFS_MAX_NAME_LENGTH);

        for (int i = 0; i < block.nentries; i++)
        {
            int checkingBlockID = block.entries[i].blockID;
            if (bitmap[checkingBlockID] == 'd')
            {
                SIFS_DIRBLOCK newBlock;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * checkingBlockID, SEEK_SET);
                fread(&newBlock, sizeof newBlock, 1, vol);
                // entryname[i] = "abc";
                entryname[i] = strdup(newBlock.name);
                // strcpy(entryname + i, newBlock.name);
            }
            else if (bitmap[checkingBlockID] == 'f')
            {
                SIFS_FILEBLOCK newBlock;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * checkingBlockID, SEEK_SET);
                fread(&newBlock, sizeof newBlock, 1, vol);
                // entryname[i] = "abc";
                entryname[i] = strdup(newBlock.filenames[block.entries[i].fileindex]);
                // strcpy(entryname + i, newBlock.filenames[block.entries[i].fileindex]);
            }
        }
    }
    *entrynames = entryname;
    // memcpy(*entrynames,*entryname,sizeof entryname);

    //  FINISHED, CLOSE THE VOLUME

    fclose(vol);

    return 0;
}
