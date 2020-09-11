#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  CITS2002 Project 1 2020
    Name:                Chaoyu Han
    Student number(s):   22384028
 */

/* TO DO LIST
1. Make a queue of process
2. Store the event(system-call) of every process


*/

//  MAXIMUM NUMBER OF PROCESSES OUR SYSTEM SUPPORTS (PID=1..20)
#define MAX_PROCESSES 20

//  MAXIMUM NUMBER OF SYSTEM-CALLS EVER MADE BY ANY PROCESS
#define MAX_SYSCALLS_PER_PROCESS 50

//  MAXIMUM NUMBER OF PIPES THAT ANY SINGLE PROCESS CAN HAVE OPEN (0..9)
#define MAX_PIPE_DESCRIPTORS_PER_PROCESS 10

//  TIME TAKEN TO SWITCH ANY PROCESS FROM ONE STATE TO ANOTHER
#define USECS_TO_CHANGE_PROCESS_STATE 5

//  TIME TAKEN TO TRANSFER ONE BYTE TO/FROM A PIPE
#define USECS_PER_BYTE_TRANSFERED 1

//  ---------------------------------------------------------------------
//  YOUR DATA STRUCTURES, VARIABLES, AND FUNCTIONS SHOULD BE ADDED HERE:

//  THIS IS DATA STRUCTURE FOR ARGUMENT
char FileName[MAX_PROCESSES];                          // Argument1
int TQ[MAX_PROCESSES];                                 //Argument2
int PipeSize[MAX_PROCESSES][MAX_SYSCALLS_PER_PROCESS]; //Argument3

//  ---------------------------------------------------------------------

// THIS IS DATA STRUCTURE FOR PROCESSES
int EventNum[MAX_PROCESSES];                                    // The total number of event of a process
int EventPID[MAX_PROCESSES][MAX_PIPE_DESCRIPTORS_PER_PROCESS];  // The PID of event
int EventType[MAX_PROCESSES][MAX_SYSCALLS_PER_PROCESS];         // The type of event
int EventTime[MAX_PROCESSES][MAX_PIPE_DESCRIPTORS_PER_PROCESS]; //The duation of event
int timetaken = 0;                                              // Total time

//  ---------------------------------------------------------------------

// Read arguments
int main(int argcount, char *argvalue[])
{
    int FileName = 0, TQ = 0, PipeSize = 0;
    if (argcount == 3)
        FileName = atoi(argvalue[1]);
    TQ = atoi(argcalue[2]);
    PipeSize = atoi(argvalue[3]);
}

//  ---------------------------------------------------------------------

