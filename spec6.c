#include "headers.h"
struct BackgroundProcess background_processes[100];
int num_background_processes = 0;
int set = 0;
char finalprint[1024];
void quicksort(struct BackgroundProcess arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}
void move_to_stopfromrun(int pid)
{
    for (int i = 0; i < num_background_processes; i++)
    {
        if (background_processes[i].pid == pid && strcmp(background_processes[i].st, "running") == 0)
        {
            // Check if the process is either "running" or "stopped"
            strcpy(background_processes[i].st, "stopped");
            return;
        }
    }
}
void move_to_runfromstop(int pid)
{
    for (int i = 0; i < num_background_processes; i++)
    {
        if (background_processes[i].pid == pid && strcmp(background_processes[i].st, "stopped") == 0)
        {
            // Check if the process is either "running" or "stopped"
            strcpy(background_processes[i].st, "running");
            return;
        }
    }
}
void cleanbgprocess()
{
    for (int i = 0; i < num_background_processes; i++)
    {
        if (strcpy(background_processes[i].st, "completed") != 0)
        {
            kill(background_processes[i].pid, SIGTERM);
        }
    }
}
char *fg(int pid)
{
    for (int i = 0; i < num_background_processes; i++)
    {
        if (background_processes[i].pid == pid)
        {
            // Check if the process is either "running" or "stopped"
            if (strcmp(background_processes[i].st, "running") == 0 || strcmp(background_processes[i].st, "stopped") == 0)
            {
                // Restore control of the terminal to the shell
                char *command = background_processes[i].command;
                background_processes[i] = background_processes[num_background_processes - 1];
                num_background_processes--;
                return command;
            }
            else
            {
                printf("Process with PID %d is not in a runnable state (it is %s).\n", pid, background_processes[i].st);
                return NULL;
            }
        }
    }
    return NULL;
}
char *bgp(int pid)
{
    for (int i = 0; i < num_background_processes; i++)
    {
        if (background_processes[i].pid == pid)
        {
            // Check if the process is either "running" or "stopped"
            if (strcmp(background_processes[i].st, "stopped") == 0)
            {
                // Restore control of the terminal to the shell
                char *command = background_processes[i].command;
                return command;
            }
            else
            {
                printf("Process with PID %d is not in a runnable state (it is %s).\n", pid, background_processes[i].st);
                return NULL;
            }
        }
    }
    return NULL;
}
int partition(struct BackgroundProcess arr[], int low, int high)
{
    int pivot = arr[high].pid;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].pid < pivot)
        {
            i++;
            struct BackgroundProcess temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    struct BackgroundProcess temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

void list_background_processes()
{
    quicksort(background_processes, 0, num_background_processes - 1);

    // printf("Running or Stopped Background Processes (Sorted by PID):\n");
    for (int i = 0; i < num_background_processes; i++)
    {
        if (strcmp(background_processes[i].st, "running") == 0 || strcmp(background_processes[i].st, "stopped") == 0)
        {
            printf("[%d] : %s - %s\n", background_processes[i].pid, background_processes[i].command, background_processes[i].st);
        }
    }
}

void spec6(char *input, int background)
{
    time_t start_time = time(NULL);
    pid_t child_pid = fork();
    if (child_pid == 0)
    {

        char *args[32];
        char *token = strtok(input, " ");
        int i = 0;
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;
        if (strcmp(args[0], "vim") == 0 && background)
        {
            // Handle 'vim' command
            kill(getpid(), SIGSTOP);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else if (child_pid > 0)
    {
        // Parent process
        if (background)
        {
            struct BackgroundProcess bg_process;
            bg_process.pid = child_pid;
            strncpy(bg_process.command, input, sizeof(bg_process.command));
            bg_process.command[sizeof(bg_process.command) - 1] = '\0';
            if (strncmp(input, "vim", 3) == 0 && background)
            {
                strcpy(bg_process.st, "stopped"); // Set to stopped
            }
            else
            {
                strcpy(bg_process.st, "running"); // Set to running
            }

            background_processes[num_background_processes++] = bg_process;
            printf("[%d] %d\n", num_background_processes, child_pid);
        }
        else
        {
            current_foreground_pid = child_pid;
            // printf("j");
            int status;
            waitpid(child_pid, &status, WUNTRACED);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
            {
                err=1;
            }
            if (WIFSTOPPED(status))
            {
                struct BackgroundProcess bg_process;
                bg_process.pid = child_pid;
                strncpy(bg_process.command, input, sizeof(bg_process.command));
                bg_process.command[sizeof(bg_process.command) - 1] = '\0';
                strcpy(bg_process.st, "stopped");
                background_processes[num_background_processes++] = bg_process;
                tcsetpgrp(STDIN_FILENO, getpgid(0)); // Give control back to the shell
            }
            current_foreground_pid = -1;
            time_t end_time = time(NULL);
            int elapsed_time = (int)(end_time - start_time);
            printf("\r");
            printf("\033[K");
            if (elapsed_time > 2)
            {
                set = 1;
                char caten[1024];
                char num[20];
                char *token = strtok(input, " ");
                sprintf(num, "%d", elapsed_time);
                strcpy(caten, token);
                strcat(caten, " : ");
                strcat(caten, num);
                strcpy(finalprint, caten);
            }
        }
    }
    else
    {
        perror("fork");
    }
}
void check_completed_background_processes()
{
    int status;
    for (int i = 0; i < num_background_processes; i++)
    {
        pid_t result = waitpid(background_processes[i].pid, &status, WNOHANG);
        if (result > 0)
        {
            num_background_processes--;
            printf("Background process '%s' (PID: %d) finished", background_processes[i].command, background_processes[i].pid);
            if (WIFEXITED(status))
            {
                printf(" normally with exit status %d\n", WEXITSTATUS(status));
            }
            else
            {
                printf(" abnormally\n");
            }
            strcpy(background_processes[i].st, "Completed");
        }
    }
}
