#include "network.h"

int sd;
pthread_mutex_t sock_mutex = PTHREAD_MUTEX_INITIALIZER;

int connect_to_server(const char* ip, int port)
{
    struct sockaddr_in server;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Eroare la socket()");
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("Eroare la connect()");
        return 0;
    }

    return 1;
}

int send_message(const char* msg)
{
    pthread_mutex_lock(&sock_mutex);
    int bytes = write(sd, msg, strlen(msg));
    pthread_mutex_unlock(&sock_mutex);
    return bytes;
}

int receive_message(char* buffer)
{
    int bytes = read(sd, buffer, MESSAGE_LENGTH - 1);

    if (bytes > 0)
    {
        buffer[bytes] = '\0';
    }

    return bytes;
}
