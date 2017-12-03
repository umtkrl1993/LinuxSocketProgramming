#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>

socket_desc openTCPSocket( int port_number ){

	socket_desc tcp_socket;
	
 	tcp_socket = socket(AF_INET , SOCK_STREAM , 0);
	int return_code = _bind( tcp_socket, port_number );

	if( return_code == -1 ){

		return -1;
	}

	return tcp_socket;

}


static int _bind( socket_desc socket, int port_number ){

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( port_number );

	int return_code = bind( socket, ( struct sockaddr* )&addr, sizeof( addr ) );

	return return_code;

}

