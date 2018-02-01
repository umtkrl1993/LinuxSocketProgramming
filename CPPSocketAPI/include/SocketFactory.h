#ifndef _SOCKFACTORY_H
#define _SOCKETFACTORY_H

class SocketFactory{

public:
	virtual int getTCPSocket() const = 0;
	virtual int getRawTCPSocket() const = 0;

};


#endif