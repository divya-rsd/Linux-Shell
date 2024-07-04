#include "headers.h"
void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int compare_filenames(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

int func(const char *dir_path, int sh)
{
    struct dirent **entries;
    int num_entries = scandir(dir_path, &entries, NULL, compare_filenames);

    if (num_entries == -1)
    {
        perror("scandir");
        return 1;
    }

    for (int i = 0; i < num_entries; i++)
    {
        if (!sh && entries[i]->d_name[0] == '.')
        {
            free(entries[i]);
            continue;
        }
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entries[i]->d_name);

        struct stat file_info;
        if (stat(full_path, &file_info) == -1)
        {
            perror("stat");
            continue;
        }

        struct passwd *user_info = getpwuid(file_info.st_uid);
        struct group *group_info = getgrgid(file_info.st_gid);

        char time_str[80];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_info.st_mtime));
        print_permissions(file_info.st_mode);
        printf(" %2hu ", file_info.st_nlink);//
        printf(" %s ", user_info->pw_name);
        printf("%s ", group_info->gr_name);
        printf("%8lld ", file_info.st_size);
        printf(" %s ", time_str);
       // printf("%s\n", entries[i]->d_name);
        if (entries[i]->d_type==DT_DIR)
        {
            printf("\033[34m%s\033[0m\n", entries[i]->d_name); // Blue 
        }
        else if (access(entries[i]->d_name, X_OK) == 0)
        {
            printf("\033[32m%s\033[0m\n", entries[i]->d_name); // Green 
        }
        else
        {
            printf("\033[0m%s\033[0m\n", entries[i]->d_name); // White 
        }
        free(entries[i]); // Free each dirent structure
    }

    free(entries);
    return 0;
}