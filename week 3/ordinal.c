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
            for (int i = 0; i <argc; i++)
            {
                int num = atoi(argv[i]);
                switch (num%10)
                {
                case 1:
                    if (num%100==11)
                    {
                        printf("%ith\n",num);
                    } else {
                        printf("%ist\n",num);
                    }
                    
                    break;
                
                case 2:
                    if (num%100==12)
                    {
                        printf("%ith\n",num);
                    } else {
                        printf("%ind\n",num);
                    }
                    
                    break;

                case 3:
                    if (num%100==13)
                    {
                        printf("%ith\n",num);
                    } else {
                        printf("%ird\n",num);
                    }
                    
                    break;
                default:
                    printf("%ith\n",num);
                    break;
                }
            }
        }
        return 0;
}
