#define PATH_MAX _PC_PATH_MAX
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "temp.h"

int find_lines(char *);
int line();
int initialise();
int arg1(char);
int arg2(const char *,char **);
int add(int,char **);
int paste();
int clear();
int list();
int remove_line();
int modify(char *);
void help();
int execute(int,char **);

const char myfile[] = "/root/Desktop/cm/clipboard";
const char myfilenew[] = "/root/Desktop/cm/clipboard.new";

FILE *fptr;
//FILE *fptr,*sptr, *dptr;

int i, j, k, n = 1, l = 0,line_start=0,line_end=0, *count;
char ***str;
enum mode {NONE,ADD,PASTE,MODIFY,REMOVE};
enum mode CURRENT_RUN = NONE; // False in function_mode indicates that the execution is for adding into clipboard
static bool move = false;          // False in copy_move is to sent copy and true for moving



int find_lines(char *options)
{
    int p=-1,l=line();
    if (options[0] == '-')
        return -2;                      
    else if (isdigit(options[0]))
    {
        for (i = 1; options[i] != '\0'; i++)
        {
            if (options[i] == '-')
            {
                if (p != -1)
                    return -1;
                p = i;
            }
            else if (!isdigit(options[i]))
                return -1;
        }
    }
    else
        return -1;
    line_start = atoi(options);
    if (line_start <= 0 || line_start > l)
        return -1;
    if (p != -1)
    {
        line_end = atoi(&options[p + 1]);
        if (line_end < line_start || line_end >= l)
            return -1;
    }
    return 0;
}

int line()
{
    int count=0;
    char ch;

    fptr = fopen(myfile, "r");
    ch = getc(fptr);
    while (ch!= EOF)
    {
        if (ch == '\n')
            count++;
        ch=getc(fptr);       
    }
    fclose(fptr);
    return count;
}

int initialise()
{
    int length,l,*count;
    char ***str;
    l = line();
    count = (int *)calloc(l, sizeof(int));
    str = (char ***)calloc(l, sizeof(char));
    fptr = fopen(myfile, "r+");
    for (i = 0; i < l; i++)
    {
       // printf("i=%d\n",i); //trial
        while (fgetc(fptr)!='\n')
        {
          //  printf("i=%d\n",i);
            fseek(fptr,-1,SEEK_CUR);
            if (fgetc(fptr) == ' ')
            {
                ++count[i];
                
            }
        }
        ++count[i];
        //printf("count[%d]=%d\n",i,t);
    }
    fclose(fptr);
    for (i = 0; i < l; i++)
    {
        //printf("i=%d\n",i);
        str[i] = (char **)calloc(count[i], sizeof(char));
        for (j = 0; j < count[i]; j++)
        {
            //printf("j=%d\n",j);
            str[i][j] = (char *)calloc(PATH_MAX, sizeof(char));
        }
    }
    
/*    for(i=0;i<l;i++)
    {
        for(j=0;j<count[i];j++)
        {
            for(k=0;k<PATH_MAX;k++)
            {
                if(str[i][j][k]=='\0')
                printf("0");
            }
            printf("\t");
        }
        printf("\n");
    }*/
    fptr = fopen(myfile, "r+");
    for (i = 0; i<l; i++)
    {
        //printf("i=%d\n",i);
        for (j = 0;j<count[i]; j++)
        {
            for(k=0;fgetc(fptr)!=' '&&(fseek(fptr,-1,SEEK_CUR),fgetc(fptr)!='\n');k++)
            {
                //printf("k=%d\n",k);
                fseek(fptr,-1,SEEK_CUR);
                str[i][j][k]=fgetc(fptr);

            }
            str[i][j][k]='\0';
        }
    }
/*    for(i=0;i<l;i++)
    {
        for(j=0;j<count[i];j++)
        {
            printf("%s\t",str[i][j]);
        }
        printf("\n");
    }*/
    return l;
}
int arg1(char ch)
{   
    switch(ch)
    {
        case 'm':
            {
                if(CURRENT_RUN==NONE)
                    CURRENT_RUN=MODIFY;
                else
                    errors(-10);
                break;
            }
        case 'a':
            {
                if(CURRENT_RUN==NONE)
                    CURRENT_RUN=ADD;
                else
                    errors(-10);
                break;
            }
        case 'p':
            {
                if(CURRENT_RUN==NONE)
                    CURRENT_RUN=PASTE;
                else
                    errors(-10);
                break;
            }
        case 'r':
            {
                if(CURRENT_RUN==NONE)
                    CURRENT_RUN=REMOVE;
                else
                    errors(-10);
                break;   
            }
        case 'l': list(); break;
        case 'h': help(); break;
        case 'x': move=true; break;
        default:
            errors(-11);
    }
    return 0;
}

int arg2(const char *choice,char *argv[])
{
    if(strcmp(&choice[2],"modify")==0)
    {
        if(CURRENT_RUN==NONE)
            CURRENT_RUN=MODIFY;
        else
            errors(-10);
    }
    else if(strcmp(&choice[2],"add")==0)
    {
        if(CURRENT_RUN==NONE)
            CURRENT_RUN=ADD;
        else
            errors(-10);
    }
    else if(strcmp(&choice[2],"list")==0)
        list();
    else if(strcmp(&choice[2],"help")==0)
        help();
    else if(strcmp(&choice[2],"remove")==0)
    {
        if(CURRENT_RUN==NONE)
            CURRENT_RUN=REMOVE;
        else
            errors(-10);
    }
    else if(strcmp(&choice[2],"move")==0)
        move=true;
    else if(strcmp(&choice[2],"range")==0)
    {
        int result=find_lines(&argv[n][0]);
        if(result==0);
        else if(result==-1)
        {
            printf("\nPlease enter a valid range!!\nThe Current List contains: ");
            list();
            exit(-1);
        }
        else if(result==-2)
        {
            printf("\nPlease enter a initial value for range!!\nThe Current List contains: ");
            list();
            exit(-1);
        }
        else
        {
            /* code */
        }
        

    }
    else 
        errors(-11);
    return 0;
}

