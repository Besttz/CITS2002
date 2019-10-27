#include <string.h>
// #include <unistd.h>
#include <stdio.h>  // Printf()
#include <stdlib.h> // Me
// #include <stdlib.h>

/*
Consider a 2-dimensional spreadsheet − a rectangular grid of cells, 
each of which holds an arbitrary string of characters, 
or an empty string if the value of the cell has not yet been defined. 

Each cell is identified by 2 non-negative integer values, 
identifying a specific row and column. 

Define a C99 user-defined data type to represent an instance 
of the 2-dimensional spreadsheet. Next, define and implement three C99 functions:
one to return a pointer to allocated memory suitable for storing and managing the spreadsheet, 
one to swap the contents of two indicated columns in a spreadsheet, and 
one to completely deallocate all memory allocated to a spreadsheet.
*/

#define MAX_SIZE 1000

typedef struct
{
    char ***data;
    int row;
    int column;
} SPREADSHEET;

char ***make(int row, int column)
{
    char ***data = calloc(row, sizeof(char *));
    for (int i = 0; i < row; i++)
    {
        data[i] = calloc(column, sizeof(char *));
        for (int j = 0; j < column; j++)
        {
            data[i][j] = calloc(1, MAX_SIZE);
        }
    }
    return data;
}

int swap(SPREADSHEET sheet, int column1, int column2)
{
    char temp[MAX_SIZE];
    for (int i = 0; i < sheet.row; i++)
    {
        strcpy(temp, sheet.data[i][column1]);
        strcpy(sheet.data[i][column1], sheet.data[i][column2]);
        strcpy(sheet.data[i][column2], temp);
    }

    return 0;
}

int freeAll(SPREADSHEET sheet)
{
    for (int i = 0; i < sheet.row; i++)
    {
        for (int j = 0; j < sheet.column; j++)
        {
            if (sheet.data[i][j] != NULL)
                free(sheet.data[i][j]);
        }
        if (sheet.data[i] != NULL)
            free(sheet.data[i]);
    }

    free(sheet.data);
    return 0;
}

//  以上由于未知原因 Segmentation fault: 11 无法执行

/*
一;一维数组的申请与释放：

例如为一个字符数组分配空间：

char *p = (char *)malloc(m*sizeof(char));

释放空间：

free(p);

二:二维数组的申请与释放：

1：使用二级指针：

char **p = (char *)malloc(m*sizeof(char *));

for(i=0;i<m;i++)

{

      p[i]=(char *)malloc(n*sizeof(char *));

}

释放空间：

for(i-0;i<m;i++)

{

    free(p[i]);

}

free(p);

2:使用数组指针的方式：

char (*p)[3] = (char(*)[3])malloc(sizeof((char)*m*n);

这种方式分配的内存只需要释放一次：

free(p);

三：三维数组的申请与释放：

char ***p=(char* **)malloc(m*sizeof(char**));

for(i=0;i<m:i++)

p[i]=(char **)malloc(n*sizeof(char*));

for(i=0;i<m;i++)

   for(j=0;j<n;j++)

     p[i][j]=(char *)malloc(p*sizeof(char));

释放空间：

for(i=0;i<m;i++)

   for(j=0;j<n;j++)

    free(p[i][j]);

for(i=0;i<m:i++)

free(p[i]);

free(p);



注意：可以看出分配空间与释放空间都是逐步进行的，但是释放空间的顺序与分配空间的顺序相反。
————————————————
版权声明：本文为CSDN博主「xckkcxxck」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/xckkcxxck/article/details/60141365
*/

/*
Different applications for processing large text documents 
(such as a book’s chapter) may sometimes prefer a chapter’s 
text to be available as a single (long) string and, at other times, 
prefer the chapter to be available as a sequence of sentences. 
This question requires you to write two functions to perform the 
conversion between chapters and sentences. 

Write a function in C99, named makeSentences(), 
that converts a chapter into a vector of sentences. 
The function receives a single string (chapter) that 
points to the chapter’s text. Assume that all sentences 
end with a full-stop character (’.’), 
and that chapter is guaranteed to end with a complete sentence. 
The function produces 2 values. 
The function returns all sentences as a dynamically 
allocated array of dynamically allocated strings. 
The number of sentences is returned via the function’s 
second parameter (nSentences). 
If any call to dynamically allocate memory fails, the function should return NULL.
*/

