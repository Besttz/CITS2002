#include <stdlib.h> // systemcall
#include <dirent.h> // DIRENT
//  stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h> // sprintf

#include <string.h> // String

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
    // printf("38\n");
    if (dirp == NULL)
        return -1;
    //  FOR ALL ITS ENTRIES
    char fullpath[MAXPATHLEN];
    // printf("OPEN DIR SUCCESSFULLY\n");
    while ((dp = readdir(dirp)) != NULL)
    {
        struct stat stat_buffer;
        struct stat *pointer = &stat_buffer;

        sprintf(fullpath, "%s/%s", directoryname, dp->d_name);
        // printf("fullpath: %s\n", fullpath);
        if (stat(fullpath, pointer) != 0)
        {
            EXIT_FAILURE;
        }
        else if (S_ISDIR(pointer->st_mode))
        {
            if (strcmp(dp->d_name, ".") == 0)
                continue;
            if (strcmp(dp->d_name, "..") == 0)
                continue;

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

/*
The parameter named dirName points to a character string, 
providing the name of a directory to be recursively explored 
for files and subdirectories. 

You may assume that dirName, and its subdirectories, 
contain only files and directories. 
After countEntries() has explored everything below the named directory, 
the parameters nFiles and nDirs should provide, to the calling function, 
the number of files and directories found, respectively. 

If countEntries() finds a directory that cannot be opened, 
it should report an error and continue its processing 
(i.e. the function should not exit on such an error). 
*/

void countEntries(char *dirName, int *nFiles, int *nDirs)
{
    DIR *dirp;
    struct dirent *dp;

    *nFiles = 0;
    *nDirs = 0;

    dirp = opendir(dirName);
    if (dirp == NULL)
    {
        //REPORT ERROR
    }
    else
    {
        struct stat stat_buffer;
        struct stat *pointer = &stat_buffer;
        char fullpath[MAXPATHLEN];

        while ((dp = readdir(dirp)) != NULL)
        {
            sprintf(fullpath, "%s/%s", dirName, dp->d_name);
            if (stat(fullpath, pointer) != 0)
                break;
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;
            if (S_ISDIR(pointer->st_mode))
            {

                int nD = 0;
                int nF = 0;
                countEntries(fullpath, &nF, &nD);
                *nFiles += nF;
                *nDirs += nD + 1;
            }
            else if (S_ISREG(pointer->st_mode))
            {
                *nFiles += 1;
            }
        }
        closedir(dirp);
    }
}

/*
Consider a hierarchical file-system consisting of directories, 
each of which possibly contains files and other sub-directories. 
There are no other types of file-system objects in this file- system.

When wishing to perform an incremental backup of all files in and below 
a named directory of the file-system, 
we require knowledge of how much space will be required to copy all files. 
However, for an incremental backup, we don’t wish to copy all files, 
but only the files that have been modified since a certain time. 

Write a C99 function to return the number of bytes contained 
in all files below a named directory 
that have been modified since an indicated time.

Any errors resulting from conditions such as being unable 
to access a sub-directory or a file should simply be ignored.
If directoryname can not be opened as a directory, 
your function should return -1.
*/

int totalBytes(char *directoryname, time_t since)
{
    DIR *dirp;
    struct dirent *dp;
    int result = 0;

    dirp = opendir(directoryname);
    if (dirp == NULL)
        return -1;
    else
    {
        while ((dp = readdir(dirp)) != NULL)
        {
            struct stat stat_buffer;
            struct stat *pointer = &stat_buffer;
            char fullpath[MAXPATHLEN];

            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
                continue;
            // sprintf(fullpath, "%s/%s", directoryname, dp->d_name);
            if (stat(fullpath, pointer) != 0)
                continue;
            printf("fullpath: %s\n", fullpath);

            if (S_ISDIR(pointer->st_mode))
            {
                int size = totalBytes(fullpath, since);
                if (size != -1)
                    result += size;
            }
            else if (S_ISREG(pointer->st_mode))
            {
                if (pointer->st_mtimespec.tv_sec > since)
                    result += pointer->st_size;
            }
        }
        closedir(dirp);
    }
    return result;
}

int main(int argc, char *argv[])
{

    // removeDirectory("/Users/Tommy/UWA/CITS2002/CITS2002/Exams/a");

    // int nD = 0;
    // int nF = 0;
    // countEntries("/Users/Tommy/UWA/CITS2002/CITS2002/Exams/a", &nF, &nD);
    // printf("nF:%i, nD: %i \n", nF, nD);
    printf("Size: %i \n", totalBytes("/Users/Tommy/UWA/CITS2002/CITS2002/Exams/a", 0));

    return 0;
}
