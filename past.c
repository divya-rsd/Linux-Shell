#include "headers.h"
void load_history(const char *history[], int *count)
{
    FILE *file = fopen(filepath_past, "r");
    if (file != NULL)
    {
        char line[4096];
        while (*count < MAX_HISTORY_SIZE && fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';
            history[*count] = strdup(line);
            (*count)++;
        }
        fclose(file);
    }
}
void copy_command(char *inputstring, int l, const char *history[], int *count)
{
    FILE *file = fopen(filepath_past, "r");
    if (file != NULL)
    {
        char line[4096];
        int current_line=1;
        while (fgets(line, sizeof(line), file))
        {
            
          if(current_line==l)
          {
            strcpy(inputstring,line);
            break;
          }
          current_line++;
        }
        fclose(file);
    }
}
int is_same_as_last(const char *command, const char *history[], int count)
{
    if (count > 0)
    {
        return strcmp(command, history[count - 1]) == 0;
    }
    return 0;
}
void save_history(const char *history[], int count)
{
    FILE *file = fopen(filepath_past, "w");
    if (file == NULL)
    {
        perror("Error opening history file");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s\n", history[i]);
    }

    fclose(file);
}

void add_to_history(const char *command, const char *history[], int *count)
{
    if (!is_same_as_last(command, history, *count))
    {
        // Add new command to history
        history[*count] = strdup(command);
        (*count)++;

        // Remove oldest command if history exceeds MAX_HISTORY_SIZE
        if (*count > MAX_HISTORY_SIZE)
        {
            free((void *)history[0]); // Free memory for the oldest command
            for (int i = 1; i < *count; i++)
            {
                history[i - 1] = history[i];
            }
            (*count)--;
        }

        save_history(history, *count);
    }
}
void clear_history(const char *history[], int *count)
{
    for (int i = 0; i < *count; i++)
    {
        free((void *)history[i]);
    }
    *count = 0;
    save_history(history, *count);
}

void print_history(const char *history[], int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("%s\n", history[i]);
    }
}
