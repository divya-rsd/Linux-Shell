#include "headers.h"
void fetch_man_page(const char *url, char *html)
{
    struct hostent *he;
    struct sockaddr_in server;
    int sock;
    if ((he = gethostbyname("man.he.net")) == NULL)
    {
        herror("gethostbyname");
        return;
    }
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    if (connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        close(sock);
        return;
    }

    char request[1024];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: man.he.net\r\n\r\n", url);

    if (send(sock, request, strlen(request), 0) == -1)
    {
        perror("send");
        close(sock);
        return;
    }

    char buffer[100000];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        strcat(html, buffer); 
    }

    close(sock);
}
void removeTags(char *html)
{
     int opener = 0;
    char *read = html;
    char *write = html;
    while (*read)
    {
        if (*read == '<')
        {
            opener = 1;
        }
        else if (*read == '>')
        {
            opener = 0;
        }
        else if (!opener)
        {
            *write = *read;
            write++;
        }
        read++;
    }
    *write = '\0';
}
void iManspec(char *input)
{

    char url[1024];
    snprintf(url, sizeof(url), "/?topic=%s&section=all", input);
    char html[100000];
    fetch_man_page(url, html);
    removeTags(html);
    char *start = strstr(html, "NAME");
    if (start)
    {
        printf("%s\n", start);
    }
    else
    {
        printf("Content not found or invalid HTML format.\n");
    }
    html[0] = '\0';
}
