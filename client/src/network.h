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

int connect_to_server(const char* ip, int port);
int send_message(const char* msg);
int receive_message(char* buffer);


