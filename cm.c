
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>

int find_lines(char *);
int line();
int arg1(char);
int arg2(const char *, char **);
int add(int, char **);
int paste();
int clear();
int list();
int remove_line();
int modify(int ,char **);
void help();
int execute(int, char **);

const char myfile[] = "/root/Desktop/cm/clipboard";
const char myfilenew[] = "/root/Desktop/cm/clipboard.new";

FILE *sptr, *dptr;

int i, j, k, n = 1, line_start = 0, line_end = 0;

enum mode
{
    NONE,
    ADD,
    PASTE,
    MODIFY,
    REMOVE
};

enum mode CURRENT_RUN = NONE; // False in function_mode indicates that the execution is for adding into clipboard
static bool Move = false;     // False in copy_move is to sent copy and true for moving
static bool Error_mode = false;

int cwd_path(char *cwd)
{
    if (getcwd(cwd, 10000) != NULL)
        return 0;
    else
    {
        perror("getcwd() error");
        exit(99);
    }
}

int absolute_path(char *file_path, char *full_path)
{
    if (realpath(file_path, full_path) != NULL)
        return 0;
    else
    {
        printf("%s can not be found.\n", file_path);
        return -1;
    }
}

int errors(int err)
{
    if (Error_mode == true)
        printf("Error Code : %d\n", err);
    switch (err)
    {
    case 1:
        printf("The additonal arguments are not evaluated.\nPlease use the options ++help or +h\n");
        break;
    case -10:
        printf("Please enter valid combination of options.\n", err);
        break;
    case -11:
        printf("Please enter valid options.\n", err);
        break;
    case 50:
        printf("Please enter the starting index of the range!!\nThe Current List contains: ");
        list();
        break;
    case 51:
        printf("Please enter a valid range!!\nThe Current List contains: ");
        list();
        break;
    case 52:
        printf("Please enter a range!!, Your input contains a non-digit value in the range.\n");
        break;
    case 53:
        printf("Please enter a valid range!!, Your input contains more than one '-'.\nPlease use one '-' to specify the range.\n");
        break;
    //case 99: reserverd for cwd_path
    case 100:
    case 200:
    case 500:
    case 1000:
        printf("The Clipboard file is not found or opened!\n");
        break;
    case 101:
        printf("The start of the range to be removed is Invalid(0)\n");
        break;
    case 201:
        printf("The start of the range to be modified is Invalid(0)\n");
        break;
    case 1001:
        printf("The add option requires additional arguments\n");
        break;
    default:
        printf("Unknown Error!");
    }
    exit(err);
}

int find_lines(char *options)
{
    int p = -1, len = line();
    if (options[0] == '-')
        errors(50);
    else if (isdigit(options[0]))
    {
        for (i = 1; options[i] != '\0'; i++)
        {
            if (options[i] == '-')
            {
                if (p != -1)
                    errors(53);
                p = i;
            }
            else if (!isdigit(options[i]))
                errors(52);
        }
    }
    else
        errors(51);
    line_start = atoi(options);
    if (line_start <= 0 || line_start > len)
        errors(51);
    if (p != -1)
    {
        line_end = atoi(&options[p + 1]);
        if (line_end < line_start || line_end >= len)
            errors(51);
    }
    n++;
    return 0;
}

int line()
{
    int count = 0;
    char ch;

    sptr = fopen(myfile, "r");
    if (sptr == NULL)
        return 0;
    ch = getc(sptr);
    while (ch != EOF)
    {
        if (ch == '\n')
            count++;
        ch = getc(sptr);
    }
    fclose(sptr);
    return count;
}

int arg1(char ch)
{
    switch (ch)
    {
    case 'm':
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = MODIFY;
        else
            errors(-10);
        break;
    }
    case 'a':
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = ADD;
        else
            errors(-10);
        break;
    }
    case 'p':
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = PASTE;
        else
            errors(-10);
        break;
    }
    case 'r':
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = REMOVE;
        else
            errors(-10);
        break;
    }
    case 'l':
        list();
        break;
    case 'h':
        help();
        break;
    case 'x':
        Move = true;
        break;
    case 'e':
        Error_mode = true;
    default:
        errors(-11);
    }
    return 0;
}

int arg2(const char *choice, char *argv[])
{
    if (strcmp(&choice[2], "modify") == 0)
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = MODIFY;
        else
            errors(-10);
    }
    else if (strcmp(&choice[2], "add") == 0)
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = ADD;
        else
            errors(-10);
    }
    else if (strcmp(&choice[2], "list") == 0)
        list();
    else if (strcmp(&choice[2], "help") == 0)
        help();
    else if (strcmp(&choice[2], "remove") == 0)
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = REMOVE;
        else
            errors(-10);
    }
    else if (strcmp(&choice[2], "move") == 0)
        Move = true;
    else if (strcmp(&choice[2], "error") == 0)
        Error_mode = true;
    else if (strcmp(&choice[2], "range") == 0)
    {
        int result = find_lines(&argv[n][0]);
        if (result != 0)
            errors(0);
    }
    else
        errors(-11);
    return 0;
}

