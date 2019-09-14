#define PATH_MAX _PC_PATH_MAX
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include "temp.h"
int line();

const char myfile[] = "/root/Desktop/cm/clipboard";

FILE *fptr;

int n = 1, l = 0, *count;
char ***str;
static bool function_mode = false; // False in function_mode indicates that the execution is for adding into clipboard
static bool copy_move = false;     // False in copy_move is to sent copy and true for moving

int line();

int shoot = 0;
void initialise()
{
    int i = 0, j = 0;
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
    if (copy_move == true)
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

int paste(int argc, char *argv[], int n)
{

    char cmd[10000] = {'\0'};
    char dest_path[_PC_PATH_MAX];
    fptr = fopen(myfile, "r+");
    if (fptr == NULL)
    {
        perror("Error!");
        exit(1);
    }
    while (getc(fptr) != EOF)
    {
        fscanf(fptr, "%[^\n]s", dest_path); // Need a dynamic arry and string concate
        strcat(cmd, dest_path + 2);
        if (copy_move == true)
            //*arr = str_replace(arr, "&&cp", "&&mv");
            system(cmd);
        printf("%s\n", cmd);
    }
    fclose(fptr);
    return 0;
}

int clear()
{
    char cmd[50] = "rm ";
    strcat(cmd, myfile);
    strcat(cmd, "; touch ");
    strcat(cmd, myfile);
    system(cmd);
    printf("\nLog cleared.\n");
    return 0;
}

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
    return 0;
}

int modify(int argc, char *argv[], int n, int opt1, int opt2) //options can be a range or a single number
{
    int i = 0, j = 0, k = 0;
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
    int n = 1, j;
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
