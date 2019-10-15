#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */

//  CLEAR ALL THE FRAGMENT IN VOLUME FILE
int SIFS_defrag(const char *volumename)
{
    //  ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if (volumename == NULL)
    {
        SIFS_errno = SIFS_EINVAL;
        return 1;
    }

    //  ATTEMPT TO OPEN THE VOLUME - OPEN FOR WRITING
    FILE *vol = fopen(volumename, "r+");

    //ENSURE THAT THE FILE IS OPENED
    if (vol == NULL)
    {
        SIFS_errno = SIFS_ENOTFILE;
        return 1;
    }

    //  Read the first header of file.
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    //  Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //  Keep cleanning fragment until break
    while (1) 
    {
        int i = volHeader.nblocks - 1;
        int foundThings = 0;
        int fragBegin = -1;
        int fragEnd = -1;

        //  CHECK ANY FRAGMENGT LEFT
        for (; i >= 0; i--)
        {
            if (foundThings == 0 && bitmap[i] != 'u')
                foundThings = 1;  //FIRST TIME FOUND BLOCK NOT BLANK
            else if (foundThings) //ALREADY INSIDE THE USED BLOCKS
            {
                if (fragEnd == -1 && bitmap[i] == 'u')
                    fragEnd = i; // FIRST TIME FOUND A BLANK
                else if (fragEnd != -1 && bitmap[i] != 'u')
                {
                    fragBegin = i + 1;
                    break;
                }
            }
        }

        //  FOUND A FRAGMENT OR ALL BLOCK CHECKED
        if (i == -1) //ALL BLOCK CHECKED
        {
            if (foundThings)
            {
                if (fragEnd == -1)
                    break; // NO FRAGMENT
                else
                    fragBegin = 0; // ALL BLOCK BEFORE fragEnd is Frag
            }
            else
                break; // THIS IS A BLANK VOLUME
        }

        //  GOING TO MANAGE THE FRAGMENT (WE HAVE THE BEGIN AND THE END)
        //  MAKE THE NEW BITMAP
        int fragLen = fragEnd - fragBegin + 1;
        for (i = fragBegin; i < volHeader.nblocks - fragLen; i++)
            bitmap[i] = bitmap[i + fragLen];

        //  NOW ALL THE BLOCKS AFTER fragEnd MOVE TO THE BEGINNNING

        //  FILL THE LEFT BITMAP WITH BLANK
        for (; i < volHeader.nblocks; i++)
            bitmap[i] = 'u';

        // WRITE THE NEW BITMAP INTO THE FILE
        fseek(vol, sizeof volHeader, SEEK_SET);
        fwrite(&bitmap, sizeof bitmap, 1, vol);

        // HANDLE THE BLOCKS OF THE FILE
        // USE A LOOP TO AVOID TOO MANY THINGS READ INTO MEMORY
        // THE SPACE TO STORE THE BLOCK TO MOVE
        char oneblock[volHeader.blocksize];

        for (i = fragBegin; i < volHeader.nblocks - fragLen; i++)
        {
            // GET THE BLOCK READY TO MOVE
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * (i + fragLen), SEEK_SET);
            fread(oneblock, sizeof oneblock, 1, vol);
            // WRITE THE BLOCK TO NEW POSITION
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
            fwrite(oneblock, sizeof oneblock, 1, vol);
        }
        //  FILL THE LEFT FLIE WITH BLANK
        memset(oneblock, 0, sizeof oneblock); // reset to all zeroes
        for (; i < volHeader.nblocks; i++)
            fwrite(oneblock, sizeof oneblock, 1, vol);

        //  NOW NEED TO CHANGE ALL THE BLOCK ID OF ENTRIES
        //  ALSO THE ' firstblockID' OF FILE BLOCK
        SIFS_DIRBLOCK block;   // TO STORE DIR BLOCK GOING TO BE CHANGED
        SIFS_FILEBLOCK fBlock; // TO STORE FILE BLOCK GOING TO BE CHANGED

        for (i = 0; i < volHeader.nblocks; i++)
        {
            if (bitmap[i] == 'd')
            {
                //  GET THE BLOCK
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
                fread(&block, sizeof block, 1, vol);
                if (block.nentries == 0)
                    continue; // SKIP DIR WITH NO ENTRY
                for (int j = 0; j < block.nentries; j++)
                    if (block.entries[j].blockID > fragBegin)
                        block.entries[j].blockID -= fragLen;
                //  WRITE THE BLOCK BACK
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
                fwrite(&block, sizeof block, 1, vol);
            }
            else if (i >= fragBegin && bitmap[i] == 'f')
            {
                //  GET THE BLOCK
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
                fread(&fBlock, sizeof fBlock, 1, vol);
                fBlock.firstblockID -= fragLen;
                //  WRITE THE BLOCK BACK
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * i, SEEK_SET);
                fwrite(&fBlock, sizeof fBlock, 1, vol);
            }
        }
    }

    //  FINISHED, CLOSE THE VOLUME
    fclose(vol);

    //  AND RETURN INDICATING SUCCESS
    return 0;
}
