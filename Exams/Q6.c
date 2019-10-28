#include <stdlib.h> // systemcall
#include <dirent.h> // DIRENT
//  stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h> // sprintf

/*
Consider the following operating system dependent system-calls: 
int unlink(char *filename);
int rmdir(char *directoryname); 
The unlink() system-call may be used to remove a file from its parent directory, 
and the rmdir() system-call may be used to remove a directory from its parent 
directory provided that directoryname itself contains no other files or directories. 
Assume that both system-calls return 0 on success, and 1 on failure. 

Using the unlink() and rmdir() system-calls, 
write a C99 function named removeDirectory() 
that removes the indicated (potentially non-empty) directory 
and all of its contents. 
Your function should have the following prototype: 
You should assume directoryname contains only files and directories. 
Your function should attempt to remove as many files and sub-directories as possible, 
returning 0 on complete success and non-zero otherwise. 
If directoryname could not be opened as a directory, your function should return -1.
*/

#define MAXPATHLEN 100
int removeDirectory(char *directoryname)
{
    DIR *dirp;
    struct dirent *dp;
    //  TRY TO OPEN THE FIR
    dirp = opendir(directoryname);
    //  CHECK IF OPEN SUCCESSFULLY
    if (dirp == NULL)
        return -1;
    //  FOR ALL ITS ENTRIES
    char fullpath[MAXPATHLEN];
    while ((dp = readdir(dirp)) != NULL)
    {
        struct stat stat_buffer;
        struct stat *pointer = &stat_buffer;

        sprintf(fullpath, "%s/%s", directoryname, dp->d_name);
        if (stat(fullpath, pointer) != 0)
        {
            EXIT_FAILURE;
        }
        else if (S_ISDIR(pointer->st_mode))
        {
            if (removeDirectory(fullpath) != 0)
                return 1;
        }
        else if (S_ISREG(pointer->st_mode))
        {
            if (unlink(fullpath) != 0)
                return 1;
        }
    }
    closedir(dirp);
    //  IF IS A FILE THEN DELETED
    //  RECURSIVELY RUN removeDirectory IF IT'S A DIR
    //  REMOVE THIS DIR AFTER CLEAR ALL ENTRIES
    rmdir(directoryname);
    return 0;
}

int main(int argc, char *argv[])
{

    removeDirectory("/Users/Tommy/UWA/CITS2002/CITS2002/Exams/a");
    return 0;
}
