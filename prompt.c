#include "headers.h"
char initiald[1024];
void prompt()
{
    // Do not hardcode the prmopt
    char *username = getlogin();
    char cwd[1024];
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    getcwd(cwd, sizeof(cwd));
    if (strlen(initiald) == 0)
    {
        strcpy(initiald, cwd);
    }
    if (strncmp(cwd, initiald, sizeof(initiald)) == 0)
    {
        if (set == 0)
            printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m~%s\033[37m>", username, hostname, cwd + strlen(initiald));
        else
        {
            strcat(cwd + strlen(initiald), " ");
            printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m~%s\033[37m>", username, hostname, strcat(cwd + strlen(initiald), finalprint));
            set = 0; 
        }
    }
    else
    {
        int n1 = strlen(cwd);
        int n2 = strlen(initiald);
        char arr[1024];
        if (n1 > n2)   // means inside the home directory
        {
            int i;
            int j = n2;
            for (i = 0; cwd[j] != '\0'; i++, j++)
            {
                arr[i] = cwd[j];
            }
            arr[i] = '\0';
            if (set == 0)
                printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m~%s\033[37m>", username, hostname, arr);
            else
            {
                strcat(arr, " ");
                printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m~%s\033[37m>", username, hostname, strcat(arr, finalprint));
                set = 0;
            }
        }
        else
        {
            if (set == 0)
                printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m%s\033[37m>", username, hostname, cwd);
            else
            {
                strcat(cwd, " ");
                printf("<\033[32m%s@\033[32m%s\033[37m:\033[34m%s\033[37m>", username, hostname, strcat(cwd, finalprint));
                set = 0;
            }
        }
    }
}