#define MAX_SENTENCES 1000
char **makeSentences(char *chapter, int *nSentences)
{
    int nSen = 0;
    char *check = chapter;
    while (*check != '\0')
        if (*check++ == '.')
            nSen++;
    check = chapter;

    char **sentences = calloc(nSen, sizeof(char *));
    if (sentences == NULL)
        return NULL;

    for (int i = 0; i < nSen; i++)
    {
        sentences[i] = calloc(1, MAX_SIZE * sizeof(char));
        if (sentences[i] == NULL)
            return NULL;
        char *write = sentences[i];
        while (*check != '.')
            *write++ = *check++;
        check++;
        *write++ = '.';
        *write = '\0';
    }
    *nSentences = nSen;
    return sentences;
}

/*
Write a function in C99, named makeChapter(), 
that converts a vector of sentences into a single 
string that represents the chapter. 
The function receives a vector of sentences 
(as might be produced by makeSentences(), above) 
and converts the sentences into a single 
dynamically allocated string holding a whole chapter of sentences. 
When (re)constructing the chapter, 
ensure that you add a single space after each complete sentence. 
If any calls to dynamically allocate memory fail, 
makeChapter() should return NULL. 
You should assume that nSentences is always strictly greater than zero, 
and that sentences contains nSentences elements. 
Assume also that each string in sentences represents a complete sentence.
*/
char *makeChapter(char **sentences, int nSentences)
{
    char *chapter = calloc(nSentences, MAX_SENTENCES);
    char *write = chapter;
    for (int i = 0; i < nSentences; i++)
    {
        char *read = sentences[i];
        while (*read != '\0')
            *write++ = *read++;
        *write++ = ' ';
    }
    *write = '\0';
    return chapter;
}

char **cloneArgv(int argc, char *argv[])
{
    char **result = calloc(argc, sizeof(char *));
    for (int i = 0; i < argc; i++)
    {
        result[i] = calloc(100, sizeof(char));
        char *write = result[i];
        char *read = argv[i];
        while (*read != '\0')
            *write++ = *read++;
        *write = '\0';
    }
    return result;
}

void freArgv(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);
}

int main(int argc, char *argv[])
{
    if (strcmp("1", argv[1]) == 0)
    {
        SPREADSHEET aaa = {make(2, 2), 2, 2};
        char *new = "A";
        strcpy(aaa.data[0][0], new);
        char *neww = "B";
        strcpy(aaa.data[0][1], neww);
        char *newww = "C";
        char *newwww = "D";
        strcpy(aaa.data[1][0], newww);
        strcpy(aaa.data[1][1], newwww);
        for (int i = 0; i < aaa.row; i++)
        {
            for (int j = 0; j < aaa.column; j++)
            {
                printf("%s ", aaa.data[i][j]);
            }
            printf("\n ");
        }
        swap(aaa, 0, 1);
        printf("\n ");
        for (int i = 0; i < aaa.row; i++)
        {
            for (int j = 0; j < aaa.column; j++)
            {
                printf("%s ", aaa.data[i][j]);
            }
            printf("\n ");
        }
        freeAll(aaa);
    }
    else if (strcmp("2", argv[1]) == 0)
    {
        char chapter[] = "Hello. This is Tommy. Blah Blah Blah. Haha. 你这个笨笨. 笨笨杨振誉.";
        int nSen = 0;
        char **sen = makeSentences(chapter, &nSen);
        for (int i = 0; i < nSen; i++)
            printf("%i: %s \n", i, sen[i]);
        char *cha = makeChapter(sen, nSen);
        printf("Chapter: %s \n", cha);
        char **new = cloneArgv(nSen, sen);
        for (int i = 0; i < nSen; i++)
            printf("%i: %s \n", i, new[i]);
        freArgv(nSen, sen);
    }
    return 0;
}
