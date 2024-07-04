#ifndef __sp4d_H
#define __sp4d_H
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include<string.h>
void print_permissions(mode_t mode);
int func(const char *dir_path,int sh);
int compare_filenames(const struct dirent **a, const struct dirent **b);
#endif