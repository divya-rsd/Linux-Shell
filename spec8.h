#ifndef __sp8_H
#define __sp8_H
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <strings.h>
void seek(const char *flags,const char *target_name, const char *target_dir);
void spec8(char *input);
#endif