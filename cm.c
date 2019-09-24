
#include <stdio.h>
#include <string.h>  // For strlen, strcpy and strcat functions
#include <stdlib.h>  // For exit(), system(), atoi(), rename() and remove() functions
#include <limits.h>  // For PATH_MAX
#include <stdbool.h> // For bool datatypes
#include <unistd.h>  // For getcwd() and realpath() functions
#include <ctype.h>   // For isdigit() function

int find_lines(char *);
int line();
int arg1(char);
int arg2(const char *, char **);
int add(int, char **);
int paste();
int clear();
int list();
int remove_line();
int modify(int, char **);
void help();
int execute(int, char **);

const char myfile[] = "/root/Desktop/cm/.clipboard";
const char myfilenew[] = "/root/Desktop/cm/.clipboard.new";

int i, j, k, n = 1, line_start = 0, line_end = 0;

enum mode
{
    NONE,
    ADD,
    PASTE,
    MODIFY,
    REMOVE
};

enum mode CURRENT_RUN = NONE;   // CURRENT_RUN is the driver object of the program
static bool Move = false;       // Move is used to decide between moving and copying [Default is copying]
static bool Error_mode = false; // Error_mode when set to true displays additional details for troubleshooting
static bool Paste_mode = false; // Paste_mode is used to decide if the files are going to be copied or moved when the paste() is executed

int cwd_path(char *cwd) // USed to get the Current Working Directory
{
    if (getcwd(cwd, 10000) != NULL)
        return 0;
    else
    {
        perror("getcwd() error");
        exit(99);
    }
}

int absolute_path(int flg, char *file_path, char *full_path) //Used to get the absolute path of the files
{
    if (realpath(file_path, full_path) != NULL)
        return 0;
    else
    {
        if (flg != 0)
            printf("%s can not be found.\n", file_path);
        return -1;
    }
}

int errors(int err) // This Function is used to display errors and terminate the program accordingly
{
    if (Error_mode == true)
        printf("Error Code : %d\n", err);
    switch (err)
    {
    case 1:
        printf("The additonal arguments are not evaluated.\nPlease use the options ++help or +h\n");
        break;
    case -10:
        printf("Please enter valid combination of options.\n");
        break;
    case -11:
        printf("Please enter valid options.\n");
        break;
    case 50:
        printf("Please enter the starting index of the range!!\nThe Current List contains: \n");
        list();
        break;
    case 51:
        printf("Please enter a valid range!!\nThe Current List contains: \n");
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

int find_lines(char *options) // Used to parse the argument provided as a string to integer variables
{                             // Example: "2-5" as string to 2 and 5 as integers
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
    return 0;
}

int line() // Used to count the number of lines in the clipboard file
{
    FILE *fptr;
    int count = 0;
    char ch;

    fptr = fopen(myfile, "r");
    if (fptr == NULL)
        return 0;
    ch = getc(fptr);
    while (ch != EOF)
    {
        if (ch == '\n')
            count++;
        ch = getc(fptr);
    }
    fclose(fptr);
    return count;
}

int arg1(char ch) // Used to set global variables according to the options[ starting with a '+' ] provided
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
    case 'c':
        Move=false;
        Paste_mode=true;
        break;
    case 'x':
        Move = true;
        Paste_mode=true;
        break;
    case 'e':
        Error_mode = true;
    default:
        errors(-11);
    }
    return 0;
}

int arg2(const char *choice, char *argv[]) // Used to set global variables according to the options[ starting with a '++' ] provided
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
    {
        Move = true;
        Paste_mode=true;
    }
    else if (strcmp(&choice[2], "copy") == 0)
    {
        Move = false;
        Paste_mode=true;
    }
    else if (strcmp(&choice[2], "error") == 0)
        Error_mode = true;
    else if (strcmp(&choice[2], "range") == 0)
    {
        int result = find_lines(&argv[++n][0]);
        if (result != 0)
            errors(0);
    }
    else if (strcmp(&choice[2], "paste") == 0)
    {
        if (CURRENT_RUN == NONE)
            CURRENT_RUN = PASTE;
        else
            errors(-10);
    }
    else
        errors(-11);
    return 0;
}

int add(int argc, char *argv[]) // Used to add the files' absolute path to the clipboard
{
    FILE *fptr;
    char full_path[10000], flg = 0;
    if (n == argc)
        errors(1001);
    fptr = fopen(myfile, "a+");

    if (fptr == NULL)
        errors(1000);

    for (int i = n; i < argc; i++)
    {
        if (((absolute_path(0, argv[i], full_path) == 0)||argv[i][0]=='-') && flg == 0)
        {
            if (Move == true)
                fprintf(fptr, " mv ");
            else
                fprintf(fptr, " cp ");
            flg = 1;
        }
        if(argv[i][0]=='-')
            fprintf(fptr, "%s ", &argv[i][0]);

        else if (absolute_path(1, argv[i], full_path) == 0)
            fprintf(fptr, "%s ", full_path);
    }
    fprintf(fptr, "%c", '\n');
    fclose(fptr);
    return 0;
}

