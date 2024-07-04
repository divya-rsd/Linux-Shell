#ifndef __sp6_H
#define __sp6_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
struct BackgroundProcess {
    pid_t pid;
    char command[4096];
    char st[20];
};
char * fg(int pid);
char *bgp(int pid);
void cleanbgprocess();
void spec6(char *input,int background);
void check_completed_background_processes();
void list_background_processes();
void quicksort(struct BackgroundProcess arr[], int low, int high);
int partition(struct BackgroundProcess arr[], int low, int high);
void move_to_stopfromrun(int pid);
void move_to_runfromstop(int pid);
#endif