#ifndef HEADERS_H_
#define HEADERS_H_
extern char initiald[1024];
extern char prevd[1024];
extern char prprinter[1024];
extern int set;
extern char finalprint[1024];
extern int err;
#include<sys/types.h>
#include <signal.h>
extern pid_t current_foreground_pid;
extern volatile sig_atomic_t interrupt_flag ;
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "prompt.h"
#include "spec2.h"
#include "spec3.h"
#include "spec4.h"
#include "past.h"
#include "spec6.h"
#include "spec7.h"
#include "spec8.h"
#include "sp4details.h"
#include "neonate.h"
#include "manpage.h"
#include "spec13.h"
#include "ping.h"
#include "helper.h"
#endif
