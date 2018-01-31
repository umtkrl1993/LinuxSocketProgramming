#include "isocketfactory.h"
#include "defs.h"


int SocketFactory::getTCPSocket() const{

	int sd = socket( domain, SOCK_STREAM, 0 );
	
	if( sd == -1 ){
	
		return ErrorCodes::SOCKET_OPEN_FAIL;
	}

	return sd;
}

int SocketFactory::getRawTCPSocket() const{


	int sd = socket( domain, SOCK_RAW, IPPROTO_TCP );

	if( sd == -1 ){

		return ErrorCodes::SOCKET_OPEN_FAIL;

	}

	return sd;

}