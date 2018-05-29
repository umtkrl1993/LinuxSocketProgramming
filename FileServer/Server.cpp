#include "Server.h"
#include <stdlib.h>

#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <sys/socket.h>
#include "TCPSocketHandler.h"


Server::Server( SocketHandler* socket_type ){

	mp_socket_type = socket_type;
	m_logger.open( "server.log", std::ios::out | std::ios::app );

}


void Server::startServer(){

	struct sockaddr_in* server_addr;
	struct sockaddr_in* client_addr;




	int socket_handler = mp_socket_type->openSocket();

	try{ 

		if( mp_socket_type->bindSocket( server_addr, 1500 ) != 0 ){

			exit(0);
		}

		if( mp_socket_type->listenSocket( 5 ) != 0 ){

			exit(0);
		}
	}

	catch( socketException &e ){
		
	}

	int sockaddr_len = sizeof( struct sockaddr_in );


	while( true ){ 
		m_logger << "Waiting for an incoming connection ...\n";
		int connection_id = accept( socket_handler, ( struct sockaddr* )&client_addr, (socklen_t*)&sockaddr_len );
		handleConnection( connection_id, client_addr );
	}


}

void Server::handleConnection( int connection_id, const struct sockaddr_in* client_addr ){

	pid_t pid = fork();

}


void Server::setConnectionType( SocketHandler* connection_type ){



}