#include <stdlib.h>   // getenv
#include <stdio.h>    // printf
#include <unistd.h>   // exec
#include <sys/wait.h> // wait
/*
Consider environment variables used in Unix-based operating systems. 
A frequently seen environment variable is named PATH, 
and is used by command interpreters, or shells, 
to identify the names of directories to be 
searched to find executable programs. 
For example, a typical value of PATH may be: 
"/Users/chris/bin:/usr/local/bin:/usr/bin:/bin:." 
which provides a colon-separated list of directories 
to search for a required program. 

Write a C99 function, named executeUsingPATH(), 
which accepts the name of a program to execute 
and a NULL-pointer terminated vector of arguments to be passed to that program. 
The requested program may be specified using just its name, 
or with either an absolute or a relative pathname. 

Function executeUsingPATH() should attempt to 
execute programName from each directory provided via PATH, 
in order. If programName is found and may be executed 
(passing to it the indicated program arguments), 
the function should wait for its execution to terminate, 
and then return the exit status of the terminated process. 
If the function cannot find and execute programName 
then it should simply return the integer -1. 
Your function should not simply call the similar library 
function named execvp(). 

*/

// int executeUsingPATH(char *programName, char *arguments[])
// {
//     char *path = getenv("PATH");
//     char pathName[500];
//     char *check = path;
//     while (*check != '\0')
//     {
//         char *write = pathName;
//         while (*check != ':')
//             *write++ = *check++;
//         check++;
//         *write = '\0';
//         char fullpath[500];
//         sprintf(fullpath, "%s/%s", pathName, programName);
//         printf("%s\n", fullpath);
//         switch (fork())
//         {
//         case -1:
//             return -1;
//         case 0:
//             execv(fullpath, arguments);
//             return -1;
//             break;
//         default:
//         {
//             int status;
//             wait(&status);
//             if (status == -1)
//                 continue;
//             else return WEXITSTATUS(status);
//         }
//         }
//     }

//     return -1;
// }

/*
WIFEXITED(status) 这个宏用来指出子进程是否为正常退出的，如果是，它会返回一个非零值。

WEXITSTATUS(status) 当WIFEXITED返回非零值时，我们可以用这个宏来提取子进程的返回值，
如果子进程调用exit(5)退出，WEXITSTATUS(status)就会返回5；如果子进程调用exit(7)，
WEXITSTATUS(status)就会返回7。请注意，如果进程不是正常退出的，
也就是说，WIFEXITED返回0，这个值就毫无意义。
https://blog.csdn.net/mafuli007/article/details/8264200

*/
int executeUsingPATH(char *programName, char *arguments[])
{
    switch (fork())
    {
    case -1:
        return -1;
    case 0:
        break;
    default:
    {
        // int status;
        // wait(&status);
        // if (WEXITSTATUS(status) == 0)
        //     return -1;
        // else
        //     return WEXITSTATUS(status);
        siginfo_t info;
        waitid(P_ALL, 0, &info, WEXITED);
        return info.si_status;
    }
    }
    execv(programName, arguments);
    char *path = getenv("PATH");
    char pathName[500];
    char *check = path;
    while (*check != '\0')
    {
        char *write = pathName;
        while (*check != ':')
            *write++ = *check++;
        check++;
        *write = '\0';
        char fullpath[500];
        sprintf(fullpath, "%s/%s", pathName, programName);
        // printf("%s\n", fullpath);
        execv(fullpath, arguments);
    }

    EXIT_FAILURE;
    exit(-1);
    return -1;
}

int main(int argc, char *argv[])
{
    printf("%i\n", executeUsingPATH("cal", NULL));
    return 0;
}
