#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*  CITS2002 Project 1 2020
    Name:                Chaoyu Han
    Student number(s):   22384028
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
int TQ; // Timequantum

//  ---------------------------------------------------------------------

// THIS IS DATA STRUCTURE FOR PROCESSES
int eventType[MAX_PROCESSES][MAX_SYSCALLS_PER_PROCESS];         // The type of event
int eventTime[MAX_PROCESSES][MAX_PIPE_DESCRIPTORS_PER_PROCESS]; //The duation of event
int eventWindow[MAX_PROCESSES];                                 // The window of the event
int eventTail[MAX_PROCESSES];                                   // The tail of the event
int exitEvent[MAX_PROCESSES];                                   // The process ID waiting for this child process
int running = 1;                                                // To see if the process is runing
int first = 1;                                                  // If the first execution
int numProcess = 1;                                             // The number of total processes
int timetaken = 0;                                              // Total time

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
        //  OTHER VALUES ON (SOME) LINES
        int otherPID, usecs;
        // int nbytes, usecs, pipedesc;

        //  IDENTIFY LINES RECORDING SYSTEM-CALLS AND THEIR OTHER VALUES
        //  THIS FUNCTION ONLY CHECKS INPUT;  YOU WILL NEED TO STORE THE VALUES
        if (nwords == 3 && strcmp(words[1], "compute") == 0)
        {
            // Record the time to compute
            usecs = check_microseconds(words[2], lc);
            eventType[thisPID][eventTail[thisPID]] = 0;
            eventTime[thisPID][eventTail[thisPID]] = atoi(words[2]);
            eventTail[thisPID]++;
        }

        else if (nwords == 3 && strcmp(words[1], "sleep") == 0)
        {
            // Record the time to sleep
            usecs = check_microseconds(words[2], lc);
            eventType[thisPID][eventTail[thisPID]] = 1;
            eventTime[thisPID][eventTail[thisPID]] = atoi(words[2]);
            eventTail[thisPID]++;
        }

        else if (nwords == 2 && strcmp(words[1], "exit") == 0)
        {
            eventType[thisPID][eventTail[thisPID]] = 2;
            eventTail[thisPID]++;
        }

        else if (nwords == 3 && strcmp(words[1], "fork") == 0)
        {
            // Record the child process ID
            otherPID = check_PID(words[2], lc);
            eventType[thisPID][eventTail[thisPID]] = 3;
            eventTime[thisPID][eventTail[thisPID]] = otherPID;
            eventTail[thisPID]++;
        }

        else if (nwords == 3 && strcmp(words[1], "wait") == 0)
        {
            // Record the child process ID to wait
            otherPID = check_PID(words[2], lc);
            eventType[thisPID][eventTail[thisPID]] = 4;
            exitEvent[otherPID] = thisPID;
            eventTail[thisPID]++;
        }

        else if (nwords == 3 && strcmp(words[1], "pipe") == 0)
        {
        }
        else if (nwords == 4 && strcmp(words[1], "writepipe") == 0)
        {
        }
        else if (nwords == 4 && strcmp(words[1], "readpipe") == 0)
        {
        }
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
int queue[MAX_PROCESSES]; // The Queue of ready process
int window = 0;           // The head of the queue
int tail = 0;             // The end of the queue

int readQ()
{
    // Check if the queue is empty
    if (window >= tail || (tail == 49 && window == 0))
    {
        return -1;
    }

    int result = queue[window];

    // Move the head to the next position
    if (window == 49)
    {
        window = 0;
    }
    else
    {
        window++;
    }
    return result;
}
void writeQ(int pid)
{
    queue[tail] = pid;
    // Move the tail to the next position
    if (tail == 49)
    {
        tail = 0;
    }
    else
    {
        tail++;
    }
}
//  ---------------------------------------------------------------------
int sleepQueue[MAX_PROCESSES]; // The sleep staus of each process
int sleepTime[MAX_PROCESSES];  // The time of awake of each process
void checkSleeping()
{
    // Check if any process wakes in current time
    for (int i = 1; i < MAX_PROCESSES; i++)
    {
        if (sleepQueue[i] && sleepTime[i] <= timetaken)
        {
            sleepQueue[i] = 0;
            writeQ(i);
        }
    }
}
//  ---------------------------------------------------------------------
// The main simulation of process executing
void execute()
{
    int currentProcess; // The PID of current running process

    // Check if this is the first time to execute
    if (first)
    {
        // Set running process to 1
        currentProcess = 1;
        first = 0;
    }
    else
    {
        // Get a process from the queue
        currentProcess = readQ();
        // Check if the queue empty
        if (currentProcess == -1)
        {
            // Check if anyone sleeping
            int recentAwake = 999999;
            // Find out the recent awake time
            for (int i = 1; i < MAX_PROCESSES; i++)
            {
                if (sleepQueue[i] && sleepTime[i] < recentAwake)
                {
                    recentAwake = sleepTime[i];
                }
            }
            // If no process in the queue, skip time to the nearest awake time
            if (recentAwake < 999999)
            {
                timetaken = recentAwake;
                checkSleeping();
            }
            else
            {
                // No process sleeping and waiting, finalise the execution
                running = 0;
            }
            return;
        }
        // Change the state of Ready to Running
        timetaken += 5;
    }

    int win = eventWindow[currentProcess]; // The head of event queue of current running process

    int childPid; // Child PID for fork() and wait()
    switch (eventType[currentProcess][win])
    {
        // Check the event(System call) type of the event
    case 0: // Compute
        if (eventTime[currentProcess][win] <= TQ)
        {
            timetaken += eventTime[currentProcess][win];
            eventWindow[currentProcess]++;
        }
        else if (eventTime[currentProcess][win] > TQ)
        {
            timetaken += TQ;
            eventTime[currentProcess][win] -= TQ;
        }
        writeQ(currentProcess);
        timetaken += 5;
        break;
    case 1: // Sleep
        timetaken += 10;
        // Put current process into the sleep queue
        sleepQueue[currentProcess] = 1;
        sleepTime[currentProcess] = timetaken + eventTime[currentProcess][win];
        eventWindow[currentProcess]++;
        break;
    case 2: // EXIT
        timetaken += 5;
        // Check if any process waiting this child process exit
        if (exitEvent[currentProcess] != 0)
        {
            writeQ(exitEvent[currentProcess]);
        }

        break;
    case 3: // Fork
        childPid = ++numProcess;
        // Add the child process into the queue
        writeQ(childPid);
        writeQ(currentProcess);
        timetaken += 10;
        eventWindow[currentProcess]++;

        break;
    case 4: // Wait
        timetaken += 10;
        // Doing nothing, and will be actived when the waiting child process exit
        eventWindow[currentProcess]++;
        break;
    default:
        break;
    }
    checkSleeping(); // Check any process awake upon the complection of current event
}

//  ---------------------------------------------------------------------

//  CHECK THE COMMAND-LINE ARGUMENTS, CALL parse_eventfile(), RUN SIMULATION

int main(int argcount, char *argvalue[])
{
    char *fileName = "";
    TQ = 0;
    fileName = argvalue[1];
    TQ = atoi(argvalue[2]);

    // Initialize the array variables
    for (size_t i = 0; i < MAX_PROCESSES; i++)
    {
        exitEvent[i] = 0;
    }

    // Parse File
    parse_eventfile(argvalue[0], fileName);
    timetaken = 0;

    // Start Simulation
    while (running)
    {
        execute();
    }

    // Return result
    printf("timetaken %i\n", timetaken);
    return 0;
}
