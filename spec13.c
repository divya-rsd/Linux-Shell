#include "headers.h"
void sigint_handler(int signum)
{
    interrupt_flag = 1;
    if (getpid() == getpgid(getpid()))
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
}

void sigtstp_handler(int signum)
{
    if (signum == SIGTSTP)
    {
        if (current_foreground_pid > 0)
        {
            kill(current_foreground_pid, SIGSTOP);
            tcsetpgrp(STDIN_FILENO, getpgid(0));
        }
    }
}
void cleanup()
{
    if (current_foreground_pid > 0)
    {
        kill(current_foreground_pid, SIGTERM);
    }

    cleanbgprocess(); // clean the stored bg processes
    printf("Exiting shell\n");
    exit(0);
}
