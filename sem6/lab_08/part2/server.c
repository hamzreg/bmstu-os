#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_COUNT_CLIENTS 10
#define SOCKET_PORT       9890
#define BUF_SIZE          100
#define SRVR_MSG    "Answer from server"

static int sock_fd;

int main(void)
{
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (sock_fd == -1)
    {
        perror("socket() failed.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SOCKET_PORT);

    if (bind(sock_fd, (struct sockaddr *) &serv_addr, 
             sizeof(serv_addr)) == -1)
    {
        perror("bind() failed.\n");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    if (listen(sock_fd, 1) == -1)
    {
        perror("listen() failed.\n");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    int clients[MAX_COUNT_CLIENTS] = {0};
    fd_set set;

    char sbuf[BUF_SIZE];
    strncpy(sbuf, SRVR_MSG, strlen(SRVR_MSG));

    while (1)
    {
        FD_ZERO(&set);
        FD_SET(sock_fd, &set);
        int max_sock_fd = sock_fd;

        for (int i = 0; i < MAX_COUNT_CLIENTS; ++i)
        {
            if (clients[i] > 0)
                FD_SET(clients[i], &set);
            
            if (clients[i] > max_sock_fd)
                max_sock_fd = clients[i];
        }

        if (select(max_sock_fd + 1, &set, NULL, NULL, NULL) == -1)
        {
            perror("select() failed.\n");
            return EXIT_FAILURE;  
        }

        if (FD_ISSET(sock_fd, &set))
        {
            struct sockaddr_in cli_addr;
            int clean;
            int new_sock = accept(sock_fd, (struct sockaddr *) &cli_addr, &clean);

            if (new_sock == - 1)
            {
                perror("accept() failed.\n");
                return EXIT_FAILURE;        
            }

            int flag = 1;

            for (int i = 0; flag && i < MAX_COUNT_CLIENTS; ++i)
            {
                if (clients[i] == 0)
                {
                    clients[i] = new_sock;
                    printf("Client № %d\n", i);
                    flag = 0;
                }
            }

            if (flag)
            {
                printf("Reached max count of clients.\n");
            }
        }

        for (int i = 0; i < MAX_COUNT_CLIENTS; ++i)
        {
            if (clients[i] && FD_ISSET(clients[i], &set))
            {
                char rbuf[BUF_SIZE];
                size_t bytes = recvfrom(clients[i], rbuf, sizeof(rbuf), 0, NULL, NULL);

                if (bytes == 0)
                {
                    printf("Client № %d disconnected.\n", i);
                    close(clients[i]);
                    clients[i] = 0;
                }
                else
                {
                    rbuf[bytes] = '\0';
                    printf("GOT MESSAGE (client № %d): %s\n", i, rbuf);

                    bytes = send(clients[i], sbuf, strlen(sbuf), 0);

                    if (bytes == -1)
                    {
                        perror("sendto() failed.\n");
                    }
                    else 
                        printf("SENT MESSAGE: %s\n", sbuf);
                }
            }
        }
    }

    close(sock_fd);
    return 0;
}
