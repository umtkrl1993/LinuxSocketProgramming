#ifndef _SERVER_H
#define _SERVER_H

#include "SocketHandler.h"
#include <fstream>

class sockaddr_in;

class Server {

public:
	Server( SocketHandler* socket_type );
	void startServer();
	
	void setConnectionType( SocketHandler* connection_type );

private:
	void handleConnection( int connection_id, const struct sockaddr_in* client_addr );
	SocketHandler* mp_socket_type;
	std::ofstream m_logger;


};



#endif