#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "udp.h"

int main()
{
    int socket_desc = 0;
    int broadcastEnable = 1;
    struct sockaddr_in server_addr;
    char server_message[MESG_MAX_LEN] = {0};
    char client_message[MESG_MAX_LEN] = {0};
    socklen_t server_struct_length = sizeof(server_addr);

    /* Create socket: */
    socket_desc = CreateSocket();

    printf("Socket created successfully\n");

    /* Set port and IP: */
    InitClient(&server_addr);

    /* Enable broadcast option */
    if (setsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        perror("setsockopt failed.\n");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        /* Get input from the user: */
        printf("Enter message to broadcast: ");
        fgets(client_message, sizeof(client_message), stdin);

        /* Remove the trailing newline character from fgets input */
        client_message[strcspn(client_message, "\n")] = '\0';

        /* Send the message to the server: */
        SendMessage(socket_desc, client_message, &server_addr, server_struct_length);

        printf("Broadcasted message: %s\n", client_message);

        /* Receive the server's response: */
        ReceiveMessage(socket_desc, server_message, &server_addr, &server_struct_length);

        printf("Server's response: %s\n", server_message);
    }

    /* Close the socket: */
    close(socket_desc);

    return 0;
}
