#ifndef _sig_H_
#define _sig_H_
void sigint_handler(int signum);
void sigtstp_handler(int signum);
void cleanup();
#endif