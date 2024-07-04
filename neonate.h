#ifndef __neo_H
#define __neo_H
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
void die(const char *s);
void enableRawMode();
void disableRawMode();
int getMostRecentProcessPID();
void spec15(int pause);
void handle(char *input);
#endif
