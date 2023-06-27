#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SRVR_SOCKET_NAME "socket.soc\0"
#define CLNT_SOCKET_NAME "client%lu.soc\0"
#define BUF_SIZE         100

void clear_socket(int sockfd, const char *name)
{
    close(sockfd);
    unlink(name);
}

int main(void)
{
    int sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    if (sock_fd == -1)
    {
        perror("socket() failed.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_un rcvr_name;
    rcvr_name.sun_family = AF_UNIX;
    snprintf(rcvr_name.sun_path, sizeof(rcvr_name.sun_path), 
             CLNT_SOCKET_NAME, getpid());

    if (bind(sock_fd, (struct sockaddr *) &rcvr_name, 
             sizeof(rcvr_name)) == -1)
    {
        perror("bind() failed.\n");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    struct sockaddr_un srvr_name;
    srvr_name.sun_family = AF_UNIX;
    strncpy(srvr_name.sun_path, SRVR_SOCKET_NAME, strlen(SRVR_SOCKET_NAME) + 1);

    char sbuf[BUF_SIZE];
    snprintf(sbuf, BUF_SIZE, "Hello from client with pid %d", getpid());

    if (sendto(sock_fd, sbuf, strlen(sbuf), 0, 
               (struct sockaddr *) &srvr_name, sizeof(srvr_name)) == -1)
    {
        perror("sendto() failed.\n");
        clear_socket(sock_fd, rcvr_name.sun_path);
        return EXIT_FAILURE;
    }
    
    printf("SENT MESSAGE: %s\n", sbuf);

    char rbuf[BUF_SIZE];
    int bytes = recvfrom(sock_fd, rbuf, sizeof(rbuf), 0, NULL, NULL);

    if (bytes == -1)
    {
        perror("recvfrom() failed.\n");
        clear_socket(sock_fd, rcvr_name.sun_path);
        return EXIT_FAILURE;
    }

    rbuf[bytes] = '\0';
    
    printf("GOT MESSAGE: %s\n", rbuf);
    sleep(10);

    clear_socket(sock_fd, rcvr_name.sun_path);

    return 0;
}
