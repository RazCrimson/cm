#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include "temp.h"

const char str[] = "./myfile.txt";

static bool function_mode = false; // False in function_mode indicates that the execution is for adding into clipboard
static bool copy_move = false;     // False in copy_move is to sent copy and true for moving

int shoot = 0;

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
    FILE *fptr;
    char full_path[_PC_PATH_MAX];
    fptr = fopen(str, "w+");
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
    FILE *fptr;
    char cmd[10000] = {'\0'};
    char dest_path[_PC_PATH_MAX];
    fptr = fopen(str, "r+");
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
    return 0;
}

int clear()
{
    char cmd[50] = "rm ";
    strcat(cmd, str);
    strcat(cmd, "; touch ");
    strcat(cmd, str);
    system(cmd);
    printf("\nLog cleared.\n");
    return 0;
}

int list(char *options) //options can be a range or a single number
{
    FILE *fptr;
    char string[10000] = {'\0'};
    fptr = fopen(str, "r");
    if (options == NULL)
        for (int i = 1; getc(fptr) != EOF; i++)
        {
            fscanf(fptr, "%[^\n]s", string);
            printf("%d. %s\n ", i, string);
        }
    return 0;
}

int remove_line(char *options) //options can be a range or a single number
{
    return 0;
}

int modify(int argc, char *argv[], int n, char *options) //options can be a range or a single number
{
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
