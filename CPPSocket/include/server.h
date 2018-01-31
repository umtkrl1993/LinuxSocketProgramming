#ifndef _SERVER_H
#define _SERVER_H

#include <netinet/in.h>

class SocketFactory
{

public:
	int getTCPSocket( int domain = AF_INET ) const;
	int getRawTCPSocket( int domain == AF_INET ) const;


};


class SocketHandler{

	public:
		SocketHandler();
		SocketHandler( char* ip, int port );
		~SocketHandler();
		int bindSocket() const;

		int startListen() const;


	private:
		char* ip_address;
		int port;


};


#endif
