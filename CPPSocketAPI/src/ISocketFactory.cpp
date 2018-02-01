#include "ISocketFactory.h"
#include <sys/socket.h>
#include <arpa/inet.h>

enum ErrorCodes{

	SOCKET_OPEN_FAIL = -1

};



int ISocketFactory::getTCPSocket() const{

	int sd = socket( AF_INET, SOCK_STREAM, 0 );
	
	if( sd == -1 ){
	
		return ErrorCodes::SOCKET_OPEN_FAIL;
	}

	return sd;
}

int ISocketFactory::getRawTCPSocket() const{


	int sd = socket( AF_INET, SOCK_RAW, IPPROTO_TCP );

	if( sd == -1 ){

		return ErrorCodes::SOCKET_OPEN_FAIL;

	}

	return sd;

}