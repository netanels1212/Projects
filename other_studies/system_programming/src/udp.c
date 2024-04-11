#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

#include "udp.h"

int CreateSocket(void)
{
    int sockfd = 0;

    /* Creating socket file descriptor */
    if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
    {
        perror("socket creation failed.\n");

        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void BindSocket(int sockfd, struct sockaddr_in *addr)
{
    if (0 > bind(sockfd, (const struct sockaddr *)addr, sizeof(*addr)))
    {
        perror("bind failed.\n");

        exit(EXIT_FAILURE);
    }
}

void ReceiveMessage(int sockfd, char *message, struct sockaddr_in *client_addr, socklen_t *len)
{
    int receive_bytes;
    memset(message, 0, MESG_MAX_LEN);
    receive_bytes = recvfrom(sockfd, message, MESG_MAX_LEN, 0, (struct sockaddr *)client_addr, len);

    if (-1 == receive_bytes)
    {
        perror("received failed.\n");

        exit(EXIT_FAILURE);
    }

    printf("***************%s\n", message);

}

void SendMessage(int sockfd, const char *message, struct sockaddr_in *client_addr, socklen_t len)
{
    if (-1 == sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)client_addr, len))
    {
        perror("sending failed.\n");

        exit(EXIT_FAILURE);
    }
}

void InitServer(struct sockaddr_in *server_addr)
{
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);
}

void InitClient(struct sockaddr_in *client_addr)
{
    client_addr->sin_family = AF_INET;
    client_addr->sin_addr.s_addr = inet_addr("10.10.0.247"); /* for broadcast */
    client_addr->sin_port = htons(PORT);
}

void CleanBuffers(char *server_message, char *client_message)
{
    memset((void *)server_message, '\0', MESG_MAX_LEN);
    memset((void *)client_message, '\0', MESG_MAX_LEN);
}