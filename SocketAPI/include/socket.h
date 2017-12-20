
#ifndef _SOCKET_H
#define _SOCKET_H
#include<arpa/inet.h>

typedef int socket_desc;

socket_desc openTCPSocket();

int connectToServer( socket_desc socket_descriptor,struct sockaddr_in* addr, const char* server_address, int port_number );

int bindSocket( socket_desc socket, struct sockaddr_in* addr, int port_number );


#endif
