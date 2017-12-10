
#ifndef _SOCKET_H
#define _SOCKET_H

typedef int socket_desc;

socket_desc openTCPSocket();

int connectToServer( socket_desc socket_descriptor, const char* server_address, int port_number );

int bindSocket( socket_desc socket, int port_number );


#endif
