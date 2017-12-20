#include "socket.h"
#include <sys/socket.h>


socket_desc openTCPSocket(){

	socket_desc tcp_socket;
	tcp_socket = socket( AF_INET, SOCK_STREAM, 0 );
	
	return tcp_socket;

}

int connectToServer( socket_desc socket_descriptor, struct sockaddr_in* addr ,const char* server_address, int port_number ){

	addr->sin_addr.s_addr = inet_addr( server_address );
	addr->sin_family = AF_INET;
	addr->sin_port = htons( port_number );

	int return_val = connect( socket_descriptor, ( struct sockaddr*)addr, sizeof( struct sockaddr_in ) );

	return return_val;

}

int bindSocket( socket_desc socket, struct sockaddr_in* addr, int port_number ){

	addr->sin_family = AF_INET;
	addr->sin_addr.s_addr = INADDR_ANY;
	addr->sin_port = htons( port_number );

	int return_code = bind( socket, ( struct sockaddr* )addr, sizeof( struct sockaddr_in ) );

	return return_code;

}

