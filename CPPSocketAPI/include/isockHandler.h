#ifndef _ISOCKHANDLER_H
#define _ISOCKHANDLER_H
#include "sockHandler.h"



class ISocketHandler : public SocketHandler{

	public:
		ISocketHandler( const ISocketHandler& handler );
		ISocketHandler& operator=( const ISocketHandler& handler );
		ISocketHandler( int socket_descriptor );
		ISocketHandler( int socket_desciptor, char* ip, int port );
		~ISocketHandler();
		int bindSocket() const;

		int startListen( int backlog ) const;

		const char* getSockIP() const { return mp_ip_address; }
		int getSockPort() const { return  m_port ; }
		int getSockDesc() const { return m_socket_descriptor; }


	private:
		char* mp_ip_address;
		int m_port;
		int m_socket_descriptor;


};

#endif