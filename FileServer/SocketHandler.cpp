#include "SocketHandler.h"
#include <sys/socket.h>



int SocketHandler::openSocket() {

	
	int socket_desc = socket( AF_INET, SOCK_STREAM, 0 );

	return socket_desc;

}
