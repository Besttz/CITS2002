#include <stdlib.h> // getenv
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

int executeUsingPATH(char *programName, char *arguments[]){
    char * path = getenv(PATH);
    return 0;
}

int main(int argc, char *argv[])
{
    
    
    return 0;
}
