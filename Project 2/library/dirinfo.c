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
    if (volumename == NULL || pathname == NULL||entrynames== NULL 
        ||nentries== NULL ||modtime== NULL){
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

//下面的内容是多层目录的支持，先把其他的功能写完之后再写这个
// 已经把将要写的内容转移到 pathmatch 到时候直接调用就可
//我们现在假设现在得到的 blockID 就是文件夹的目录
    int blockID = SIFS_pathmatch(vol, pathname);
    SIFS_DIRBLOCK block;
    fseek(vol,sizeof volHeader+sizeof bitmap+volHeader.blocksize*blockID,SEEK_SET);
    fread(&block, sizeof block, 1, vol);
    // strcpy(***entrynames,block.name);
    *nentries=block.nentries;
    *modtime=block.modtime;
    for (int i = 0; i < block.nentries; i++)
    {
        strcpy(*entrynames[i],"a");
    }
    
    return 0;
}
