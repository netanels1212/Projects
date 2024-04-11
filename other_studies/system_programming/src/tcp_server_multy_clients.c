#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_CLIENTS 3

int main(void)
{
    int i = 0;
    int socket_desc;
    int client_sockets[MAX_CLIENTS] = {0};
    int max_sd, activity, new_socket, client_sock;
    socklen_t client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000] = {0};
    char client_message[2000] = {0};
    fd_set readfds;

    /* Create socket: */
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");

    /* Set port and IP: */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Bind to the set port and IP: */
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");

    /* Listen for clients: */
    if (listen(socket_desc, MAX_CLIENTS) < 0)
    {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(socket_desc, &readfds);
        max_sd = socket_desc;

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            client_sock = client_sockets[i];

            if (client_sock > 0)
                FD_SET(client_sock, &readfds);

            if (client_sock > max_sd)
                max_sd = client_sock;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            printf("Error in select\n");
            return -1;
        }

        if (FD_ISSET(socket_desc, &readfds))
        {
            if ((new_socket = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size)) < 0)
            {
                printf("Can't accept\n");
                return -1;
            }

            printf("New connection, socket fd is %d, IP is: %s, Port is: %d\n", new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            for (i = 0; i < MAX_CLIENTS; ++i)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < MAX_CLIENTS; ++i)
        {
            client_sock = client_sockets[i];

            if (FD_ISSET(client_sock, &readfds))
            {
                if (recv(client_sock, client_message, sizeof(client_message), 0) <= 0)
                {
                    printf("Client disconnected, socket fd is %d\n", client_sock);
                    close(client_sock);
                    client_sockets[i] = 0;
                }
                else
                {
                    printf("Msg from client %d: %s\n", client_sock, client_message);

                    /* Respond to client: */
                    strcpy(server_message, "This is the server's message.");

                    if (send(client_sock, server_message, strlen(server_message), 0) < 0)
                    {
                        printf("Can't send\n");
                        return -1;
                    }
                }
            }
        }

        /* Clean buffers: */
        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));
    }

    /* Closing the socket: */
    close(socket_desc);

    return 0;
}
