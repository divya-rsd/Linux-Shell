#include "headers.h"
void proclore(const char *arg)
{
    int pid;

    if (arg == NULL)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(arg);
    }

    char proc_path[256];
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/status", pid);

    FILE *fp = fopen(proc_path, "r");
    if (fp == NULL)
    {
        perror("Error opening process status file");
        return ;
    }

    char line[256];
    char status[3] = "";
    int pgrp = 0;
    int vsize = 0;
    char exe_path[256] = "";

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "State: %2s", status) == 1)
        {
            if (strchr(status, '+'))
            {
                status[1] = '+';
            }
        }
        else if (sscanf(line, "PPid: %*d Pid: %*d PGid: %d", &pgrp) == 1)
        {
        }
        else if (sscanf(line, "VmSize: %d", &vsize) == 1)
        {
            vsize /= 1024;
        }
        else if (sscanf(line, "Name: %255s", exe_path) == 1)
        {
        }
    }

    fclose(fp);

    printf("pid : %d\n", pid);
    printf("process status : %s\n", status);
    printf("Process Group : %d\n", pgrp);
    printf("Virtual memory : %d\n", vsize);
    printf("executable path : %s\n", exe_path);
}

void spec7(char *input)
{
    char *arguments = input + 8;
    char *token = strtok(arguments, " ");
    if (token == NULL)
    {
        proclore(NULL);
    }
    else
    {
        while (token != NULL)
        {
            proclore(token);
            token = strtok(NULL, " ");
        }
    }
}