int add(int argc, char *argv[])
{
    char full_path[10000];//PATH_MAX
    fptr = fopen(myfile, "a+");
    fseek(fptr, 0, SEEK_END);
    if (fptr == NULL)
    {
        perror("Error!");
        exit(1);
    }
    if (move == true)
        fprintf(fptr, " mv ");
    else
        fprintf(fptr, " cp ");
    for (int i = n; i < argc; i++)
    {
        if (absolute_path(argv[i], full_path) == 0)
            fprintf(fptr, "%s ", full_path);
    }
    fprintf(fptr, "%c", '\n');
    fclose(fptr);
    return 0;
}

/*int copy_file(char *source_path, char *dest_pelse if(strcmp(&choice[2],"add")==0)ath)
{
    char *prevptr, *ptr = source_path, ch;
    sptr = fopen(source_path, "r");
    while ((ptr = strstr(ptr, "/")))
        prevptr = ptr++;
    strcat(dest_path, prevptr);
    dptr = fopen(dest_path, "r");
re:
    if (dptr != NULL)
        printf("\n%s file exists in directory. Would you like to overwrite it (y/n) ? ");
    scanf("%c", &ch);
    tolower(ch);
    if (ch == 'n')
        return -1;
    else if (ch == 'y')
    {
        if ((remove(dest_path)) != 0)
            printf("\nThe File cannot be deleted.");
        return -1;
    }
    else
        printf("\nPlease enter a Valid option.");
    goto re;
    dptr = fopen(dest_path, "w+");
    while ((ch = fgetc(sptr)) != EOF)
        fputc(ch, dptr);
    if (move == true)
        remove(source_path);
    return 0;
}*/

int paste()
{
    char source_path[100000] = {""};
    char *dest_path = (char *)malloc(sizeof(char) * 100000);
    cwd_path(dest_path);
    fptr = fopen(myfile, "r+");
    if (fptr == NULL)
    {
        perror("Error!");
        exit(1);
    }
       
    if(line_start>0)
    {
        while(--line_start)
        {
            fscanf(fptr,"%[^\n]s ",source_path);
            if(line_end>0)
                line_end--;
            if(fgetc(fptr)==EOF)
            {
                printf("ERROR the lines specified exccced the no. of entries");
                exit(-1);
            }
        }
    }
    if (line_end == NULL)
    {
        while (fgetc(fptr) != EOF)
        {
            fscanf(fptr, "%[^\n]s", source_path+3); // Need a dynamic arry and string concate
            strcat(source_path,dest_path);
            system(source_path);
            if(!(line_end--))
                break;
        }
    }
    return 0;
}

int clear()
{
    remove(myfile);
    fptr = fopen(myfile, "w+");
    fclose(fptr);
    return 0;
}
int list()
{
    char string[10000] = {'\0'};
    fptr = fopen(myfile, "r");
    for (int i = 1; fgetc(fptr) != EOF; i++)
    {
        fscanf(fptr, "%[^\n]s", string);
        printf("%d. %s\n ", i, string);
    }
    fclose(fptr);
    return 0;
}

int remove_line()
{
    char ch;
    int Current_line=1;
    FILE *sptr,*dptr;
    sptr = fopen(myfile, "r");
    dptr = fopen(myfilenew, "w");

    ch = getc(sptr);
    if(line_start==0)
        return -1;
    if(line_end==0)
        line_end=line_start;
        
    while (ch != EOF)
    {
        if (ch == '\n')
            Current_line++;

        if ((Current_line < line_start)||(Current_line> line_end))
            putc(ch, dptr);
        
        ch = getc(sptr);
    }
    fclose(sptr);
    fclose(dptr);
    remove(myfile);
    rename(myfilenew,myfile);
    return 0;
}

int modify(char *line_to_add)         
{
    char ch,flg=0;
    int Current_line=1;
    FILE *sptr,*dptr;
    sptr = fopen(myfile, "r");
    dptr = fopen(myfilenew, "w");

    ch = getc(sptr);
    if(line_start==0)
        return -1;
        
    while (ch != EOF)
    {
        if (ch == '\n')
            Current_line++;

        if (Current_line != line_start)
            putc(ch, dptr);
        else if(Current_line==line_start)
        {
            if(flg==0)
                fprintf(dptr,"\n%s",line_to_add);
            flg=1;
        }            
        ch = getc(sptr);
    }
    fclose(sptr);
    fclose(dptr);
    remove(myfile);
    rename(myfilenew,myfile);
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
    if(CURRENT_RUN==ADD)
        add(argc, argv);
    else if(CURRENT_RUN==PASTE)
        paste();
    else if(CURRENT_RUN==REMOVE)
        remove_line();
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
    for (i = 1; i < argc; i++)
    {
        strcpy(choice, argv[i]);
        if (choice[0] != '+')
            break;
        else if (choice[1] == '+')
        {
            n++;
            arg2(choice,argv);
        }
        else
        {
            n++;
            for (j = 1; choice[j] != '\0'; j++)
                arg1(choice[j]); 
        }
    }
    execute(argc,argv);
    return 0;
}
