#ifndef _past_H
#define _past_H
#define MAX_HISTORY_SIZE 15
char filepath_past[1024];
void print_history(const char *history[], int count);
void add_to_history(const char *command, const char *history[], int *count);
void save_history(const char *history[], int count);
void load_history(const char *history[], int *count);
int is_same_as_last(const char *command, const char *history[], int count);
void clear_history(const char *history[], int *count);
void copy_command(char *inputstring, int l, const char *history[], int *count);
#endif