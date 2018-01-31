#include "isockHandler.h"


ISocketHandler::ISocketHandler( const ISocketHandler& handler ){



}


ISocketHandler::ISocketHandler( int socket_descriptor ){

	m_socket_descriptor = socket_descriptor;
	mp_ip_address = nullptr;
	m_port = 0;
}


ISocketHandler::ISocketHandler( int socket_descriptor, char* ip, int port ){


}

ISocketHandler::ISocketHandler& operator=( const ISocketHandler& handler ){


}