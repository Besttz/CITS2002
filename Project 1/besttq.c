#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//需要更正总体运行时间不包括第一个进程开始前的 bug
//需要根据示范程序调整 I/O 的时间计算

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

int optimal_time_quantum                = 1000000000;
int total_process_completion_time       = 1000000000;

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
    int readyQEnd= 0; //The current end point of ready queue(No process here)
    //BLOCK
    int devQ[MAX_DEVICES][MAX_PROCESSES];//The queue for block for each device
    int devQDuration[MAX_DEVICES][MAX_PROCESSES];//The next event duration
    int nextD[MAX_DEVICES]; //The index of next block process
    int devQEnd[MAX_DEVICES];//The current end point of block queue(No here)
    for (int i = 0; i < MAX_DEVICES; i++)
    {
        devQEnd[i] =0;
        nextD[i] =0;
        for (int j = 0; j < MAX_PROCESSES; j++)
        {
            devQ[i][j]=0;
            devQDuration[i][j]=0;
        }
    }
    int time = 0;//System time
    int processTime[MAX_PROCESSES];//Total exe time of process
    int finishedProcess = 0;//The count of finished process
    int finishedIO[MAX_PROCESSES];//Total I/O task done by this process
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        readyQ[i]=-1;
        processTime[i]=0;
        finishedIO[i]=0;
    }
    int processOnCPU = -1;//Current runnning process in CPU
    int processOnIO = -1;//Current runnning process in data bus
    int CPUrunningTime = 0;//Current runnning time of this time quantum
    int devRunningTime = 0;//Current I/O request runnning time

    //START TO SIMULATE LOOP
    //THE LOOP WILL REPEAT ONCE THE TIME CHANGES
    //SO AT BEGINNING WE NEED TO DECIDE HOW MANYTIME ADDED THIS TIME
    while (finishedProcess != pCount)
    {
        //POSSBILITY OF TIME CHANGES(DIFFERENT RESPONSE)
        //1. NEW PROCESS ADDED 
        //2.TIME QUANTUM 3. PROCESS EXIT 4. PROCESS REQUEST I/O
        //5. BLOCKED -> READY(I/O FINISHED) 
        //6. READY->RUNNING(NO PROCESS RUNNING)
        //THEN WE CHECK THE NEAREST THING AND DO IT
// CASE CHECKING !需要考虑上次切换上下文的状态!现在还没考虑完善 
        //CHECK TIME TO CASE 1
        int case1 = 1000000000; 
        int c1Process;
        for (int i = 0; i < pCount; i++)
        {
            if (pStartTime[i]==time) { 
                //check if at this time will be new process add
                //check if this process already in ready queue
                int j = 0;
                for (; j < MAX_PROCESSES; j++)
                {
                    if (readyQ[j]==i)  break;//This process already added
                }
                if (j!=MAX_PROCESSES) continue;//breaked before
                else {
                    c1Process = i;
                    case1 = 0;
                }
            } else if (pStartTime[i]>time&&pStartTime[i]-time<case1)
            {
                c1Process = i;
                case1 = pStartTime[i]-time;
            }
        }

        //CHECK IF PROCESS RUNNING, IF SO CASE 2 3 4 IS FOR THIS PROCESS
        //ELSE CASE 2 3 4 FOR THE PROCESS IN READY
        //CHECK TIME TO CASE 2 3 4 
        //CHECK THE NEXT PROCESS EXIT TIME OR EVENT 

        //NEED TO ADD SWITCH C
        int case2or3or4 = 1000000000;
        int case2346 = 0;
        int devID = 0; // For case 4
        int devDuration = 0;// For case 4
        if (processOnCPU!=-1)
        {
            int eventTime = 0;
            if (finishedIO[processOnCPU]!=pEventNums[processOnCPU])
            {
                eventTime=pEventTime[processOnCPU][finishedIO[processOnCPU]]-
                    processTime[processOnCPU];
                case2346 = 4;
                devID = pEventDevice[processOnCPU][finishedIO[processOnCPU]];
                devDuration = pEventDuration[processOnCPU][finishedIO[processOnCPU]];
            } else { 
                eventTime= pEndTime[processOnCPU]-processTime[processOnCPU];
                case2346 = 3;
            }
            case2or3or4 = eventTime;
            if (eventTime>time_quantum-CPUrunningTime&&eventTime!=0) 
            {
                case2346 = 2;//NEXT EVENT LONGER THAN TIME QUANTUM
                case2or3or4 = time_quantum-CPUrunningTime;
            }
        }

//CHECK TIME TO CASE 5  因为 CPU 执行其他任务时 I/O 也在读写 需要记录当前 I/O 任务 
        int case5 = 1000000000;
        int c5DevID=0;
        int c5Process = 0;
        //CHECK EVERY DEVICE FROM PRIORITY 0-MAX
        for (int i = 0; i < devCount; i++)
        {
            if (nextD[i]==devQEnd[i])//THIS DEVICE HAS NO QUEUING PROCESS
            {
                continue;
            }
            case5=devQDuration[i][nextD[i]]-devRunningTime;
            c5DevID = i;
            c5Process = devQ[i][nextD[i]];
            break;
        }
        int case6 = 1000000000;
        if (processOnCPU == -1&&nextR!=readyQEnd)
        {
            case6 = 5-CPUrunningTime;
        }
        

        //CHECK WHO IS THE CASE FOR THIS TIME(THE SMALLEST)
        //!!!!!!CASE6
        int caseNo = 0;
        if((case6<case5) && (case6<case2or3or4) && (case6<case1)) caseNo = 6;
        else if((case5<case6) && (case5<case2or3or4) && (case5<case1))caseNo=5;
        else if((case2or3or4<case6) && (case2or3or4<case5) && (case2or3or4<case1))
        caseNo=case2346;
        else caseNo=1;
        //GET REAL CASENO, NOW USE A SWITCH
//SIMULATE
        //EVERYTIME WHEN THE TIME IS PASSING, THE
        // CPUrunningTime  devRunningTime processTime time need to increse too
        switch (caseNo) //每次增加时长需要同时记录 CPU 运行 当前进程运行时长 当前 I/O 任务运行时长 系统总时间
        {
        case 1: //NEW PROCESS ADDED, PUT IT INTO READY QUEUE
            if (processOnIO!=-1) devRunningTime+=case1;
            time+=case1;
            if (processOnCPU!=-1) {
                processTime[processOnCPU]+=case1;
                CPUrunningTime+=case1;
            } else if(nextR!=readyQEnd) {
                CPUrunningTime+=case1;
            }
            //CHECK IF CURRENT INDEX IS IN ARRAY END POINT
            if (readyQEnd!=MAX_PROCESSES-1) 
            {
                readyQ[readyQEnd++] = c1Process;
            } else { //PUT IT AT THE BEGINNING OF ARRAY
                readyQ[MAX_PROCESSES-1] = c1Process;
                readyQEnd = 0;
            }
            //TIME ADDED
            break;
        case 2: // KEEP RUNNING UNTIL T.Q.
            
            time += case2or3or4; //ADD SYSTEM TIME CHANGE
            processTime[processOnCPU] +=case2or3or4; //ADD PROCESS TIME
            if (processOnIO!=-1) devRunningTime+=case2or3or4;
            //Checked the ready queue to decide if needs switch
            if (nextR!=readyQEnd)
            {
                //Put current process end of queue
                if (readyQEnd!=MAX_PROCESSES-1)  
                    readyQ[readyQEnd++] = processOnCPU;
                else { //OR PUT IT AT THE BEGINNING OF ARRAY
                    readyQ[MAX_PROCESSES-1] = processOnCPU;
                    readyQEnd = 0;
                }
                processOnCPU = -1;
            }
            CPUrunningTime = 0;
            break;
        case 3: //KEEP RUNNING UNTIL EXIT
            time += case2or3or4;//系统时间增加
            if (processOnIO!=-1) devRunningTime+=case2or3or4;//I/O 时间增加
            processTime[processOnCPU] = -1;//当前进程处理时间改为 -1
            finishedProcess++;//进程处理完成数 +=1
            CPUrunningTime = 0;//CPU 处理时间 0
            processOnCPU = -1;
            break;
        case 4: //KEEP RUNNING UNTIL I/O REQUIST
            time += case2or3or4;//系统时间增加
            if (processOnIO!=-1) devRunningTime+=case2or3or4;//I/O 时间增加
            processTime[processOnCPU] +=case2or3or4;//当前进程处理时间增加
            CPUrunningTime = 0;//CPU 处理时间 0
            //对应设备的队列加入本进程 根据 devQEnd判断 位置
            devQ[devID][devQEnd[devID]] = processOnCPU;
            //查看当进程是否已经请求过数据总线（看当前事件是否是该进程的第一个） 若无时长加5
            if (finishedIO[processOnCPU]==0)
                devQDuration[devID][devQEnd[devID]]=devDuration+5;
            else devQDuration[devID][devQEnd[devID]]=devDuration;
            //对应设备的队列时间信息加入本进程 位置同上
            //devQEnd ++
            if (devQEnd[devID]!=MAX_PROCESSES-1)  devQEnd[devID]++;
                else  devQEnd[devID] = 0; 
            processOnCPU =-1;
            /* code */
            break;
        case 5: // FINISH AN I/O REQUIST, BLOCK -> READY
            time += case5;//系统时间增加
            if (processOnCPU!=-1) {
                processTime[processOnCPU]+=case5;
                CPUrunningTime+=case5;
            } else if(nextR!=readyQEnd) {
                CPUrunningTime+=case5;
            }
            finishedIO[c5Process] ++;
            // processTime[processOnCPU] +=case5;//当前进程处理时间增加
            // CPUrunningTime +=case5;//CPU 处理时间增加
            //当前处理进程加入 Ready 队列 （模仿 case1）
            //CHECK IF CURRENT INDEX IS IN ARRAY END POINT
            if (readyQEnd!=MAX_PROCESSES-1) 
            {
                readyQ[readyQEnd++] = c5Process;
            } else { //PUT IT AT THE BEGINNING OF ARRAY
                readyQ[MAX_PROCESSES-1] = c5Process;
                readyQEnd = 0;
            }
            //设备 Ready 队列换成下一个
            if (nextD[devID]!=MAX_PROCESSES-1)  nextD[c5DevID]++;
                else  nextD[c5DevID] = 0; 
            //查看设备 Ready 队列（下一个任务）是否需要换进程（上下文切换） 是则处理时'-5'
            devRunningTime = 0; //否则 I/O 时间归零
            //首先核实当 数据总线换进程使用时是否还需计算五秒钟的问题 
            // for (int i = devID; i < devCount; i++)
            // {
            //     if (nextD[i]==devQEnd[i])//THIS DEVICE HAS NO QUEUING PROCESS
            //     {
            //         continue;
            //     }
            //     if (c5Process != devQ[i][nextD[i]]) devRunningTime = -5;
            //     break;
            // }
            break;
        case 6: // MOVE READY TO RUNNING 切换上下文时间过后
            time += case6;//系统时间增加
            if (processOnIO!=-1) devRunningTime+=case6;//I/O 时间增加
            CPUrunningTime = 0;//CPU 处理时间 0
            processOnCPU = readyQ[nextR];//当前处理进程改为 Ready 首位
            //Switch ready to the next process
            if (nextR!=MAX_PROCESSES-1)  nextR++;
            else  nextR = 0;
            //Ready 队列换成下一个
            break;
                    
        default:
            break;
        }

    }
    
    //DECIDED HOW MUCH TIME PAST
    printf("running simulate_job_mix( time_quantum = %i usecs ), total %iμs\n",
                time_quantum, time);
    // CHECK IF IT'S BEST
    if (time < total_process_completion_time )
    {
        total_process_completion_time = time;
        optimal_time_quantum = time_quantum;
    }
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