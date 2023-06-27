#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SOCKET_NAME "socket.soc"
#define BUF_SIZE    100
#define SRVR_MSG    "Answer from server"

static int sock_fd;

void clear_socket(int sockfd)
{
    close(sockfd);
    unlink(SOCKET_NAME);
}

void sigint_handler(int signal_num)
{
    clear_socket(sock_fd);
    exit(0);
}

int main(void)
{
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (sock_fd == -1)
    {
        perror("socket() failed.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_un srvr_name;
    srvr_name.sun_family = AF_UNIX;
    strncpy(srvr_name.sun_path, SOCKET_NAME, strlen(SOCKET_NAME) + 1);

    if (bind(sock_fd, (struct sockaddr *) &srvr_name, 
             sizeof(srvr_name)) == -1)
    {
        perror("bind() failed.\n");
        return EXIT_FAILURE;
    }

    signal(SIGINT, sigint_handler);

    char sbuf[BUF_SIZE];
    strncpy(sbuf, SRVR_MSG, strlen(SRVR_MSG));

    while (1)
    {
        char rbuf[BUF_SIZE];
        struct sockaddr_un rcvr_name;
        socklen_t namelen = sizeof(rcvr_name);

        int bytes = recvfrom(sock_fd, rbuf, sizeof(rbuf), 0, 
                             (struct sockaddr *) &rcvr_name, &namelen);

        if (bytes == -1)
        {
            perror("recvfrom() failed.\n");
            clear_socket(sock_fd);
            return EXIT_FAILURE;
        }

        rbuf[bytes] = '\0';
        printf("GOT MESSAGE: %s\n", rbuf);

        bytes = sendto(sock_fd, sbuf, strlen(sbuf), 0, 
                   (struct sockaddr *) &rcvr_name, namelen);

        if (bytes == -1)
        {
            perror("sendto() failed.\n");
            clear_socket(sock_fd);
            return EXIT_FAILURE;
        }

        printf("SENT MESSAGE: %s\n", sbuf);
    }

    return 0;
}
