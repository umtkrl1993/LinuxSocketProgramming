#ifndef _ISOCKFACTORY_H
#define _ISOCKFACTORY_H

#include "sockFactory.h"

class ISocketFactory : public SocketFactory
{

public:
	int getTCPSocket() const;
	int getRawTCPSocket() const;

};


#endif