#ifndef _SERVER_H
#define _SERVER_H

#include <netinet/in.h>

class SocketFactory
{

public:
	int getTCPServerSocket( int domain = AF_INET, int port, int allowedConnNumber = 5 );

};


#endif
