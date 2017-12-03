
#ifndef _SOCKET_H
#define _SOCKET_H


typedef int socket_desc;

socket_desc openTCPSocket( int port_number );

static int _bind( socket_desc socket, int port_number );


#endif
