#include "headers.h"
char prprinter[1024];
int firstp = 0;
int firstd = 0;
int nofound = 0;
char store[1024];
char stord[1024];
char path[2048];
int num_matching_files = 0, num_matching_dirs = 0;
void eflagset(const char *flags, const char *target_name, const char *target_dir)
{
    DIR *dir;
    struct dirent *entry;
    char finad[1024];
    int dflag = 0;
    int fflag = 0;
    int eflag = 0;
    if (flags != NULL)
    {
        if (strcmp(flags, "-e") == 0)
            eflag = 1;
        else if (strcmp(flags, "-e-d") == 0 || strcmp(flags, "-d-e") == 0 || strcmp(flags, "-ed") == 0 || strcmp(flags, "-de") == 0)
        {
            eflag = 1;
            dflag = 1;
        }
        else if (strcmp(flags, "-e-f") == 0 || strcmp(flags, "-f-e") == 0 || strcmp(flags, "-ef") == 0 || strcmp(flags, "-fe") == 0)
        {
            eflag = 1;
            fflag = 1;
        }
    }
    if (strcmp(target_dir, "~") == 0)
    {
        strcpy(finad, initiald);
    }
    else if (strcmp(target_dir, "-") == 0)
    {
        strcpy(finad, prevd);
    }
    else if (strcmp(target_dir, ".") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        strcpy(finad, cwd);
    }
    else if (strcmp(target_dir, "..") == 0)
    {
        char new_directory[1024];
        strcpy(new_directory, getcwd(NULL, 0));
        char *last_slash = strrchr(new_directory, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
        strcpy(finad, new_directory);
    }
    else if (target_dir[0] == '~')
    {
        char full_path[1025];
        snprintf(full_path, sizeof(full_path), "%s%s", initiald, target_dir + 1);
        // printf("asd%s\n",full_path);
        strcpy(finad, full_path);
    }
    else if (target_dir[0] == '/')
    {
        strcpy(finad, target_dir);
    }
    else if (target_dir[0] == '.')
    {
        char full_path[1025];
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        snprintf(full_path, sizeof(full_path), "%s%s", cwd, target_dir + 1);
        strcpy(finad, full_path);
    }
    // Open the target directory
    if (strlen(prprinter) == 0)
    {
        strcpy(prprinter, finad);
    }
    dir = opendir(finad);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcasecmp(entry->d_name, ".") == 0 || strcasecmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        char path[2048];
        snprintf(path, sizeof(path), "%s/%s", finad, entry->d_name);
        struct stat entry_stat;
        if (stat(path, &entry_stat) != 0)
        {
            perror("Error getting file/directory info");
            continue;
        }
        if (S_ISDIR(entry_stat.st_mode))
        {
            if (strcasecmp(entry->d_name, target_name) == 0 && !fflag)
            {
                num_matching_dirs++; 
                if (firstd == 0)
                {
                    strcpy(stord, path);
                    firstd = 1;
                }
            }
            eflagset(flags, target_name, path); 
        }
        else
        {
            char *dot = strrchr(entry->d_name, '.');
            if (dot != NULL)
            {
                *dot = '\0'; 
            }
            if (strcasecmp(entry->d_name, target_name) == 0 && !dflag)
            {
                num_matching_files++;
                if (firstp == 0)
                {
                    strcpy(store, path);
                }
              
            }
        } 
    }
}
void eexist(const char *flags, const char *target_name, const char *target_dir)
{
    int dflag = 0;
    int fflag = 0;
    int eflag = 0;
    if (flags != NULL)
    {
        if (strcmp(flags, "-e") == 0)
            eflag = 1;
        else if (strcmp(flags, "-e-d") == 0 || strcmp(flags, "-d-e") == 0 || strcmp(flags, "-ed") == 0 || strcmp(flags, "-de") == 0)
        {
            eflag = 1;
            dflag = 1;
        }
        else if (strcmp(flags, "-e-f") == 0 || strcmp(flags, "-f-e") == 0 || strcmp(flags, "-ef") == 0 || strcmp(flags, "-fe") == 0)
        {
            eflag = 1;
            fflag = 1;
        }
    }
    if (fflag == 1 && num_matching_files == 1)
    {
        printf("%s\n", store + strlen(prprinter));
        FILE *file;
        char filename[1024];
        strcpy(filename, store); 
        file = fopen(filename, "r");
        if (file == NULL)
        {
            printf("Error opening the file.\n");
            return;
        }
        char line[1024];
        while (fgets(line, sizeof(line), file))
        {
            printf("%s", line);
        }
        fclose(file);
    }
    else if (dflag == 1 && num_matching_dirs == 1)
    {
        warp(stord);
    }
    else if (num_matching_dirs == 0 && dflag == 1)
    {
        printf("No matches\n");
    }
    else if (num_matching_dirs == 1 && num_matching_files == 0)
    {
        warp(stord);
    }
    else if (num_matching_files == 1 && num_matching_dirs == 0)
    {
        printf("%s\n", store + strlen(prprinter));
    }
    else if (fflag == 1 && num_matching_files == 0)
    {
        printf("No Matches\n");
    }
    else
    {
        printf("No action\n");
    }
}
void seek(const char *flags, const char *target_name, const char *target_dir)
{
    DIR *dir;
    struct dirent *entry;
    char finad[1024];
    int dflag = 0;
    int fflag = 0;
    int eflag = 0;
    if (flags != NULL)
    {
        if (strcmp(flags, "-f") == 0)
            fflag = 1;
        else if (strcmp(flags, "-d") == 0)
            dflag = 1;
        else if (strcmp(flags, "-e") == 0)
            eflag = 1;
        else if (strcmp(flags, "-e-d") == 0 || strcmp(flags, "-d-e") == 0 || strcmp(flags, "-ed") == 0 || strcmp(flags, "-de") == 0)
        {
            eflag = 1;
            dflag = 1;
        }
        else if (strcmp(flags, "-e-f") == 0 || strcmp(flags, "-f-e") == 0 || strcmp(flags, "-ef") == 0 || strcmp(flags, "-fe") == 0)
        {
            eflag = 1;
            fflag = 1;
        }
        else if (strcmp(flags, "-f-d") == 0 || strcmp(flags, "-d-f") == 0 || strcmp(flags, "-fd") == 0 || strcmp(flags, "-df") == 0)
        {
            printf("Error: Invalid Flags\n");
            return;
        }
    }
    if (strcmp(target_dir, "~") == 0)
    {
        strcpy(finad, initiald);
    }
    else if (strcmp(target_dir, "-") == 0)
    {
        strcpy(finad, prevd);
    }
    else if (strcmp(target_dir, ".") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        strcpy(finad, cwd);
    }
    else if (strcmp(target_dir, "..") == 0)
    {
        char new_directory[1024];
        strcpy(new_directory, getcwd(NULL, 0));
        char *last_slash = strrchr(new_directory, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
        }
        strcpy(finad, new_directory);
    }
    else if (target_dir[0] == '~')
    {
        char full_path[1025];
        snprintf(full_path, sizeof(full_path), "%s%s", initiald, target_dir + 1);
        strcpy(finad, full_path);
    }
    else if (target_dir[0] == '/')
    {
        strcpy(finad, target_dir);
    }
    else if (target_dir[0] == '.')
    {
        char full_path[1025];
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        snprintf(full_path, sizeof(full_path), "%s%s", cwd, target_dir + 1);
        strcpy(finad, full_path);
    }
    if (strlen(prprinter) == 0)
    {
        strcpy(prprinter, finad);
    }
    dir = opendir(finad);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }
    if (eflag == 0)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcasecmp(entry->d_name, ".") == 0 || strcasecmp(entry->d_name, "..") == 0)
            {
                continue; // Skip "." and ".."
            }

            char path[2048];
            snprintf(path, sizeof(path), "%s/%s", finad, entry->d_name);
            struct stat entry_stat;
            if (stat(path, &entry_stat) != 0)
            {
               // perror("Error getting file/directory info");
                continue;
            }
            if (S_ISDIR(entry_stat.st_mode))
            {
                if (strcasecmp(entry->d_name, target_name) == 0 && !fflag)
                {
                    nofound++;                                        
                    printf("\033[34m%s/\n", path + strlen(prprinter)); 
                }
                seek(flags, target_name, path); 
            }
            else
            { 
                char *dot = strrchr(entry->d_name, '.');
                if (dot != NULL)
                {
                    *dot = '\0'; 
                }
                if (strcasecmp(entry->d_name, target_name) == 0 && !dflag)
                {
                    nofound++;
                    printf("\033[32m%s\n", path + strlen(prprinter));
                    //  printf("f%s\n", finad);
                }
            }
        }
        closedir(dir);
    }

    return;
}
void spec8(char *input)
{
    char *arguments = input + 4;
    char *token = strtok(arguments, " ");
    nofound = 0;
    num_matching_files = 0, num_matching_dirs = 0;
    if (token == NULL)
    {
        printf("Error: provide the target to search\n");
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
            seek(NULL, tokens[0], ".");
            if (nofound == 0)
            {
                printf("No files found!\n");
            }
        }
        if (num_tokens == 2)
        {
            if (strncmp(tokens[0], "-e", 2) == 0 || strncmp(tokens[0], "-f", 2) == 0 || strncmp(tokens[0], "-d", 2) == 0)
            {
                if (strlen(tokens[0]) > 3 || strcmp(tokens[0], "-fd") == 0 || strcmp(tokens[0], "-df") == 0)
                {
                    printf("Error: Invalid Flags\n");
                }
                else if (strcmp(tokens[0], "-e") == 0 || strcmp(tokens[0], "-ef") == 0 || strcmp(tokens[0], "-ed") == 0 || strcmp(tokens[0], "-fe") == 0 || strcmp(tokens[0], "-ed") == 0)
                {
                    eflagset(tokens[0], tokens[1], ".");
                    if (num_matching_dirs == 0 && num_matching_files == 0)
                    {
                        printf("No files found!\n");
                    }
                    else if (num_matching_dirs + num_matching_files == 1)
                        eexist(tokens[0], tokens[1], ".");
                    else
                    {
                        printf("No action\n");
                    }
                }
                else
                {
                    seek(tokens[0], tokens[1], ".");
                    if (strcmp(tokens[0], "-e") != 0 && nofound == 0)
                    {
                        printf("No files found!\n");
                    }
                }
            }
            else
            {
                seek(NULL, tokens[0], tokens[1]);
                if (nofound == 0)
                {
                    printf("No files found\n");
                }
            }
        }
        else if (num_tokens == 3)
        {
            if ((strcmp(tokens[2], "-d") == 0 || strcmp(tokens[2], "-e") == 0 || strcmp(tokens[2], "-f") == 0))
            {
                printf("Error : Missing target\n");
            }
            else if ((strncmp(tokens[1], "-d", 2) == 0 || strncmp(tokens[1], "-e", 2) == 0 || strncmp(tokens[1], "-f", 2) == 0))
            {
                if (strlen(tokens[0]) > 2 || strlen(tokens[1]) > 2 || strcmp(tokens[0], tokens[1]) == 0)
                {
                    printf("Invalid Flags\n");
                }
                else if (strcmp(tokens[1], "-e") == 0 || strcmp(tokens[0], "-e") == 0)
                {
                    strcat(tokens[0], tokens[1]);
                    // printf("%s",tokens[0]);
                    eflagset(tokens[0], tokens[2], ".");
                    if (num_matching_dirs == 0 && num_matching_files == 0)
                    {
                        printf("No files found!\n");
                    }
                    else if (num_matching_dirs <= 1 && num_matching_files <= 1)
                    {
                        // printf("%s", tokens[0]);
                        eexist(tokens[0], tokens[2], ".");
                    }
                    else
                    {
                        printf("No action\n");
                    }
                }
                else
                {
                    strcat(tokens[0], tokens[1]);
                    seek(tokens[0], tokens[2], ".");
                }
            }
            else
            {
                if(strlen(tokens[0])>3)
                {
                    printf("Invalid Flags\n");
                }
                else if (strcmp(tokens[0], "-e") == 0 || strcmp(tokens[0], "-fe") == 0 || strcmp(tokens[0], "-ef") == 0 || strcmp(tokens[0], "-ed") == 0 || strcmp(tokens[0], "-de") == 0)
                {
                    eflagset(tokens[0], tokens[1], tokens[2]);
                    if (num_matching_dirs == 0 && num_matching_files == 0)
                    {
                        printf("No files found\n");
                    }
                    else if (num_matching_dirs == 1 || num_matching_files == 1)
                    {
                        //printf("A");
                        eexist(tokens[0], tokens[1], tokens[2]);
                    }
                    else
                    {
                        printf("No action\n");
                    }
                }
                else
                {
                    seek(tokens[0], tokens[1], tokens[2]);
                    if (nofound == 0)
                    {
                        printf("No files found\n");
                    }
                }
            }
        }
        else if (num_tokens == 4)
        {
            if (strcmp(tokens[1], "-e") == 0 || strcmp(tokens[0], "-e") == 0)
            {
                strcat(tokens[0], tokens[1]);
                // printf("%s\n",tokens[2]);
                eflagset(tokens[0], tokens[2], tokens[3]);
                if (num_matching_dirs == 0 && num_matching_files == 0)
                {
                    printf("No files found!\n");
                }
                else if (num_matching_dirs + num_matching_files < 2)
                {
                    eexist(tokens[0], tokens[2], tokens[3]);
                }
                else
                {
                    printf("No action");
                }
            }
            else
            {
                printf("invalid flags\n");
            }
        }
        else if (num_tokens >= 5)
        {
            printf("Invalid flags!\n");
            return;
        }
    }
}