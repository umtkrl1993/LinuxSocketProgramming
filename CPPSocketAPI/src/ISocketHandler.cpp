#include "ISocketHandler.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>


ISocketHandler::ISocketHandler( const ISocketHandler& handler ){

	m_socket_descriptor = handler.m_socket_descriptor;
	mp_ip_address = handler.mp_ip_address;
	m_port = handler.m_port;
}


ISocketHandler::ISocketHandler( int socket_descriptor ){

	m_socket_descriptor = socket_descriptor;
	mp_ip_address = nullptr;
	m_port = 0;
}


ISocketHandler::ISocketHandler( int socket_descriptor, char* ip, int port ){

	m_socket_descriptor = socket_descriptor;
	mp_ip_address = ip;
	m_port = port;
}

ISocketHandler& ISocketHandler::operator=( const ISocketHandler& handler ){

	m_socket_descriptor = handler.m_socket_descriptor;
	mp_ip_address = handler.mp_ip_address;
	m_port = handler.m_port;

}

ISocketHandler::~ISocketHandler(){

}

int ISocketHandler::bindSocket() const{

	struct sockaddr_in bind_address;


	if( m_port == 0 || m_socket_descriptor == 0 ){

		return -1;
	}

	bind_address.sin_family = AF_INET;
	bind_address.sin_port = htons( m_port );

	if( mp_ip_address == nullptr ){

		bind_address.sin_addr.s_addr = INADDR_ANY;
	}

	else{
		bind_address.sin_addr.s_addr = inet_addr( mp_ip_address );
	}

	int return_value = bind( m_socket_descriptor, (struct sockaddr*)&bind_address, sizeof( bind_address ) );

	return return_value;
}

int ISocketHandler::startListen( int backlog ) const{

	int return_value = listen( m_socket_descriptor, backlog );

	return return_value;
}

// Need to check if descriptor is valid or not
int ISocketHandler::acceptConnection( struct sockaddr_in& client ) const{

	int size = sizeof( struct sockaddr_in );

	int connected_socket = accept( m_socket_descriptor, ( sockaddr*)&client, ( socklen_t*) &( size ) );

	return connected_socket;
}