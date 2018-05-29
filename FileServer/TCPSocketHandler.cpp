#include "TCPSocketHandler.h"
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr




int TCPSocketHandler::openSocket(){

	m_socket_desc = SocketHandler::openSocket();
	return m_socket_desc;
}


int TCPSocketHandler::bindSocket( struct sockaddr_in* bind_address, int port_number ) const {

	if( m_socket_desc != -1 ){ 

		bind_address->sin_family = AF_INET;
		bind_address->sin_addr.s_addr = INADDR_ANY;
		bind_address->sin_port = htons( port_number );

		return bind( m_socket_desc, ( struct sockaddr* )bind_address, sizeof( struct sockaddr_in ) );
	}

	else{

		throw socketException();
	}

}

int TCPSocketHandler::listenSocket( int backlog ) const{

	if( m_socket_desc != -1 ){

		return listen( m_socket_desc, backlog );
	}

	else{

		throw socketException();
	}

}
