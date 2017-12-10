#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>


socket_desc openTCPSocket(){

	socket_desc tcp_socket;
	tcp_socket = socket( AF_INET, SOCK_STREAM, 0 );
	
	return tcp_socket;

}

int connectToServer( socket_desc socket_descriptor, const char* server_address, int port_number ){

	struct sockaddr_in server_add;
	server_add.sin_addr.s_addr = inet_addr( server_address );
	server_add.sin_family = AF_INET;
	server_add.sin_port = htons( port_number );

	int return_val = connect( socket_descriptor, ( struct sockaddr*)&server_add, sizeof( struct sockaddr_in ) );

	return return_val;

}

int bindSocket( socket_desc socket, int port_number ){

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( port_number );

	int return_code = bind( socket, ( struct sockaddr* )&addr, sizeof( addr ) );

	return return_code;

}

