#define PATH_MAX _PC_PATH_MAX
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "temp.h"

const char myfile[] = "clipboard_path_added_by_bash_script";

FILE *fptr,*sptr,*dptr;

int i, j, k, n = 1, l = 0, *count;
char ***str;
static bool function_mode = false; // False in function_mode indicates that the execution is for adding into clipboard
static bool move = false;     // False in copy_move is to sent copy and true for moving


int line()
{
    int count;
    char ch;

    fptr = fopen(myfile, "r");
    while (getc(fptr) != EOF)
    {
        fscanf(fptr, "%c", &ch);
        if (ch == '\n')
            count++;
    }
    fclose(fptr);
    return count;
}

void initialise()
{
    l = line();
    count = (int *)calloc(l, sizeof(int));
    str = (char ***)calloc(l, sizeof(char));
    fptr = fopen(myfile, "w+");
    for (i = 0; i < l; i++)
    {
        while (getc(fptr) != '\n')
        {
             if (getc(fptr) == '\0')
                count[i]++;
        }
    }
    for (i = 0; i < l; i++)
    {
        str[i] = (char **)calloc(count[i], sizeof(char));
        for (j = 0; j < count[i]; j++)
        {
            str[i][j] = (char *)calloc(PATH_MAX, sizeof(char));
        }
    }
    for (i = 0; getc(fptr) != EOF; i++)
    {
        for (j = 0; getc(fptr) != '\n'; j++)
        {
            fscanf(fptr, "%s", &str[i][j][0]);
        }
    }
}
int arg1(char ch)
{ 
    //if (ch == 'a')
    return 0;
}

int arg2(const char *choice)
{
    return 0;
}

int add(int argc, char *argv[], int n)
{

    char full_path[_PC_PATH_MAX];
    fptr = fopen(myfile, "w+");
    fseek(fptr, 0, SEEK_END);
    if (fptr == NULL)
    {
        perror("Error!");
        exit(1);
    }
    if (move == true)
        fprintf(fptr, "mv ");
    else
        fprintf(fptr, "cp ");
    for (int i = n; i < argc; i++)
    {
        if (absolute_path(argv[i], full_path) == 0)
            fprintf(fptr, "%s ", full_path);
    }
    fprintf(fptr, "%c", '\n');
    fclose(fptr);
    printf("\nFile Closed\n");
    return 0;
}

int copy_file(char* source_path,char* dest_path)
{
    char *prevptr,*ptr=source_path,ch;
    sptr=fopen(source_path,"r");
    while( (ptr = strstr(ptr,"/")))
        prevptr = ptr++;
    strcat(dest_path,prevptr);   
    dptr=fopen(dest_path,"r");
    re:
    if(dptr!=NULL)
        printf("\n%s file exists in directory. Would you like to overwrite it (y/n) ? ");
        scanf("%c",&ch);
        tolower(ch);
        if(ch=='n')
            return -1;
        else if(ch=='y')
            {
                if((remove(dest_path))!=0)
                    printf("\nThe File cannot be deleted.");
                return -1;
            }
        else
            printf("\nPlease enter a Valid option.");
            goto re;
    dptr=fopen(dest_path,"w+");
    while ((ch  = fgetc(sptr)) != EOF)
        fputc(ch, dptr);
    if (move == true)
        remove(source_path);            
    return 0;
}

int paste(int argc, char *argv[], int n)
{
    char source_path[10000] = {'\0'};
    char dest_path[_PC_PATH_MAX];
    cwd_path(dest_path);
    fptr = fopen(myfile, "r+");
    if (fptr == NULL)
    {
        perror("Error!");
        exit(1);
    }
    while (getc(fptr) != EOF)
    {
        fscanf(fptr, "%s", source_path); // Need a dynamic arry and string concate
        copy_file(source_path,dest_path);
    }
    fclose(fptr);
    return 0;
}

int clear()
{
    remove(myfile);
    fptr=fopen(myfile,"wb+");
    fclose(fptr);
    return 0;
}
int list(char *options) //options can be a range or a single number
{

    char string[10000] = {'\0'};
    fptr = fopen(myfile, "r");
    if (options == NULL)
        for (int i = 1; getc(fptr) != EOF; i++)
        {
            fscanf(fptr, "%[^\n]s", string);
            printf("%d. %s\n ", i, string);
        }
    fclose(fptr);
    return 0;
}

