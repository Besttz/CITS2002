#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEFAULT_M 8
#define DEFAULT_Y 2019


// first_day_of_month.c START
//  written by Chris.McDonald@uwa.edu.au

//  PROVIDED WITHOUT MUCH EXPLANATION YET!

//  RETURNS  0=Sun, 1=Mon, .....

int first_day_of_month(int month, int year)
{
    struct tm   tm;

//  SET ALL FIELDS OF tm TO ZERO TO MAKE SOME FIELDS 'UNKNOWN'
    memset(&tm, 0, sizeof(tm));

//  INITIALIZE THE FILEDS THAT WE ALREADY KNOW
    tm.tm_mday  = 1;
    tm.tm_mon   = month-1;              // 0=Jan, 1=Feb, ....
    tm.tm_year  = year-1900;

//  ASK THE POSIX FUNCTION mktime TO DETERMINE THE 'UNKNOWN' FIELDS
//  See: http://pubs.opengroup.org/onlinepubs/9699919799/
    mktime(&tm);

//  RETURN THE INTEGER MONTH VALUE
    return tm.tm_wday;                  // 0=Sun, 1=Mon, .....
}
//first_day_of_month.c END


void printMonthly (int firstDay, int days) {
    printf("Su Mo Tu We Th Fr Sa\n");
    for (int i = 1; i <= 42; i++)
    {
        if (i<firstDay+1 || i>firstDay+days)
        {
            printf("   ");
        } else {
            printf("%2i ", (i-firstDay));
        }
        if (i%7==0)
        {
            printf("\n");
        } 
    }

}

void printCal(int month,int year){
    switch (month)
    {
    case 1:
        printf("     January %i    \n",year);
        printMonthly(first_day_of_month(month,year),31);
        break;
    
    case 8:
        printf("     August %i    \n",year);
        printMonthly(first_day_of_month(month,year),31);
        break;
    
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    //Determine if there're parameters
    if (argc == 1)
    {
        printCal(DEFAULT_M,DEFAULT_Y);
    } else if (argc == 2)
    {
        int para = atoi(argv[1]);
        if (para<=12)
        {
            printCal(para,DEFAULT_Y);
        } else
        {
            printf("Arguement Error\n");
            EXIT_FAILURE;
        }
    } else if (argc == 3) {
        printCal(atoi(argv[1]),atoi(argv[2]));
    }else
    {
        printf("Arguement Error\n");
        EXIT_FAILURE;
    }
    return 0;    
}
