#ifndef _SOCKHANDLER_H
#define _SOCKHANDLER_H

class sockaddr_in;

class SocketHandler{

	virtual int bindSocket() const = 0;
	virtual int startListen( int backlog ) const = 0;
	virtual int acceptConnection( struct sockaddr_in& ) const = 0;

};


#endif