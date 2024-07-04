#ifndef _iMan_H
#define _iMan_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
void fetch_man_page(const char *url, char *html);
void iManspec(char *input);
void removeTags(char *html);
#endif