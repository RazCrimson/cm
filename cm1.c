#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <unistd.h>
#include "temp.c"

const char str[]="./myfile.txt"

static bool paste_mode=false;
static bool absolute_path=true;
static bool copy_move=false;

int path(char *cwd) {
    if (getcwd(cwd, PATH_MAX) != NULL)
        return 0;
    else
    {
       perror("getcwd() error");
       return -1;
    }
}

int shoot=0;

int arg1(char ch)
{
	if(ch=='a')
		
int add(int argc, char *argv[],int n)
{
	int k=0;
    bool absolute_path=false;
    FILE *fptr;
    char source_path[PATH_MAX+4];
    fptr=fopen(str,"w+");
    fseek(fptr, 0, SEEK_END);
    if(fptr == NULL)
    {
      perror("Error!");
      exit(1);
    }
    for(int i=n;i<argc;i++)
	{    if(argv[i][0]!='\\')
        {
            absolute_path=false;
            break;
        }
    }
	if(absolute_path==false)
	{
        path(source_path);
        fprintf(fptr," cd %s &&",source_path);
	}
	if(paste_mode==true)
		fprintf(fptr,"mv ");
	else
		fprintf(fptr,"cp ");
    for(int i=n;i<argc;i++)
        fprintf(fptr,"%s ",argv[i]);
    fprintf(fptr,"%c",'\n');
    fclose(fptr);
    printf("\nFile Closed\n");
    return 0;
}

int paste(int argc, char *argv[],int n)
{
    FILE *fptr;
    char dest_path[PATH_MAX]="cd ";
    char arr[10000]={'\0'};
    fptr=fopen(str,"r+");
    if(fptr == NULL)
    {
      perror("Error!");
      exit(1);
    }
    path(dest_path+3);
    while(getc(fptr)!=EOF)
    {
        fscanf(fptr,"%[^\n]s",arr);
        strcat(arr,dest_path+2);
        if(paste_mode==true)
            *arr=str_replace(arr, "&&cp", "&&mv")
	    system(arr);
	    printf("%s\n",arr);
    }
    system(dest_path);
    return 2;
}

int clear()
{
    char cmd[50]="rm ";
    strcat(cmd,str);
    strcat(cmd,"; touch ");
    strcat(cmd,str);
    system(cmd);
    printf("\nLog cleared.\n")
    return 0;

}

void help()
{
	printf("++add or +a to add file\n");
	printf("++paste or +p to paste file\n");
	printf("++list or +l to list all the copied files\n");
	printf("++modify or +m to modify the copied files\n");
	printf("++remove or +r to remove the copied files\n");
	printf("++clear or +c to clear contents copied files\n");
}
int main(int argc,char *argv[])
{
    int ReturnVal=-1,n=1,j;
    if(argc<=1)
    {
        printf("\n%s requires at least one argument.\nPlease try %1$s ++help for more information.\n",argv[0]);
        return 0;
    }
    char choice[20];
    for(int i= 1;i<argc;i++)
    {
	strcpy(choice,argv[i]);
	if(choice[0]!='+')
		break;
	else if(choice[1]=='+')
	{
		arg2(choice);
	}
	else
	{
		for(j=1;choice[j]!='\0';j++)
			arg1(choice[j]);
	}
    }
    printf("Return Value = %d",ReturnVal);
    return 0;
}

