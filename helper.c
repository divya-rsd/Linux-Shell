#include "headers.h"
void actshelp(char *input_redirect, char *output_redirect, char *append_redirect)
{
    int original_stdout = dup(STDOUT_FILENO);
    int original_stdin = dup(STDIN_FILENO);
    if (input_redirect)
    {
        *input_redirect = '\0';
        char *filename = strtok(input_redirect + 1, " ");
        FILE *file = fopen(filename, "r");

        if (file)
        {
            // Redirect stdin to the file.
            dup2(fileno(file), STDIN_FILENO);
            fclose(file);
        }
        else
        {
            perror("Failed to open file");
        }
    }
    if (output_redirect)
    {
        if (append_redirect)
        {
            *append_redirect = '\0';
            char *filename = strtok(append_redirect + 2, " ");

            FILE *file = fopen(filename, "a");
            if (file)
            {
                dup2(fileno(file), STDOUT_FILENO);
                fclose(file);
            }
            else
            {
                perror("Failed to open file");
            }
        }
        else
        {
            *output_redirect = '\0';
            char *filename = strtok(output_redirect + 1, " ");

            FILE *file = fopen(filename, "w");
            if (file)
            {
                dup2(fileno(file), STDOUT_FILENO);
                fclose(file);
            }
            else
            {
                perror("Failed to open file");
            }
        }
    }
    list_background_processes();
    dup2(original_stdin, STDIN_FILENO);
    dup2(original_stdout, STDOUT_FILENO);
    close(original_stdout);
    close(original_stdin);
}

void removeSubstring(char *str, const char *sub)
{
    char *pos;
    int len = strlen(sub);
     printf("x");
    while ((pos = strstr(str, sub)) != NULL)
    {
        memmove(pos, pos + len, strlen(pos + len) + 1);
    }
}