int remove_line(char *options) //options can be a range or a single number
{
    int l1=0,l2=0,p=-1,len=0,start=0;
    initialise();
    if (options[0] == '-')
    {
        clear();
        return 0;
    }
    else if (isdigit(options[0]))
    {
        for (i = 1; options[i] != '\0'; i++)
        {
            if (options[i] == '-')
            {
                if(p!=-1)
                    return -1;
                p=i;
            }
            else if(!isdigit(options[i]))
                return -1;
        }
    }
    else return -1;
    /*for(i=1;i<p;i++)
    {
        if (!isdigit(options[i]))
            return -1;
    }
    for(i=p+1;options[i]!='\0';i++)
    {
        if (!isdigit(options[i]))
            return -1;
    }*/
    l1 = atoi(options);
    if (l1 <= 0 || l1 > l)
        return -1;
    if(p)
    {
        l2=atoi(&options[p+1]);
        if(l2<l1||l2>=l)
            return -1;
    }
    if (l2 == 0)
    {
        len = 1;
        start = l1;
    }
    else
    {
        len = l2 - l1 + 1;
        start = l2;
    }
    for (i = start; i < l; i++)
    {
        str[i - len] = (char **)realloc(str + i - len, count[i] * sizeof(char));
        memset(str + i - len, '\0', count[i] * PATH_MAX * sizeof(char));
        for (j = 0; j < count[i]; j++)
        {
            for (k = 0; k < PATH_MAX; k++)
            {
                str[i - len][j][k] = str[i][j][k];
            }
        }
    }
    for (i = start; i < l; i++)
    {
        count[i - len] = count[i];
    }
    l=l-len;
    clear();
    for (i = 0; i < l; i++)
    {
        for (j = 0; j < count[i]; j++)
        {
            fprintf(fptr, "%s", &str[i][j][0]);
            fprintf(fptr, "\0");
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
    return 0;
}

int modify(int argc, char *argv[], int n, int opt1, int opt2) //options can be a range or a single number
{

    if (opt2 == -1) // clearing all the lines and replacing them
    {
        int temp = argc - n;
        str[opt1 - 1] = (char **)realloc(str + opt1 - 1, sizeof(char) * temp); //free other memory spaces

        memset(str + opt1 - 1, '\0', temp * PATH_MAX * sizeof(char));
        for (i = 0; i < temp; i++)
            strcpy(&str[opt1 - 1][i][0], argv[i + n]);
        count[opt1 - 1] = temp;
    }
    else
    {
        memset(&str[opt1 - 1][opt2 - 1], '\0', PATH_MAX * sizeof(char));
        strcpy(&str[opt1 - 1][opt2 - 1][0], argv[i + n]);
    }
    clear();
    for (i = 0; i < l; i++)
    {
        for (j = 0; j < count[i]; j++)
        {
            for (k = 0; str[i][j][k] != '\0'; k++)
            {
                fprintf(fptr, "%c", str[i][j][k]);
            }
            fprintf(fptr, 0);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
    return 0;
}

void help()
{
    printf("++add or +a to add file\n");
    printf("++paste or +p to paste file\n");
    printf("++list or +l to list all the copied files\n");
    printf("++modify to modify the copied files\n");
    printf("++remove or +r to remove the copied files\n");
    printf("++clear or +c to clear contents copied files\n");
}

int execute() //All the execution of the functions r gonna be made here
{
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("\n%s requires at least one argument.\nPlease try %s ++help for more information.\n", argv[0], argv[0]);
        return 0;
    }
    char choice[20];
    for (int i = 1; i < argc; i++)
    {
        strcpy(choice, argv[i]);
        if (choice[0] != '+')
            break;
        else if (choice[1] == '+')
        {
            arg2(choice);
        }
        else
        {
            for (j = 1; choice[j] != '\0'; j++)
                arg1(choice[j]);
        }
    }
    execute();
    return 0;
}
