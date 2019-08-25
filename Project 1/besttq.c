#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CITS2002 Project 1 2019
   Name(s):             Tommy Zhang, Zhenyu Yang
   Student number(s):   22181467 22374571
 */


//  besttq (v1.0)
//  Written by Chris.McDonald@uwa.edu.au, 2019, free for all to copy and modify

//  Compile with:  cc -std=c99 -Wall -Werror -o besttq besttq.c


//  THESE CONSTANTS DEFINE THE MAXIMUM SIZE OF TRACEFILE CONTENTS (AND HENCE
//  JOB-MIX) THAT YOUR PROGRAM NEEDS TO SUPPORT.  YOU'LL REQUIRE THESE
//  CONSTANTS WHEN DEFINING THE MAXIMUM SIZES OF ANY REQUIRED DATA STRUCTURES.

#define MAX_DEVICES             4
#define MAX_DEVICE_NAME         20
#define MAX_PROCESSES           50
// DO NOT USE THIS - #define MAX_PROCESS_EVENTS      1000
#define MAX_EVENTS_PER_PROCESS	100

#define TIME_CONTEXT_SWITCH     5
#define TIME_ACQUIRE_BUS        5


//  NOTE THAT DEVICE DATA-TRANSFER-RATES ARE MEASURED IN BYTES/SECOND,
//  THAT ALL TIMES ARE MEASURED IN MICROSECONDS (usecs),
//  AND THAT THE TOTAL-PROCESS-COMPLETION-TIME WILL NOT EXCEED 2000 SECONDS
//  (SO YOU CAN SAFELY USE 'STANDARD' 32-BIT ints TO STORE TIMES).

int optimal_time_quantum                = 0;
int total_process_completion_time       = 0;

//  ----------------------------------------------------------------------

#define CHAR_COMMENT            '#'
#define MAXWORD                 20

//  ----------------------------------------------------------------------
//  THIS IS DATA STRUCTURE FOR STORING I/O DEVICES
int devCount = 0;
int devSpeed[MAX_DEVICES]; 
char devNames[MAX_DEVICES][MAX_DEVICE_NAME];
//  Need to use strcpy(strs[0], devNames) to modify
//  One Device is represented by a single index
//  Using devSpeed[i] devNames[i] to get preporty
//  ----------------------------------------------------------------------
//  THIS IS DATA STRUCTURE FOR STORING PROCESSES
int pCount = 0;
int pStartTime[MAX_PROCESSES]; //The Start time of a process
int pEndTime[MAX_PROCESSES]; //The exit time of a process
int pEventNums[MAX_PROCESSES];//The total number of event of a process
int pEventTime[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS];//The start time of event
int pEventDevice[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS];//The asked device ID
int pEventDuration[MAX_PROCESSES][MAX_EVENTS_PER_PROCESS];//The time event need
int foundNewProcess = 0;
//  ----------------------------------------------------------------------

