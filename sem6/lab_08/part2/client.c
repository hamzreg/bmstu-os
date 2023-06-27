#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SOCKET_PORT 9890
#define BUF_SIZE    100

static int sock_fd;

void sigint_handler(int signal_num)
{
    close(sock_fd);
    exit(0);
}

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

    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("connect() failed.\n");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    signal(SIGINT, sigint_handler);

    while (1)
    {
        char sbuf[BUF_SIZE];
        snprintf(sbuf, BUF_SIZE, "Hello from cleint with pid %d", getpid());

        int bytes = send(sock_fd, sbuf, strlen(sbuf), 0);

        if (bytes == -1)
        {
            perror("send() failed.\n");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        printf("SENT MESSAGE: %s\n", sbuf);

        char rbuf[BUF_SIZE];
        bytes = recvfrom(sock_fd, rbuf, sizeof(rbuf), 0, NULL, NULL);

        if (bytes == -1)
        {
            perror("recvfrom() failed.\n");
            close(sock_fd);
            return EXIT_FAILURE;
        }

        rbuf[bytes] = '\0';
        
        printf("GOT MESSAGE: %s\n", rbuf);
    }
}
