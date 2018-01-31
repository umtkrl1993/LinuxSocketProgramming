#include "server.h"
#include <sys/socket.h>
#include <arpha/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

int SocketFactory::getTCPSocket( int domain = AF_INET ) const{

	int sd = socket( domain, SOCK_STREAM, 0 );
	
	if( sd == -1 ){
	
		return ErrorCodes::SOCKET_OPEN_FAIL;
	}

	return sd;
}

int SocketFactory::getRawTCPSocket( int domain = AF_INET ) const{


	int sd = socket( domain, SOCK_RAW,IPPROTO_TCP );

	if( sd == -1 ){

		return ErrorCodes::SOCKET_OPEN_FAIL;

	}

	return sd;

}

int SocketFactory::bindSocket( int socket_descriptor, const char* ip_address, int port ) const{
	struct sockaddr_in server; 
	
	server.sin_family = domain;

	if( ip_address == NULL )
		server.sin_addr.s_addr = INADDR_ANY;
	else
		server.sin_addr.s_addr = inet_addr( ip_address );

	server.sin_port = htons( port );

	if( bind( socket_desctiptor, ( struct sockaddr * )&server, sizeof( server ) ) < 0 )
	{
		return ErrorCodes::SOCKET_BIND_FAIL;
	}

	return 0;

}


int main( int argc, char** argv )
{
	
	SocketFactory f;

}
