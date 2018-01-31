#ifndef _SERVER_H
#define _SERVER_H


class ISocketHandler{

	public:
		ISocketHandler( int socket_descriptor );
		ISocketHandler( int socket_desciptor, char* ip, int port );
		~ISocketHandler();
		int bindSocket() const;

		int startListen() const;


	private:
		char* mp_ip_address;
		int m_port;
		int m_socket_descriptor;


};


#endif
