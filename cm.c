#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>


const char str[]="./myfile.txt"



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

int copy_cut(int argc, char *argv[],int n)
{
    FILE *fptr;
    char dir_path[PATH_MAX+4];
    fptr=fopen(str,"a+");
    if(fptr == NULL)
    {
      perror("Error!");
      exit(1); 
    }
    path(dir_path);
    fprintf(fptr," cd %s && ",dir_path);
    for(;n<argc;n++)
        fprintf(fptr,"%s ",argv[n]);
    fprintf(fptr,"%s","\n");
    fclose(fptr);
    printf("\nFile Closed\n");
    return 1;
}

int paste(int argc, char *argv[],int n)
{
    FILE *fptr;
    char dir_path[PATH_MAX]="cd ";
    char arr[5000]={'\0'};
    fptr=fopen(str,"r+");
    if(fptr == NULL)
    {
      printf("Error!");
      exit(1);
    }
    path(dir_path+3);
    while(getc(fptr)!=EOF)
    {
        fscanf(fptr,"%[^\n]s",arr);
        strcat(arr,dir_path+2);
	system(arr);
	printf("%s\n",arr);
    }
    system(dir_path);
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

int main(int argc,char *argv[])
{
    int ReturnVal=-1,cn=1;
    if(argc>1)
    {
        char choice[10];
        if(strcmp(argv[1],"-shoot")==0)
        {
            shoot=1;
            cn=2;
        }
        strcpy(choice,argv[cn]);
        if(strcmp(choice,"cp")==0||strcmp(choice,"mv")==0)
            ReturnVal=copy_cut(argc,argv,cn);
        else if(strcmp(choice,"pas")==0)
            ReturnVal=paste(argc,argv,cn);
	else if(strcmp(choice,"clr")==0)
	    ReturnVal=clear();
        printf("Return Value = %d",ReturnVal);
	return 0;
    }
}

