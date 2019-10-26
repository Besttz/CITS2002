#include <string.h>
#include <stdbool.h>
#include <stdio.h>  //printf
#include <stdlib.h> //atoi()
/* 
The purpose of the function is to determine 
if the character string pointed to by needle 
is a substring of the character string pointed to by haystack. 
The function returns true if needle is a substring of haystack 
(i.e. needle completely appears within haystack), and false otherwise. 
You may assume that both haystack and needle are valid pointers to strings.
*/

bool subString(char *haystack, char *needle)
{
    char *string = haystack;
    char *pattern;

    while (*string != '\0')
    {
        pattern = needle; // PUT THIS POINT TO THE BEGINING OF NEEDLE

        if (*string == *pattern) // CHECK IF THE FIRST CHAR MATCH
        {
            char *stringNEW = string;

            while (1)
            {
                pattern++;
                stringNEW++;
                // CHECK PATTERN IS END (ALL CHAR MATCHED)
                if (*pattern == '\0')

                    return true; // BREAK IF MISMATCH
                if (*pattern != *stringNEW)
                    break;
                //  IF STILL MATCH AND THE PATTERN IS NOT OVER, CONTINUE
            }
        }
        string++; // CHECK FROM THE NEXT CHAR UNTIL THE END OF HAYSTACK
    }
    // IF NOT FOUND MATCH UNTIL THE END OF HAYSTACK, RETURN FALSE
    return false;
}

/*
The function takes four integer parameters,
each providing information about a dateand its year.

The goal of the function is to firstly ensure that
the provided date is valid and, if so, to determine
on which day of the week the date falls. 

The first parameter, day, provides the day of interest − its
values must range from 1 to 31 (inclusive). 

The second parameter, month, provides the month of interest
− its values must range from 1 to 12 (inclusive). 

The third parameter, year, provides the year of interest
− any integer value of 1970 or greater is permitted. 

The final parameter, firstJan, indicates the day
of the week on which the first of January falls
in the provided year. Its permitted values are 0 
(representing Sunday), 1 (representing Monday), 
and so on, up to 6 (representing Saturday).

If the provided date is invalid, 
the function should return the integer -1.
Write the function dayOfWeek() in C99.
*/
int dayOfWeek(int day, int month, int year, int firstJan)
{
    int leap = 0; // IF THIS IS LEAP YEAR
    int dayOfYear = 0;
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1970)
        return -1;
    //  CHECK IF THIS YEAR IS LEAP YEAR
    if (year % 4 == 0 && year % 100 != 0)
        leap = 1;
    else if (year % 400 == 0)
        leap = 1;

    if (!leap && month == 2 && day > 28)
        return -1;
    for (int i = 1; i <= 12; i++)
    {
        if (i < month)
        {
            if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10)
                dayOfYear += 31;
            else if (i == 4 || i == 6 || i == 9 || i == 11)
                dayOfYear += 30;
            else if (leap)
                dayOfYear += 29;
            else
                dayOfYear += 28;
        }
        else
        {
            dayOfYear += day;
            return (dayOfYear - 1 + firstJan) % 7;
        }
    }
    return -1;
}

/*
The function receives two arrays of integers, named set1 and set2,
whose lengths are provided by parameters len1 and len2, respectively.
All elements of set1 are guaranteed to be unique,
but are not necessarily sorted.
The same is also true of the elements of set2. 
There is no requirement that len1 and len2 have the same value.
The goal of the function is to determine if set2 is a non-empty subset of set1.
*/

bool isSubset(int set1[], int len1, int set2[], int len2)
{

    if (len2 <= 0)
        return false;
    if (len2 > len1)
        return false;
        
    for (int i = 0; i < len2; i++)
    {
        int now = set2[i];
        int j = 0;
        for (; j < len1; j++)
            if (set1[j == now])
                break;
        if (j == len1)
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    printf("%i\n", dayOfWeek(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4])));
}
