#include "headers.h"
void peek(const char *dir, const char *flags)
{
    struct dirent *entry, **namelist;
    int no_entries, num_files = 0;
    char full_path[1024], *filenames[1024];
    DIR *directory;
    int sh = 0, details = 0;
    char finad[1024];
    if (flags != NULL)
    {
        if ((strcmp(flags, "-a") == 0))
        {
            sh = 1;
        }
        else if ((strcmp(flags, "-l") == 0))
        {
            details = 1;
        }
        else if ((strcmp(flags, "-a-l") == 0) || (strcmp(flags, "-l-a") == 0) || (strcmp(flags, "-la") == 0) || (strcmp(flags, "-al") == 0))
        {
            sh = 1;
            details = 1;
        }
    }
    if (dir == NULL)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        strcpy(finad, cwd);
        no_entries = scandir(cwd, &namelist, NULL, alphasort);
    }
    else
    {
        if (strcmp(dir, "~") == 0)
        {
            char *filenames[1024];
            int num_files = 0;
            strcpy(finad, initiald);
            no_entries = scandir(initiald, &namelist, NULL, alphasort);
        }
        if (strcmp(dir, "-") == 0)
        {
            strcpy(finad, prevd);
            no_entries = scandir(prevd, &namelist, NULL, alphasort);
        }
        else if (strcmp(dir, ".") == 0)
        {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            strcpy(finad, cwd);
            no_entries = scandir(cwd, &namelist, NULL, alphasort);
        }
        else if (strcmp(dir, "..") == 0)
        {

            char new_directory[1024];
            strcpy(new_directory, getcwd(NULL, 0));
            char *last_slash = strrchr(new_directory, '/');
            if (last_slash != NULL)
            {
                if (last_slash == new_directory)
                {
                    new_directory[1] = '\0';
                }
                else
                {
                    *last_slash = '\0';
                }
            }
            strcpy(finad, new_directory);
            no_entries = scandir(new_directory, &namelist, NULL, alphasort);
        }
        else if (dir[0] == '~')
        {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s%s", initiald, dir + 1);
            strcpy(finad, full_path);
            no_entries = scandir(full_path, &namelist, NULL, alphasort);
        }
        else if (dir[0] == '/')
        {
            strcpy(finad, dir);
            no_entries = scandir(dir, &namelist, NULL, alphasort);
        }
        else
        {
            char full_path[1025];
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            snprintf(full_path, sizeof(full_path), "%s/%s", cwd, dir);
            strcpy(finad, full_path);
            printf("%s\n", finad);
            no_entries = scandir(full_path, &namelist, NULL, alphasort);
        }
    }
    if (no_entries < 0)
    {
        perror("scandir");
        return;
    }
    if (!(details))
    {
        for (int i = 0; i < no_entries; i++)
        {
            if (!sh && namelist[i]->d_name[0] == '.')
            {
                free(namelist[i]);
                continue;
            }
            struct dirent *entry = namelist[i];

            if (entry->d_type == DT_DIR)
            {
                printf("\033[34m%s\033[0m\n", entry->d_name); // Blue color for directories
            }
            else if (access(entry->d_name, X_OK) == 0)
            {
                printf("\033[32m%s\033[0m\n", entry->d_name); // Green color for executables
            }
            else
            {
                printf("\033[0m%s\033[0m\n", entry->d_name); // White color for regular files
            }

            free(namelist[i]);
        }
        free(namelist);
    }
    else
    {
        func(finad, sh);
    }
}
void spec4(char *input)
{
    char *arguments = input + 4;
    char *token = strtok(arguments, " ");
    if (token == NULL)
    {
        peek(NULL, NULL);
    }
    else
    {
        char *tokens[1024];
        int num_tokens = 0;
        while (token != NULL)
        {
            tokens[num_tokens++] = token;
            token = strtok(NULL, " ");
        }
        if (num_tokens == 1)
        {
            if (strncmp(tokens[0], "-a", 2) == 0 || strncmp(tokens[0], "-l", 2) == 0)
            {
                if (strlen(tokens[0]) > 3 || strcmp(tokens[0], "-aa") == 0 || strcmp(tokens[0], "-ll") == 0)
                {
                    // printf("%s\n",tokens[0]);
                    printf("Invalid Flag\n");
                }
                else
                {
                    peek(NULL, tokens[0]);
                }
            }
            else
            {
                peek(tokens[0], NULL);
            }
        }
        else if (num_tokens == 2)
        {
            if ((strncmp(tokens[1], "-a", 2) == 0 || strncmp(tokens[1], "-l", 2) == 0))
            {
                if (strlen(tokens[0]) == 2 && strlen(tokens[1])== 2 && strcmp(tokens[0],tokens[1])!=0)
                {
                    strcat(tokens[0], tokens[1]);
                    // printf("%s\n",tokens[0]);
                    peek(NULL, tokens[0]);
                }
                else
                {
                    printf("Invalid Flags\n");
                }
            }
            else
            {
                peek(tokens[1], tokens[0]);
            }
        }
        else
        {
            if (strlen(tokens[0]) == 2 && strlen(tokens[1]) == 2)
            {
                strcat(tokens[0], tokens[1]);
                peek(tokens[2], tokens[0]);
            }
            else
            {
                printf("Invalid Flags\n");
            }
        }
    }
}