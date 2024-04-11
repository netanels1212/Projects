#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define UDP_PORT 8080
#define TCP_PORT 9090
#define MAXLINE 1024
#define MAX_CLIENTS 10

void handle_udp_connection(int udp_sock)
{
    char buffer[MAXLINE];
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int n;

    /*Receive message from UDP client*/
    n = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
    if (-1 == n)
    {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';

    printf("UDP client: %s\n", buffer);

    /*Check if the received message is "quit"*/
    if (strcmp(buffer, "quit") == 0)
    {
        printf("Received 'quit' command from UDP client. Server shutting down...\n");
        close(udp_sock);    /*Close the UDP socket*/
        exit(EXIT_SUCCESS); /*Exit the server*/
    }

    /*Respond with "pong" for other messages*/
    sendto(udp_sock, "pong", strlen("pong"), 0, (struct sockaddr *)&cliaddr, len);
}

void handle_tcp_connection(int tcp_sock, int *client_sockets)
{
    char buffer[MAXLINE];
    int n;
    int i = 0;

    n = read(tcp_sock, buffer, sizeof(buffer));
    if (n <= 0)
    {
        if (n == 0)
        {
            printf("TCP client disconnected\n");
        }
        else
        {
            perror("read failed");
        }

        close(tcp_sock);
        for (i = 0; i < MAX_CLIENTS; ++i)
        {
            if (client_sockets[i] == tcp_sock)
            {
                client_sockets[i] = 0;
                break;
            }
        }
    }
    else
    {
        buffer[n] = '\0';
        printf("TCP client: %s\n", buffer);

        /*Check if the received message is "quit"*/
        if (strcmp(buffer, "quit") == 0)
        {
            printf("Received 'quit' command from TCP client. Server shutting down...\n");
            close(tcp_sock);
            for (i = 0; i < MAX_CLIENTS; ++i)
            {
                if (client_sockets[i] == tcp_sock)
                {
                    client_sockets[i] = 0;
                    break;
                }
            }
            close(tcp_sock);    /*Close the TCP socket*/
            exit(EXIT_SUCCESS); /*Exit the server*/
        }

        /*Respond with "pong" for other messages*/
        write(tcp_sock, "pong", strlen("pong"));
    }
}

int main()
{
    int udp_sock, tcp_sock;
    struct sockaddr_in servaddr;
    int broadcastEnable = 1;
    int i = 0;
    int max_sd = 0;
    int activity = 0;
    /*Initialize array to track client sockets*/
    int client_sockets[MAX_CLIENTS] = {0};
    /*Set timeout for 7 seconds*/
    struct timeval timeout;

    /*Create UDP socket*/
    if ((udp_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    /*Create TCP socket*/
    if ((tcp_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    /*Initialize server address structure for UDP*/
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    /* servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); */
    servaddr.sin_port = htons(UDP_PORT);

    if (setsockopt(udp_sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        perror("setsockopt(SO_BROADCAST) failed");
        exit(EXIT_FAILURE);
    }
    /*Bind UDP socket*/
    if (bind(udp_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server listening on port %d...\n", UDP_PORT);

    /*Initialize server address structure for TCP*/
    servaddr.sin_port = htons(TCP_PORT);

    /*Bind TCP socket*/
    if (bind(tcp_sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    /*Listen for TCP connections*/
    if (listen(tcp_sock, 5) == -1)
    {
        perror("TCP listen failed");
        exit(EXIT_FAILURE);
    }

    printf("TCP Server listening on port %d...\n", TCP_PORT);

    /*Main server loop*/
    while (1)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(udp_sock, &fds);
        FD_SET(tcp_sock, &fds);
        FD_SET(STDIN_FILENO, &fds);

        /*Add client sockets to the set*/
        max_sd = tcp_sock;
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (sd > 0)
            {
                FD_SET(sd, &fds);
                if (sd > max_sd)
                {
                    max_sd = sd;
                }
            }
        }

        timeout.tv_sec = 7;
        timeout.tv_usec = 0;

        /*Wait for activity on any of the sockets*/
        activity = select(max_sd + 1, &fds, NULL, NULL, &timeout);
        if (activity == -1)
        {
            perror("select failed");
            exit(EXIT_FAILURE);
        }
        else if (activity == 0)
        {
            printf("No activity for 7 seconds\n");
            continue;
        }

        /*Check for activity on stdin*/
        if (FD_ISSET(STDIN_FILENO, &fds))
        {
            char input[MAXLINE];
            fgets(input, MAXLINE, stdin);
            input[strcspn(input, "\n")] = '\0'; /*Remove newline character*/

            /*Check if input is "quit"*/
            if (strcmp(input, "quit") == 0)
            {
                printf("Server shutting down...\n");
                close(udp_sock); /*Close the UDP socket*/
                close(tcp_sock); /*Close the TCP socket*/
                exit(EXIT_SUCCESS);
            }
        }
        /*Handle UDP activity*/
        if (FD_ISSET(udp_sock, &fds))
        {
            handle_udp_connection(udp_sock);
        }

        /*Handle TCP activity*/
        if (FD_ISSET(tcp_sock, &fds))
        {
            int tcp_conn = accept(tcp_sock, NULL, NULL);
            if (tcp_conn == -1)
            {
                perror("TCP accept failed");
                exit(EXIT_FAILURE);
            }
            printf("New TCP client connected\n");

            /*Add the new client socket to the array*/
            for (i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_sockets[i] == 0)
                {
                    client_sockets[i] = tcp_conn;
                    break;
                }
            }
        }

        /*Handle client activity*/
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_sockets[i];
            if (FD_ISSET(sd, &fds))
            {
                handle_tcp_connection(sd, client_sockets);
            }
        }
    }

    close(udp_sock);
    close(tcp_sock);

    return 0;
}
