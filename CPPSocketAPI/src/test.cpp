#include "ISocketFactory.h"
#include "ISocketHandler.h"
#include <arpa/inet.h>
#include <iostream>

int main(){


	ISocketFactory* fact = new ISocketFactory;
	int desc = fact->getTCPSocket();
	ISocketHandler* handler = new ISocketHandler( desc, nullptr, 1100 );

	handler->bindSocket();
	handler->startListen( 5 );

	while( 1 ){

		struct sockaddr_in client;
		int conn = handler->acceptConnection( client );
		std::cout<< "Connection Established" << std::endl;

	}

	return 0;
}
