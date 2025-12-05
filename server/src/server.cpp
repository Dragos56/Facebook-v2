#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "handle_commands.h"

#define PORT 5656

extern int errno;

char *conv_addr(struct sockaddr_in address) 
{
    static char str[25];
    char port[7];

    strcpy (str, inet_ntoa (address.sin_addr));
    bzero (port, 7);
    sprintf (port, ":%d", ntohs (address.sin_port));
    strcat (str, port);
    return (str);
}

int main ()
{
    struct sockaddr_in server;	
    struct sockaddr_in from;
    fd_set readfds;		
    fd_set actfds;		
    struct timeval tv;		
    int sd, client;		
    int optval=1; 		
    int fd;			
    int nfds;		
    socklen_t len;	

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("[server] Eroare la socket().\n");
        return errno;
    }

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    bzero (&server, sizeof (server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("[server] Eroare la bind().\n");
        return errno;
    }

    if (listen (sd, 5) == -1)
    {
        perror ("[server] Eroare la listen().\n");
        return errno;
    }

    FD_ZERO (&actfds);	
    FD_SET (sd, &actfds);	

    tv.tv_sec = 1;	
    tv.tv_usec = 0;

    nfds = sd;

    printf ("[server] Asteptam la portul %d...\n", PORT);
    fflush (stdout);

    while (1)
    {
        bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

        if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
        {
            perror ("[server] Eroare la select().\n");
            return errno;
        }
        
        if (FD_ISSET (sd, &readfds))
	    {
            
            len = sizeof (from);
            bzero (&from, sizeof (from));

            
            client = accept (sd, (struct sockaddr *) &from, &len);

            if (client < 0)
            {
                perror ("[server] Eroare la accept().\n");
                continue;
            }

            if (nfds < client) 
                nfds = client;

            FD_SET (client, &actfds);

            printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
            fflush (stdout);
	    }
        for (fd = 0; fd <= nfds; fd++)
	    {
            if (fd != sd && FD_ISSET (fd, &readfds))
            {
	            if (handle_commands(fd))
		        {
                    printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
                    fflush (stdout);
                    close (fd);		
                    FD_CLR (fd, &actfds);
		        }
	        }
	    }			
    }				
}				

