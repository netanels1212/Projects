/*******************************************************************************
Project: UDP
Name: Netanel shemual
Reviewer: 
Date:
Version: 1.0    

*******************************************************************************/

#ifndef __UDP_H__
#define __UDP_H__

#include <netinet/in.h>

#define PORT    8080
#define MESG_MAX_LEN 2000

int  CreateSocket(void);
void BindSocket(int sockfd, struct sockaddr_in *addr);
void ReceiveMessage(int sockfd, char *message, struct sockaddr_in *client_addr, socklen_t *len);
void SendMessage(int sockfd, const char *message, struct sockaddr_in *client_addr, socklen_t len);
void InitServer(struct sockaddr_in *server_addr);
void InitClient(struct sockaddr_in *client_addr);
void CleanBuffers(char *server_message, char *client_message);

#endif /*__UDP_H__*/