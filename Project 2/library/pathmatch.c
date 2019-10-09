#include "sifs-new.h"

/* CITS2002 Project 2 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */
// get the block ID by given path
// If the mode is 0, this function will find the block ID of this file or dir
// If the mode is 1, this function will find the block ID of its parents
// If the mode is 2, this function will find the block ID of its parents(Even the file is not)
int SIFS_pathmatch(const char *volumename, const char *pathname, int mode)
{
    FILE *vol = fopen(volumename, "r");

    // CHECK IF IT'S A SUBDIRECTORY
    const char *currentChar = pathname;
    while (*currentChar == '/')
        ++currentChar; //SKIP THE FIRST '/'
    if (*currentChar == '\0')
        return 0; // The path is root dir

    while (*currentChar != '\0') // Check any '/' left
    {
        if (*currentChar == '/')
            break;
        ++currentChar;
    }
    if (*currentChar == '\0' && mode == 2) //This address is in root
        return 0;

    //  START TO READ FILE
    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    // Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    //  START TO SEARCH
    currentChar = pathname;
    int currentCheckingBlock = 0;
    int parent = -1;
    char thisOne[SIFS_MAX_NAME_LENGTH]; // CURRENT SEARCHING NAME
    int endOfSearch = 0;                //THIS IS THE LAST TO SEARCH

    while (1) //LOOP TO SEARCH
    {
        //  CHECK IF THE LAST "THISONE" MATCHES AND RETURN (HAPPENS IN THE END OF SEARCH)
        //  CHECK IF CURRENT CHECKING BLOCK IS A FILE (CHECK THE NAME THEN)
        if (bitmap[currentCheckingBlock] == 'f') // CHECKING A FILE (MEANS END OF SEARCH)
        {
            //  GET THE INFO OF FILE
            SIFS_FILEBLOCK checking_block;
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
            fread(&checking_block, sizeof checking_block, 1, vol);
            int i = 0;
            //  COMPARE FILE NAME
            for (i = 0; i < SIFS_MAX_ENTRIES; i++)
            {
                if (strcmp(checking_block.filenames[i], thisOne) == 0)
                {
                    if (mode == 0)
                        return currentCheckingBlock;
                    if (mode == 1)
                        return parent;
                }
            }
            if (i == SIFS_MAX_ENTRIES)
                return -1; // NAME DOSNOT MATCH, CANNOT FIND
        }
        else if (endOfSearch == 1 && bitmap[currentCheckingBlock] == 'd')
        {
            //  GET THE INFO OF DIR
            SIFS_DIRBLOCK checking_block;
            fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
            fread(&checking_block, sizeof checking_block, 1, vol);
            //  COMPARE  NAME
            if (strcmp(checking_block.name, thisOne) == 0)
            {
                if (mode == 0)
                    return currentCheckingBlock;
                if (mode == 1)
                    return parent;
            }
        }
        //END OF "END OF SEARCH" COMPARE

        while (*currentChar == '/')
            ++currentChar; // DELETED THE '/'

        //  GET THE NAME TO SEARCH IN THIS ROUND
        for (int i = 0; i < SIFS_MAX_NAME_LENGTH; i++)
        {
            thisOne[i] = *currentChar;
            currentChar++;

            if (*currentChar == '\0')
            { //Found a file (Or a ending floder)
                endOfSearch = 1;
                thisOne[i + 1] = '\0';
                break;
            }
            else if (*currentChar == '/') // Found a floder
            {
                while (*(currentChar + 1) == '/')
                    currentChar++;
                if (*(currentChar + 1) == '\0')
                { //Found a file (Or a ending floder)
                    endOfSearch = 1;
                    thisOne[i + 1] = '\0';
                    break;
                }

                // currentSearching = 1;
                thisOne[i + 1] = '\0';
                break;
            }
        } // NOW WE HAVE THE NAME TO FIND

        if (strcmp(thisOne, "") == 0) //Current Finding Nothing
        {
            //  Which means it's in the right floder rightnow
            if (mode == 0)
                return currentCheckingBlock;
            if (mode == 1)
                return parent;
        }

        //CHECK THE CURRENT CHECKING BLOCK (Parent) AND FIND ALL SUB
        SIFS_DIRBLOCK checking_dir_block;
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
        fread(&checking_dir_block, sizeof checking_dir_block, 1, vol);
        // NOW CHECK ITS ENTRIES TO COMPARE THE NAME

        int i = 0;
        for (i = 0; i < checking_dir_block.nentries; i++)
        {
            int checking = checking_dir_block.entries[i].blockID; //BLOCK ID GOING TO CHECK
            if (bitmap[checking] == 'd')                          //THIS IS A DIR
            {
                //GET INFO OF THIS DIR
                SIFS_DIRBLOCK checking_block;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * checking, SEEK_SET);
                fread(&checking_block, sizeof checking_block, 1, vol);
                if (strcmp(thisOne, checking_block.name) == 0) //Match!
                {
                    parent = currentCheckingBlock;
                    currentCheckingBlock = checking;
                    break;
                }
                else
                    continue;
            }
            else if (bitmap[checking] == 'f') // THIS IS A FILE
            {
                int fileNameIndex = checking_dir_block.entries[i].fileindex;
                SIFS_FILEBLOCK checking_block;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * checking, SEEK_SET);
                fread(&checking_block, sizeof checking_block, 1, vol);
                if (strcmp(thisOne, checking_block.filenames[fileNameIndex]) == 0) //Match!
                {
                    parent = currentCheckingBlock;
                    currentCheckingBlock = checking;
                    break;
                }
                else
                    continue;
            }
        }
        // CAN NOT FIND OUR GOAL IN THIS FOLDER
        if (i == checking_dir_block.nentries)
        {
            if (endOfSearch == 1 && mode == 2)
            {
                // printf("测试用 现在检索完毕 父文件夹 %s 并且没有在找的 %s. \n", checking_dir_block.name, thisOne);
                if (strcmp(thisOne, "") == 0)  return parent;
                return currentCheckingBlock;
            }
            else if (mode == 0)
                return -1;
        }
    }

    return -1;
}
