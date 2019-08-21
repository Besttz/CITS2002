#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

 void replace( char oldword[], char newword[], char whole_sentence[] ) {
    int oo = strlen(oldword);
    int ss = strlen(whole_sentence);
    int nn = strlen(newword);
    int match[ss];
    int matchCount = 0;
    for (int i = 0; i < ss; i++)
    {
        match[i]=0;
    }
    
    for (int i = 0;i<(ss-oo+1);i++){
        int isMatch = 1;
        if(whole_sentence[i]==oldword[0]){
            for (int j =1;j < oo; j++)
            {
                if(whole_sentence[i+j]!=oldword[j]) {
                    isMatch = 0;
                    break;
                }
            }
            if (isMatch)
            {
                match[i]=1;
                matchCount++;
            }
        }
    }

    char result[ss+matchCount*(nn-oo)+1];
    int already = 0;
    for (int i = 0; i < ss; i++)
    {
        if (match[i]==0)
        {
            result[i+already]=whole_sentence[i];
        } else {
            int j = 0;
            for (; j < nn; j++)
            {
                result[i+j+already] = newword[j];
            }
            already+=(nn-oo);
            i+=oo-1;
        }
    }
    result[ss+matchCount*(nn-oo)] ='\0';
    printf("%s \n",result);
    
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
