#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 void replace( char oldword[], char newword[], char whole_sentence[] ) {
     int oo = strlen(oldword);
     int ss = strlen(whole_sentence);
     for 
 }

 int main(int argc, char *argv[])
{   if(argc != 4)
        {
                fprintf(stderr, "%s: program expected 3 argument, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            replace(argv[1], argv[2],argv[3]);
            return 0;
        }
}