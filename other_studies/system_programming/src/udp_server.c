#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "udp.h"

int main()
{
    int socket_desc;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char server_message[MESG_MAX_LEN] = {0};
    char client_message[MESG_MAX_LEN] = {0};
    socklen_t client_struct_length = sizeof(client_addr);

    /* Create UDP socket: */
    socket_desc = CreateSocket();

    printf("Socket created successfully\n");

    /* Set port and IP: */
    InitServer(&server_addr);

    /* Bind to the set port and IP: */
    BindSocket(socket_desc, &server_addr);

    printf("Done with binding\n");

    printf("Listening for incoming messages...\n\n");

while (1)
{
    /* Receive client's message: */
    ReceiveMessage(socket_desc, client_message, &client_addr, &client_struct_length);

    printf("Received message from IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    printf("Msg from client: %s\n", client_message);

    /* Respond to client: */
    SendMessage(socket_desc, client_message, &client_addr, client_struct_length);

    /* Clean buffers: */
    CleanBuffers(server_message, client_message);
}

    /* Close the socket: */
    close(socket_desc);

    return 0;
}
