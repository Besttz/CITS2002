#include <string.h>
#include <stdbool.h>

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
