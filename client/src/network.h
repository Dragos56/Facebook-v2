#pragma once

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MESSAGE_LENGTH 1000

extern int errno;

int connect_to_server(const char* ip, int port);
int send_message(const char* msg);
int receive_message(char* buffer);


