#include "headers.h"
char prevd[1024];
void warp(const char *directory)
{
    char new_directory[1024];
    if (strlen(prevd) == 0)
    {
        strcpy(prevd, initiald);
    }
    if (strcmp(directory, "~") == 0)
    {
        strcpy(new_directory, initiald);
    }
    else if (strcmp(directory, "-") == 0)
    {
        strcpy(new_directory, prevd);
    }
    else if (strcmp(directory, ".") == 0)
    {
        strcpy(new_directory, getcwd(NULL, 0));
    }
    else if (strcmp(directory, "..") == 0)
    {
        strcpy(new_directory, getcwd(NULL, 0));
        char *last_slash = strrchr(new_directory, '/');
        if (last_slash != NULL)
        {
            if (last_slash == new_directory)
            {
                // Special case: navigating to root directory
                new_directory[1] = '\0'; 
            }
            else
            {
                *last_slash = '\0'; 
            }
        }
    }
    else if (directory[0] == '~')
    {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s%s", initiald, directory + 1);
        strcpy(new_directory, full_path);
    }
    else
    {
        strcpy(new_directory, directory);
    }
    char storeero[1024];
    strcpy(storeero,prevd);
    getcwd(prevd, sizeof(prevd));
    int checker=0;
    if (chdir(new_directory) != 0)
    {
        checker=1;
        strcpy(prevd,storeero);
        perror("chdir");
    }
    if(checker!=1)
    printf("%s\n",getcwd(NULL,0));
    // else
    // {
    //     printf("%s\n",new_directory);
    // }
}