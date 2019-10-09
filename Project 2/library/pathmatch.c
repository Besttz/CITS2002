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

    //下面的内容是多层目录的支持，先把其他的功能写完之后再写这个
    int parent = -1;
    int result = -1;
    // CHECK IF IT'S A SUBDIRECTORY
    const char *currentChar = pathname;
    while (*currentChar == '/')
        ++currentChar; //SKIP THE FIRST '/'
    if (*currentChar == '\0')
        return 0; // The path is root dir

    while (*currentChar != '\0') // Check any '/' left
    {
        if (*currentChar == '/')
        {
            result = -1;
            break;
        }
        ++currentChar;
    }
    if (*currentChar == '\0' && mode == 2) //This address is in root
        return 0;

    SIFS_VOLUME_HEADER volHeader;
    fread(&volHeader, sizeof volHeader, 1, vol);
    // Read current bitmap
    SIFS_BIT bitmap[volHeader.nblocks];
    fread(&bitmap, sizeof bitmap, 1, vol);

    currentChar = pathname;
    int currentCheckingBlock = 0;
    parent = 0;
    char thisOne[SIFS_MAX_NAME_LENGTH];
    int endOfSearch = 0;

    while (result == -1) //THIS DIR IS NOT WITHIN ROOTDIR
    {
        //CHECK IF THE LAST "THISONE" MATCHES AND RETURN (HAPPENS IN THE END OF SEARCH)
        // CHECK IF CURRENT CHECKING BLOCK IS A FILE (CHECK THE NAME THEN)
        if (bitmap[currentCheckingBlock] == 'f')
            {
                SIFS_FILEBLOCK checking_block;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
                fread(&checking_block, sizeof checking_block, 1, vol);
                int i = 0;
                for ( i = 0; i < SIFS_MAX_ENTRIES; i++)
                {
                    if (strcmp(checking_block.filenames[i],thisOne)==0)
                    {
                        if(mode == 0) return currentCheckingBlock;
                        if(mode == 1) return parent;
                    }  
                }
                if (i == SIFS_MAX_ENTRIES) return -1; // CANNOT FIND  
            }
            else if (endOfSearch == 1 &&bitmap[currentCheckingBlock] == 'd')
            {
                SIFS_DIRBLOCK checking_block;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
                fread(&checking_block, sizeof checking_block, 1, vol);
                 if (strcmp(checking_block.name,thisOne)==0)
                    {
                        if(mode == 0) return currentCheckingBlock;
                        if(mode == 1) return parent;
                    }
            }
            //DONE

        while (*currentChar == '/')
            ++currentChar;
        // int currentSearching = 0; // 0 for file, 1 for dir

        //  GET THE FIRST DIR NAME
        for (int i = 0; i < SIFS_MAX_NAME_LENGTH; i++)
        {
            thisOne[i] = *currentChar;
            currentChar++;
            
            if (*currentChar == '\0')
            { //Found a floder (Or a ending floder)
            endOfSearch = 1;
                thisOne[i + 1] = '\0';
                break;
            } else if (*currentChar == '/') // Found a floder
            {
                // currentSearching = 1;
                thisOne[i + 1] = '\0';
                break;
            }
        } // NOW WE HAVE THE NAME TO FIND
        
            
        //CHECK THE CURRENT CHECKING BLOCK (Parent) AND FIND ALL SUB
        SIFS_DIRBLOCK checking_dir_block;
        fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * currentCheckingBlock, SEEK_SET);
        fread(&checking_dir_block, sizeof checking_dir_block, 1, vol);
        // NOW CHECK ITS ENTRIES TO COMPARE THE NAME
        // char entryname[checking_dir_block.nentries][SIFS_MAX_NAME_LENGTH];
        int i = 0;
        for (i = 0; i < checking_dir_block.nentries; i++)
        {
            int checking = checking_dir_block.entries[i].blockID;
            if (bitmap[checking] == 'd')
            {
                SIFS_DIRBLOCK checking_block;
                fseek(vol, sizeof volHeader + sizeof bitmap + volHeader.blocksize * checking, SEEK_SET);
                fread(&checking_block, sizeof checking_block, 1, vol);
                if (strcmp(thisOne, checking_block.name) == 0) //Match!
                {
                    parent = currentCheckingBlock;
                    currentCheckingBlock = checking;
                    break;
                    //    if(mode == 0) return checking;
                    //    if(mode == 1) return parent;
                }
                else
                    continue;
            }
            else if (bitmap[checking] == 'f')
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
                    //    if(mode == 0) return checking;
                    //    if(mode == 1) return parent;
                }
                else
                    continue;
            }
        }
        if (i == checking_dir_block.nentries && mode == 2)
        {
           return currentCheckingBlock;
        }
        


        //  GET INTO THE FIRST DICTORY
    }
    //我们现在假设现在得到的 parentBlockID 就是需要建立新文件夹的目录

    return 0;
}