//  FUNCTIONS TO VALIDATE FIELDS IN EACH eventfile - NO NEED TO MODIFY
int check_PID(char word[], int lc)
{
    int PID = atoi(word);

    if (PID <= 0 || PID > MAX_PROCESSES)
    {
        printf("invalid PID '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return PID;
}

int check_microseconds(char word[], int lc)
{
    int usecs = atoi(word);

    if (usecs <= 0)
    {
        printf("invalid microseconds '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return usecs;
}

int check_descriptor(char word[], int lc)
{
    int pd = atoi(word);

    if (pd < 0 || pd >= MAX_PIPE_DESCRIPTORS_PER_PROCESS)
    {
        printf("invalid pipe descriptor '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return pd;
}

int check_bytes(char word[], int lc)
{
    int nbytes = atoi(word);

    if (nbytes <= 0)
    {
        printf("invalid number of bytes '%s', line %i\n", word, lc);
        exit(EXIT_FAILURE);
    }
    return nbytes;
}

//  parse_eventfile() READS AND VALIDATES THE FILE'S CONTENTS
//  YOU NEED TO STORE ITS VALUES INTO YOUR OWN DATA-STRUCTURES AND VARIABLES
void parse_eventfile(char program[], char eventfile[])
{
#define LINELEN 100
#define WORDLEN 20
#define CHAR_COMMENT '#'

    //  ATTEMPT TO OPEN OUR EVENTFILE, REPORTING AN ERROR IF WE CAN'T
    FILE *fp = fopen(eventfile, "r");

    if (fp == NULL)
    {
        printf("%s: unable to open '%s'\n", program, eventfile);
        exit(EXIT_FAILURE);
    }

    char line[LINELEN], words[4][WORDLEN];
    int lc = 0;

    //  READ EACH LINE FROM THE EVENTFILE, UNTIL WE REACH THE END-OF-FILE
    while (fgets(line, sizeof line, fp) != NULL)
    {
        ++lc;

        //  COMMENT LINES ARE SIMPLY SKIPPED
        if (line[0] == CHAR_COMMENT)
        {
            continue;
        }

        //  ATTEMPT TO BREAK EACH LINE INTO A NUMBER OF WORDS, USING sscanf()
        int nwords = sscanf(line, "%19s %19s %19s %19s",
                            words[0], words[1], words[2], words[3]);
        //  WE WILL SIMPLY IGNORE ANY LINE WITHOUT ANY WORDS
        if (nwords <= 0)
        {
            continue;
        }

        //  ENSURE THAT THIS LINE'S PID IS VALID
        int thisPID = check_PID(words[0], lc);
        EventNum
            //  OTHER VALUES ON (SOME) LINES
            int otherPID,
            nbytes, usecs, pipedesc;

        //  IDENTIFY LINES RECORDING SYSTEM-CALLS AND THEIR OTHER VALUES
        //  THIS FUNCTION ONLY CHECKS INPUT;  YOU WILL NEED TO STORE THE VALUES
        if (nwords == 3 && strcmp(words[1], "compute") == 0)
        {
            usecs = check_microseconds(words[2], lc);
        }
        // if eventtype == [0] eventtime =
        else if (nwords == 3 && strcmp(words[1], "sleep") == 0)
        {
            usecs = check_microseconds(words[2], lc);
        }
        // if eventtype == [1] eventtime =
        else if (nwords == 2 && strcmp(words[1], "exit") == 0)
        {
            ;
        }
        // if eventtype == [3] eventtime [eventnum] = 5
        else if (nwords == 3 && strcmp(words[1], "fork") == 0)
        {
            otherPID = check_PID(words[2], lc)
        }
        // if eventtype == [4] eventtime [eventnum] =
        else if (nwords == 3 && strcmp(words[1], "wait") == 0)
        {
            otherPID = check_PID(words[2], lc);
        }
        // if eventtype == [5] eventtime =
        else if (nwords == 3 && strcmp(words[1], "pipe") == 0)
        {
            pipedesc = check_descriptor(words[2], lc);
        }
        else if (nwords == 4 && strcmp(words[1], "writepipe") == 0)
        {
            pipedesc = check_descriptor(words[2], lc);
            nbytes = check_bytes(words[3], lc);
        }
        else if (nwords == 4 && strcmp(words[1], "readpipe") == 0)
        {
            pipedesc = check_descriptor(words[2], lc);
            nbytes = check_bytes(words[3], lc);
        }
        //  UNRECOGNISED LINE
        else
        {
            printf("%s: line %i of '%s' is unrecognized\n", program, lc, eventfile);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);

#undef LINELEN
#undef WORDLEN
#undef CHAR_COMMENT
}

//  ---------------------------------------------------------------------
int queue[MAX_PROCESSES]; //
int window = 0;
int tail = 0;

int readQ()
{
    int result = queue[window];
    if (window = 49)
    {
        window = 0;
    }
    else
    {
        window++;
    }
    return result;
}
writeQ(int pid)
{
    queue[tail] = pid;
    if (tail = 49)
    {
        tail = 0;
    }
    else
    {
        tail++;
    }
}
//  ---------------------------------------------------------------------

//  CHECK THE COMMAND-LINE ARGUMENTS, CALL parse_eventfile(), RUN SIMULATION
int main(int argc, char *argv[])
{
    printf("timetaken %i\n", timetaken);
    return 0;
}