void parse_tracefile (char program[], char tracefile[])
{
//  ATTEMPT TO OPEN OUR TRACEFILE, REPORTING AN ERROR IF WE CAN'T
    FILE *fp    = fopen(tracefile, "r");

    if(fp == NULL) {
        printf("%s: unable to open '%s'\n", program, tracefile);
        exit(EXIT_FAILURE);
    }

    char line[BUFSIZ];
    int  lc     = 0;

//  READ EACH LINE FROM THE TRACEFILE, UNTIL WE REACH THE END-OF-FILE
    while(fgets(line, sizeof line, fp) != NULL) {
        ++lc;

//  COMMENT LINES ARE SIMPLY SKIPPED
        if(line[0] == CHAR_COMMENT) {
            continue;
        }

//  ATTEMPT TO BREAK EACH LINE INTO A NUMBER OF WORDS, USING sscanf()
        char    word0[MAXWORD], word1[MAXWORD], word2[MAXWORD], word3[MAXWORD];
        int nwords = sscanf(line, "%s %s %s %s", word0, word1, word2, word3);

//      printf("%i = %s", nwords, line);

//  WE WILL SIMPLY IGNORE ANY LINE WITHOUT ANY WORDS
        if(nwords <= 0) {
            continue;
        }
//  LOOK FOR LINES DEFINING DEVICES, PROCESSES, AND PROCESS EVENTS
        if(nwords == 4 && strcmp(word0, "device") == 0) {
            // FOUND A DEVICE, WILL CHECK THE SPEED TO CALCULATE PRIOROTY
            // AND PUT IT IN THE RIGHT POSITION IN THE DEVICE ARRAY
            int newSpeed = atoi(word2)*0.000001;
            int newID = 0;
            // Compare with current device list
            for (; newID < devCount; newID++) 
                if (devSpeed[newID]<newSpeed) break;
            // Put all the devices slower than this devide one position lower
            for (int i = devCount-1; i >= newID; i--)
            {
                devSpeed[i+1]=devSpeed[i];
                strcpy(devNames[i+1],devNames[i]);
            }
            // Put the new device in, and increse the count
            strcpy(devNames[newID],word1);    
            devSpeed[newID] = newSpeed;
            devCount++;
        }

        else if(nwords == 1 && strcmp(word0, "reboot") == 0) {
            printf("Totally %i Devices Added \n",devCount); // TEST PRINT
            for (int i = 0; i < devCount; i++) printf(
                "    %i: %s, Speed is %i b/μs\n",i,devNames[i], devSpeed[i]); 
                // TEST PRINT
            // NOTHING REALLY REQUIRED, DEVICE DEFINITIONS HAVE FINISHED
        }

        else if(nwords == 4 && strcmp(word0, "process") == 0) {
            pStartTime[pCount]=atoi(word2);   
            // FOUND THE START OF A PROCESS'S EVENTS, STORE BEGIN TIME
            foundNewProcess = 1;
            printf("Process %i Added, Start at %i\n",pCount,pStartTime[pCount]);
            // TEST PRINT
        }

        else if(nwords == 4 && strcmp(word0, "i/o") == 0) {
            if (foundNewProcess) { //Check if the pEventNums intilized
                pEventNums[pCount] = 0;
                foundNewProcess = 0;
            }
            int currentEvent = pEventNums[pCount]; // Get current event ID
            pEventTime[pCount][currentEvent] = atoi(word1); //Set start time
            char currentDevice[MAX_DEVICE_NAME]; // A String for device
            strcpy(currentDevice,word2);//Get current device
            int i = 0;
            for (; i < devCount; i++) //Find the device ID
            {
                if (strcmp(devNames[i],currentDevice)==0)
                {
                    pEventDevice[pCount][currentEvent]=i;
                    break;
                }
            }
            pEventDuration[pCount][currentEvent] = atoi(word3)/devSpeed[i];
               //  AN I/O EVENT FOR THE CURRENT PROCESS, STORE THIS SOMEWHERE
            printf("    Event %i Added, at %i time use %i %s for %iμs %ibyte.\n",
                    pEventNums[pCount],pEventTime[pCount][currentEvent],i,
                    currentDevice,pEventDuration[pCount][currentEvent],
                    atoi(word3)); // TEST PRINT
            pEventNums[pCount]++;
        }

        else if(nwords == 2 && strcmp(word0, "exit") == 0) {
            pEndTime[pCount]=atoi(word1);   
            //  PRESUMABLY THE LAST EVENT WE'LL SEE FOR THE CURRENT PROCESS
            printf("Process %i Exit at %i\n",pCount, pEndTime[pCount]);
            // TEST PRINT
        }

        else if(nwords == 1 && strcmp(word0, "}") == 0) {
            pCount++;   //  JUST THE END OF THE CURRENT PROCESS'S EVENTS
        }
        else {
            printf("%s: line %i of '%s' is unrecognized \n",
                        program, lc, tracefile);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

#undef  MAXWORD
#undef  CHAR_COMMENT

//  ----------------------------------------------------------------------

//  SIMULATE THE JOB-MIX FROM THE TRACEFILE, FOR THE GIVEN TIME-QUANTUM
void simulate_job_mix(int time_quantum)
{
    //PREPARING DATA STRUCTURE FOR QUEUES 
    //READY
    int readyQ[MAX_PROCESSES];//The queue for ready
    int nextR = 0; //The index of next ready process
    int readyQEnd= 0; //The current end point of ready queue
    //BLOCK
    int devQ[MAX_DEVICES][MAX_PROCESSES];//The queue for block for each device
    int devQDuration[MAX_DEVICES][MAX_PROCESSES];//The next event duration
    int nextD[MAX_DEVICES]; //The index of next block process
    int devQEnd[MAX_DEVICES];//The current end point of block queue

    int time = 0;//System time
    int processTime[MAX_PROCESSES];//Total exe time of process
    int finishedProcess = 0;//The count of finished process
    int processOnCPU = -1;//Current runnning process in CPU
    int CPUrunningTime = 0;//Current runnning time of this time quantum

    //START TO SIMULATE LOOP
    //THE LOOP WILL REPEAT ONCE THE TIME CHANGES
    //SO AT BEGINNING WE NEED TO DECIDE HOW MANYTIME ADDED THIS TIME
    while (finishedProcess != pCount)
    {
        //POSSBILITY OF TIME CHANGES(DIFFERENT RESPONSE)
        //1. NEW PROCESS ADDED 
        //2.TIME QUANTUM 3.RUNNING PROCESS EXIT 4.RUNNING PROCESS REQUEST I/O
        //5. BLOCKED -> READY(I/O FINISHED)
        //THEN WE CHECK THE NEAREST THING AND DO IT

        //CHECK TIME TO CASE 1
        int case1 = 10000000000; 
        int c1Process;
        for (int i = 0; i < pCount; i++)
        {
            if (pStartTime[i]>time&&pStartTime[i]-time<case1)
            {
                c1Process = i;
                case1 = pStartTime[i]-time;
            }
        }

        //CHECK TIME TO CASE 2 3 4 
        //CHECK THE NEXT PROCESS EXIT TIME OR EVENT 
        int case2or3or4 = 10000000000;
        int case234 = 0;
        //AND COMPARE WITH T.Q.
        if (nextR!=readyQEnd) //CHECK IF HAS PROCESS IN READY
        {
            //CHECK ALL THE EVENT IS DONE
            int eventTime = 0;
            for (int i = 0; i < pEventNums[nextR]; i++)
            {
                int tt =pEventTime[readyQ[nextR]][i]-processTime[readyQ[nextR]];
                if (tt>=0)
                {
                    eventTime = tt;
                    case234 = 4;
                    break; //IF THERE'S NEXT EVENT, SET CASE 4
                }   
            }
            if (case234==0) //IF case234 ISNT MODIFIED, THEN CHECK REMAIN TIME
            {
                eventTime= pEndTime[readyQ[nextR]]-processTime[readyQ[nextR]];
                case234 = 3;
            }
            
            if (eventTime>time_quantum) 
            {
                case234 = 2;//NEXT EVENT LONGER THAN TIME QUANTUM
                case2or3or4 = time_quantum;
            } else {
                case2or3or4 = eventTime;
            }
        }
        
        //CHECK TIME TO CASE 5
        int case5 = 10000000000;
        //CHECK EVERY DEVICE FROM PRIORITY 0-MAX
        for (int i = 0; i < devCount; i++)
        {
            if (nextD[i]==devQEnd[i])//THIS DEVICE HAS NO QUEUING PROCESS
            {
                continue;
            }
            case5=devQDuration[i][nextD[i]];
            break;
        }
        //CHECK WHO IS THE CASE FOR THIS TIME(THE SMALLEST)
        int caseNo = 0;
        if (case1<case2or3or4)
        {
            if (case1<case5) caseNo = 1;
            else caseNo = 5;
        } else {
            if (case2or3or4<case5) {
                caseNo = case234;
            } else caseNo = 5;
        }
        //GET REAL CASENO, NOW USE A SWITCH
        switch (caseNo)
        {
        case 1: //NEW PROCESS ADDED, PUT IT INTO READY QUEUE
            //CHECK IF CURRENT INDEX IS IN ARRAY END POINT
            if (readyQEnd!=MAX_PROCESSES-1) 
            {
                readyQ[readyQEnd++] = c1Process;
            } else { //PUT IT AT THE BEGINNING OF ARRAY
                readyQ[0] = c1Process;
                readyQEnd = 0;
            }
            
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
                    
        default:
            break;
        }
        //THE 5 microseconds FOR CONTENT SWITCH
        
        /* code */
    }
    
    //DECIDED HOW MUCH TIME PAST
    printf("running simulate_job_mix( time_quantum = %i usecs )\n",
                time_quantum);
}

//  ----------------------------------------------------------------------

void usage(char program[])
{
    printf("Usage: %s tracefile TQ-first [TQ-final TQ-increment]\n", program);
    exit(EXIT_FAILURE);
}

int main(int argcount, char *argvalue[])
{
    int TQ0 = 0, TQfinal = 0, TQinc = 0;

//  CALLED WITH THE PROVIDED TRACEFILE (NAME) AND THREE TIME VALUES
    if(argcount == 5) {
        TQ0     = atoi(argvalue[2]);
        TQfinal = atoi(argvalue[3]);
        TQinc   = atoi(argvalue[4]);

        if(TQ0 < 1 || TQfinal < TQ0 || TQinc < 1) {
            usage(argvalue[0]);
        }
    }
//  CALLED WITH THE PROVIDED TRACEFILE (NAME) AND ONE TIME VALUE
    else if(argcount == 3) {
        TQ0     = atoi(argvalue[2]);
        if(TQ0 < 1) {
            usage(argvalue[0]);
        }
        TQfinal = TQ0;
        TQinc   = 1;
    }
//  CALLED INCORRECTLY, REPORT THE ERROR AND TERMINATE
    else {
        usage(argvalue[0]);
    }

//  READ THE JOB-MIX FROM THE TRACEFILE, STORING INFORMATION IN DATA-STRUCTURES
    parse_tracefile(argvalue[0], argvalue[1]);

//  SIMULATE THE JOB-MIX FROM THE TRACEFILE, VARYING THE TIME-QUANTUM EACH TIME.
//  WE NEED TO FIND THE BEST (SHORTEST) TOTAL-PROCESS-COMPLETION-TIME
//  ACROSS EACH OF THE TIME-QUANTA BEING CONSIDERED

    for(int time_quantum=TQ0 ; time_quantum<=TQfinal ; time_quantum += TQinc) {
        simulate_job_mix(time_quantum);
    }

//  PRINT THE PROGRAM'S RESULT
    printf("best %i %i\n", optimal_time_quantum, total_process_completion_time);

    exit(EXIT_SUCCESS);
}

//  vim: ts=8 sw=4