int add(int argc, char *argv[])
{
    char full_path[10000], flg = 0;
    if (n == argc)
        errors(1001);
    sptr = fopen(myfile, "a+");

    if (sptr == NULL)
        errors(1000);

    for (int i = n; i < argc; i++)
    {
        if ((absolute_path(argv[i], full_path) == 0) && flg == 0)
        {
            if (Move == true)
                fprintf(sptr, " mv ");
            else
                fprintf(sptr, " cp ");
            flg = 1;
        }

        if (absolute_path(argv[i], full_path) == 0)
            fprintf(sptr, "%s ", full_path);
    }
    fprintf(sptr, "%c", '\n');
    fclose(sptr);
    return 0;
}

int paste()
{
    char ch, source_path[100000] = {""};
    char *dest_path = (char *)malloc(sizeof(char) * 10000);
    int Current_line = 1;
    cwd_path(dest_path);
    sptr = fopen(myfile, "r+");
    if (sptr == NULL)
        errors(500);

    if (line_start == 0 && line_end == 0)
    {
        line_end = line();
        goto file_copy;
    }

    while (Current_line != line_start)
    {
        if (ch == '\n')
            Current_line++;
        ch = getc(sptr);
    }

file_copy:
    ch = getc(sptr);
    while (ch != EOF)
    {
        fscanf(sptr, "%[^\n]s", source_path); // Need a dynamic arry and string concate
        strcat(source_path, dest_path);
        system(source_path);
        if (Current_line == line_end)
            break;
        ch = getc(sptr);
    }
    return 0;
}

int clear()
{
    remove(myfile);
    sptr = fopen(myfile, "w+");
    fclose(sptr);
    return 0;
}

int list()
{
    char string[10000] = {'\0'};
    sptr = fopen(myfile, "r");
    for (int i = 1; fgetc(sptr) != EOF; i++)
    {
        fscanf(sptr, "%[^\n]s", string);
        printf("%d. %s\n ", i, string);
    }
    fclose(sptr);
    return 0;
}

int remove_line()
{
    char ch;
    int Current_line = 1;
    FILE *sptr, *dptr;
    sptr = fopen(myfile, "r");
    if (sptr == NULL)
        errors(100);
    dptr = fopen(myfilenew, "w");

    if (line_start == 0)
        errors(101);
    if (line_end == 0)
        line_end = line_start;

    ch = getc(sptr);
    while (ch != EOF)
    {
        if (ch == '\n')
            Current_line++;

        if ((Current_line < line_start) || (Current_line > line_end))
            putc(ch, dptr);

        ch = getc(sptr);
    }
    fclose(sptr);
    fclose(dptr);
    remove(myfile);
    rename(myfilenew, myfile);
    return 0;
}

int modify(int argc,char *argv[])
{
    char ch,file_to_add[10000], flg = 0,flg1=0;
    int Current_line = 1;
    FILE *sptr, *dptr;
    sptr = fopen(myfile, "r");
    if (sptr == NULL)
        errors(200);

    dptr = fopen(myfilenew, "w");

    ch = getc(sptr);
    if (line_start == 0)
        errors(201);

    while (ch != EOF)
    {
        if (ch == '\n')
            Current_line++;

        if ((Current_line < line_start) || (Current_line > line_end))
            putc(ch, dptr);
        else if ((Current_line >= line_start) && (Current_line <= line_end))
        {
            if (flg == 0)
            {
                for (int i = n; i < argc; i++)
                {
                    if ((absolute_path(&argv[i][0], file_to_add) == 0) && flg1 == 0)
                    {
                        if (Move == true)
                            fprintf(dptr, "\n mv ");
                        else
                            fprintf(dptr, "\n cp ");
                        flg1=1;
                    }

                    if (absolute_path(&argv[i][0], file_to_add) == 0)
                        fprintf(dptr, "%s ", file_to_add);
                }
            
            }
            flg = 1;
        }
        ch = getc(sptr);
    }
    fclose(sptr);
    fclose(dptr);
    remove(myfile);
    rename(myfilenew, myfile);
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

int execute(int argc, char *argv[]) //All the execution of the functions r gonna be made here
{
    if (CURRENT_RUN == ADD)
        add(argc, argv);
    else if (CURRENT_RUN == PASTE)
        paste();
    else if (CURRENT_RUN == REMOVE)
    {
        if (line_start == 0 && line_start == 0)
            clear();
        else
            remove_line();
    }
    else if (CURRENT_RUN == MODIFY)
        modify(argc,argv);
    return 0;
}

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        printf("Please enter a valid option.\n%s requires atleast one option\
                \nTry the %s ++help or %s +h for more information\n",
               argv[0], argv[0], argv[0]);
        return 0;
    }
    char choice[20];
    for (i = 1; i < argc; i++)
    {
        strcpy(choice, argv[i]);
        if (choice[0] != '+')
            break;
        else if (choice[1] == '+')
        {
            n++;
            arg2(choice, argv);
        }
        else
        {
            n++;
            for (j = 1; choice[j] != '\0'; j++)
                arg1(choice[j]);
        }
    }
    execute(argc, argv);
    return 0;
}
