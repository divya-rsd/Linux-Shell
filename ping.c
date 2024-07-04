#include "headers.h"
void specping(char *input)
{
    char *arguments = input + 4;
    char *toki = strtok(arguments, " ");
    char *argst[4];
    int num_to = 0;
    while (toki != NULL)
    {
        argst[num_to++] = toki;
        toki = strtok(NULL, " ");
    }

    // printf("%s", argst[0]);
    if (num_to != 2)
    {
        printf("Error:Missing Arguments\n");
    }   
    else
    {
        int pidof = atoi(argst[0]);
        int sig = atoi(argst[1]);
        sig = sig % 32;
        if (sig == 19)
        {
            move_to_stopfromrun(pidof);
        }
        if (sig == 18)
        {
            move_to_runfromstop(pidof);
        }
        kill(pidof, sig);
    }
}