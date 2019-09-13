#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *str_replace(char *orig, char *rep, char *with) 
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

int cwd_path(char *cwd)
{
    if (getcwd(cwd, PATH_MAX) != NULL)
        return 0;
    else
    {
        perror("getcwd() error");
        return -1;
    }
}

int exists(const char *file)
{
    FILE *fptr;
    if ((fptr = fopen(file, "r")))
    {
        fclose(fptr);
        return 1;
    }
    return 0;
}

char * absolute_path(char * file_path)
{
    char *path=malloc(sizeof(char)*PATH_MAX)
    if(file_path[0]=='\\')
        return file_path;

    else if(file_path[0]=='.')
    {
        if(file_path[1]=='.'&&file_path[2]=='/')
        {
            path()
        }
        else if(file_path[1]=='/')
        {
            /* code */
        }
        
    }

}