#ifndef _TCPSOCKETHANDLER_H
#define _TCPSOCKETHANDLER_H

#include "SocketHandler.h"
#include <exception>

class sockaddr_in;

class TCPSocketHandler : public SocketHandler{

public:
	TCPSocketHandler():
		m_socket_desc(-1)
		{

		}
	int openSocket();
	int bindSocket( struct sockaddr_in* bind_address, int port_number ) const;
	int listenSocket( int backlog ) const;

private:
	int m_socket_desc;

};


class socketException : public std::exception{

	virtual const char* what() const throw(){
		return "unopened socket exception";
	}
};




#endif