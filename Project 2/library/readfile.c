#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

// read the contents of an existing file from an existing volume
int SIFS_readfile(const char *volumename, const char *pathname,
                  void **data, size_t *nbytes)
{
    //  ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if (volumename == NULL || pathname == NULL || data == NULL || *nbytes <= 0)
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

    //  FIND THE BLOCK ID OF PATHNAME
    int blockID = SIFS_pathmatch(volumename, pathname, 0);
    if (blockID == -1)
    {
        SIFS_errno = SIFS_ENOENT;
        return 1;
    }

    //  Read the first header of file.
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    //  Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //  CHECK IF IT'S A FILE
    if (bitmap[blockID] != SIFS_FILE)
    {
        SIFS_errno = SIFS_ENOTDIR;
        return 1;
    }

    //  GET THIS BLOCK
    // SIFS_FILEBLOCK block;
    fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * (blockID + 1), SEEK_SET);
    // fread(&block, sizeof block, 1, vol);
    fread(data, *nbytes, 1, vol);

    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
