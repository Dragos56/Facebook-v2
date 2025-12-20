#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include "handle_commands.h"
#include "database.h"

#define PORT 5656

extern int errno;

typedef struct
{
    pthread_t idThread;
    int thCount;
} Thread;

Thread *threadsPool;
int sd;
int nthreads;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

static void *treat(void *);
void raspunde(int client, int idThread);
void threadCreate(int i);

void stop_server()
{
    db_close();
    close(sd);
    pthread_mutex_destroy(&mlock);
    free(threadsPool);
    exit(0);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server;

    if (argc < 2)
    {
        fprintf(stderr, "Eroare: Primul argument este numarul de fire de executie...\n");
        exit(1);
    }

    nthreads = atoi(argv[1]);
    if (nthreads <= 0)
    {
        fprintf(stderr, "Eroare: Numar de fire invalid...\n");
        exit(1);
    }

    if (db_init("data/database.db") != 0) 
    {
        fprintf(stderr, "EROARE DB, se inchide server-ul...\n");
        exit(1);
    }

    //signal(SIGINT, stop_server());

    threadsPool = (Thread*)calloc(sizeof(Thread), nthreads);

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server] Eroare la socket().\n");
        return errno;
    }

    int on = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server] Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, nthreads) == -1)
    {
        perror("[server] Eroare la listen().\n");
        return errno;
    }

    printf("Nr threaduri %d \n", nthreads);
    fflush(stdout);

    int i;
    for (i = 0; i < nthreads; i++)
    {
        threadCreate(i);
    }

    for (;;)
    {
        printf("[server] Asteptam la portul %d...\n", PORT);
        pause();
    }
}

void threadCreate(int i)
{
    pthread_create(&threadsPool[i].idThread, NULL, &treat, (void *)(intptr_t)i);
    return;
}

void *treat(void *arg)
{
    int client;
    struct sockaddr_in from;
    bzero(&from, sizeof(from));
    printf("[thread] - %d - pornit...\n", (int)(intptr_t)arg);
    fflush(stdout);

    for (;;)
    {
        int length = sizeof(from);

        pthread_mutex_lock(&mlock);
        if ((client = accept(sd, (struct sockaddr *)&from, (socklen_t *)&length)) < 0)
        {
            perror("[thread] Eroare la accept().\n");
        }
        pthread_mutex_unlock(&mlock);

        if (client >= 0)
        {
            threadsPool[(int)(intptr_t)arg].thCount++;
            raspunde(client, (int)(intptr_t)arg);
            close(client);
        }
    }
}

void raspunde(int client, int idThread)
{
    printf("[Thread %d] Client conectat. Procesam comenzi...\n", idThread);
    fflush(stdout);

    while (1)
    {
        if (handle_commands(client))
        {
            break;
        }
    }

    printf("[Thread %d] Client deconectat.\n", idThread);
}