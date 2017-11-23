#include "server.h"
#include <sys/socket.h>
#include <arpha/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

int SocketFactory::getTCPServerSocket( int domain = AF_INET, int port, int allowedConnNumber = 5 )
{

	int sd = socket( domain, SOCK_STREAM, 0 );
	
	if( sd == -1 )
	{
		return ErrorCodes::SOCKET_OPEN_FAIL;
	}

	struct sockaddr_in server; 
	
	server.sin_family = domain;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );

	if( bind( sd, ( struct sockaddr * )&server, sizeof( server ) ) < 0 )
	{
		return ErrorCodes::SOCKET_BIND_FAIL;
	}

	if( listen( sd, allowedConnNumber ) < 0 ) 
	{
		return ErrorCodes::SOCKET_LISTEN_FAIL;
	}
	
	return sd;
}



int main( int argc, char** argv )
{
	
	SocketFactory f;
	
	int sd = f.getTCPServerSocket( 

}