int paste() // Used to execute the lines stored in the clipboard file
{
    FILE *sptr, *dptr;
    char ch = 0, source_path[10000] = {0};
    //char *dest_path = (char *)malloc(sizeof(char) * 10000);
    char dest_path[10000] = "/root/Desktop/cm/";
    int Current_line = 1;
    //cwd_path(dest_path);
    sptr = fopen(myfile, "r+");

    if (sptr == NULL)
        errors(500);

    if (line_start == 0 && line_end == 0)
        line_end = line();
    else
    {
        while (Current_line != line_start)
        {
            ch = getc(sptr);
            if (ch == '\n')
                Current_line++;
        }
    }

    
    while (getc(sptr) != EOF)
    {
        fscanf(sptr, "%[^\n]s", source_path);
        if(Paste_mode==true)
        {
            if(Move==true)
            {
                source_path[0]=' ';
                source_path[1]='m';
                source_path[2]='v';
            }
            else
            {
                source_path[0]=' ';
                source_path[1]='c';
                source_path[2]='p';
            }   
        }
        strcat(source_path, dest_path);
        system(source_path);
        if (Current_line == line_end)
            break;
        ch = getc(sptr);
    } 
    return 0;
}

int clear() // Empties the clipboard by deleting the old one and creating a new clipboard file
{
    FILE *fptr;
    remove(myfile);
    fptr = fopen(myfile, "w+");
    fclose(fptr);
    return 0;
}

int list() //Used to list all the contents of the clipboard
{
    FILE *fptr;
    char string[10000] = {'\0'};
    fptr = fopen(myfile, "r");
    printf(" ");
    for (int i = 1; fgetc(fptr) != EOF; i++)
    {
        fscanf(fptr, "%[^\n]s", string);
        printf("%d. %s\n", i, string + 3);
    }
    fclose(fptr);
    return 0;
}

int remove_line() // Used to remove specific line in the clipboard file
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

int modify(int argc, char *argv[]) // Used to modify specific lines in the clipboard file
{
    char ch, file_to_add[10000], flg = 0, flg1 = 0;
    int Current_line = 1;
    FILE *sptr, *dptr;
    sptr = fopen(myfile, "r");
    if (sptr == NULL)
        errors(200);

    dptr = fopen(myfilenew, "w");

    ch = getc(sptr);
    if (line_start == 0)
        errors(201);
    if (line_end == 0)
        line_end = line_start;

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
                    if ((absolute_path(0, &argv[i][0], file_to_add) == 0) && flg1 == 0)
                    {
                        if (Move == true)
                            fprintf(dptr, " mv ");
                        else
                            fprintf(dptr, " cp ");
                        flg1 = 1;
                    }

                    if (absolute_path(1, &argv[i][0], file_to_add) == 0)
                        fprintf(dptr, "%s ", file_to_add);
                }
                fprintf(dptr, "%c", "\n");
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
    system("clear");
    printf("\n\n\t#### AVAILABLE OPTIONS ##### \n\n\n");
    printf("\t1. To add file to the clipboard\n");
    printf("\t\t+a\n\t\t++add\n\n");
    printf("\t  The format is:\n");
    printf("\t  ++add [file 1] [file 2] ...\n\n");
    printf("\n\t2.To paste copied files to the clipboard\n");
    printf("\t\t+p\n\t\t++paste\n\n");
    printf("\t  The format is:\n");
    printf("\t  ++paste -> Will paste all the files copied in the clipboard into the current directory\n\n");
    printf("\t  ++paste ++range [line number]");
    printf(" -> Will paste the file in the given line number's respective file in the current directory\n\n");
    printf("\t  ++paste ++range [line number 1]-[line number 2]");
    printf(" -> Will paste the files in the respective range of lines in the current directory\n\n");
    printf("\n\t3.To list all the files copied\n");
    printf("\t\t+l\n\t\t++list\n\n");
    printf("\n\t4.To modify the copied files\n");
    printf("\t\t++modify\n\n");
    printf("\t  The format is:\n");
    printf("\t  ++modify ++range [line number] [new file 1] [new file 2] ...\n\n\t");
    printf(" -> Will remove the file copied in the line number and replace it with the new files\n\n");
    printf("\t  ++modify ++range [line number 1]-[line number 2] [new file 1] [new file 2] ...\n\n\t");
    printf(" -> Will remove the files copied in the respective reange of lines and replace it with the new files\n\n");
    printf("\n\t5.To remove the files in the clipboard\n");
    printf("\t\t+r\n\t\t++remove\n\n");
    printf("\t  The format is:\n");
    printf("\t  ++remove -> Will remove all the files copied in the clipboard\n\n");
    printf("\t  ++remove ++range [line number]");
    printf(" -> Will remove the file in the given line number from the clipboard\n\n");
    printf("\t  ++remove ++range [line number 1]-[line number 2]\n\n\t");
    printf(" -> Will remove the files in the respective range of lines copied in the clipboard\n\n");
    printf("\n\t6.For trouble shooting\n");
    printf("\t\t+e\n\t\t++error\n\n");
}

int execute(int argc, char *argv[]) // Used to execute the functions as requested by the options
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
        modify(argc, argv);
    return 0;
}

int main(int argc, char *argv[]) // The Main function with command line arguments
{

    if (argc <= 1)
    {
        printf("Please enter a valid option.\n%s requires atleast one option\
                \nTry the %s ++help or %s +h for more information\n",
               argv[0], argv[0], argv[0]);
        return 0;
    }
    char choice[20];
    for (n = 1; n < argc; n++) // Loop to parse for the options in the command line arguments
    {
        strcpy(choice, argv[n]);
        if (choice[0] != '+')
            break;
        else if (choice[1] == '+')
        {
            arg2(choice, argv);
        }
        else
        {
            for (j = 1; choice[j] != '\0'; j++)
                arg1(choice[j]);
        }
    }
    execute(argc, argv);
    return 0;
}
