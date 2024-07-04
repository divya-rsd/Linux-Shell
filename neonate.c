#include "headers.h"

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int getMostRecentProcessPID()
{
    FILE *loadavg_file = fopen("/proc/loadavg", "r");
    if (loadavg_file == NULL)
        die("fopen");

    float avg1, avg5, avg15;
    int runnable, total, last_pid;
    fscanf(loadavg_file, "%f %f %f %d/%d %d", &avg1, &avg5, &avg15, &runnable, &total, &last_pid);

    fclose(loadavg_file);
    return last_pid;
}

void spec15(int pause)
{
    int time_arg = pause;
    if (time_arg <= 0)
    {
        fprintf(stderr, "Invalid time argument. Please provide a positive integer.\n");
        exit(1);
    }

    pid_t pid = getMostRecentProcessPID();
    if (pid == 0)
        die("No processes found.");

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        die("pipe");
    }

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        die("fork");
    }
    else if (child_pid == 0)
    {
        close(pipe_fd[0]);
        while (1)
        {
            pid = getMostRecentProcessPID();
            printf("%d\n", pid);
            sleep(time_arg);
        }
    }
    else
    {
        close(pipe_fd[1]);
        char c;
        enableRawMode();
        // printf("Press 'x' to exit...\n");
        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (c == 'x')
            {
                kill(child_pid, SIGTERM);
                break;
            }
        }
        disableRawMode();
        close(pipe_fd[0]);
    }
}
void handle(char *input)
{
    int time_arg;
    if (sscanf(input + 11, "%d", &time_arg) == 1)
    {
        if (time_arg <= 0)
        {
            printf("Invalid Time\n");
        }
        else
        {
            spec15(time_arg);
        }
    }
    else
    {
        fprintf(stderr, "Usage: neonate -n <time_arg>\n");
    }
}