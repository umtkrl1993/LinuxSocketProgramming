#ifndef _ISOCKFACTORY_H
#define _ISOCKFACTORY_H

#include "SocketFactory.h"

class ISocketFactory : public SocketFactory
{

public:
	ISocketFactory(){
		
	}
	int getTCPSocket() const;
	int getRawTCPSocket() const;

};


#endif