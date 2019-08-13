#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
        {
                fprintf(stderr, "%s: program expected 2 arguments, but instead received %d\n",
					argv[0], argc-1);
                exit(EXIT_FAILURE);
        }
        else
        {
            for (int i = 1 ; i <argc; i++)
            {
                long num = atol(argv[i]);
                if (num<1000)
                {
                    printf("%ldByte\n",num);
                }  else if(num<1000000) {
                    printf("%ldKByte\n",(num/1000));
                } else if (num<1000000000) {
                    printf("%ldMByte\n",(num/1000000));
                } else {
                    printf("%ldGByte\n",(num/1000000000));
                }               
            }
            exit(EXIT_SUCCESS);
        }
}
