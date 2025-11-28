#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define MESSAGE_LENGTH 1000

extern int errno;

int sd;

int connect_to_server(const char* ip, int port) 
{
    struct sockaddr_in server;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        perror ("socket() error");
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("connect() error");
        return 0;
    }

    return 1;
}

int send_message(const char* msg) 
{
    return write(sd, msg, MESSAGE_LENGTH);
}

int receive_message(char* buffer) 
{
    return read(sd, buffer, MESSAGE_LENGTH);
}
