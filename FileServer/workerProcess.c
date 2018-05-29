

#include <unistd.h>    //write
#include <errno.h>
#include <fcntl.h>
#include "socket.h"

#define FILENAME_SIZE 50
#define BUFFER_SIZE 2000
#define DATABASE_PORT 1194
#define DATABASE_HOST "127.0.0.1"


static int _handleAuthentication( const char* username, const char* password ){


	char string_to_be_send[51];
	char response_from_dbserver[30];
	int return_value = 0;
	struct sockaddr_in db_server;
	socket_desc database_socket = openTCPSocket();
	connectToServer( database_socket, &db_server, DATABASE_HOST, DATABASE_PORT );
	int ret = snprintf( string_to_be_send, sizeof( string_to_be_send ), "%s%c%s", username, ' ', password );
	string_to_be_send[ret] = '\0';

	send( database_socket, string_to_be_send, strlen( string_to_be_send ), 0 );

	int read_bytes = read( database_socket, response_from_dbserver, 30 );
	response_from_dbserver[read_bytes] = '\0';

	if( strcmp( response_from_dbserver, "correct" ) == 0 ){
		return_value = 0;
	}

	else if( strcmp( response_from_dbserver, "incorrect" ) == 0 ){
		return_value = 1;
	}

	else{
		return_value = 2;
	}

	close( database_socket );
	return return_value;

}


//There is only one chance to authenticate for now
//In the future three right will be implemented
void* workerProcess( int sock ){


	pid_t pid = getpid();
	printf( "Worker process has started with pid %d\n", pid );
	int read_bytes;
	int client_socket = sock;
	int is_authenticated = 1;
	char filename[FILENAME_SIZE];
	char file[300];
	char username[20];
	char password[10];

	read_bytes = read( client_socket, username, 20 );


	if( read_bytes == 0 ){
		exit(0);
	}

	username[read_bytes-1] = '\0';
	read_bytes = read( client_socket, password, 10 );
	if( read_bytes == 0 ){
		exit(0);
	}
	password[read_bytes-1] = '\0';

	is_authenticated = _handleAuthentication( username, password );

	if( is_authenticated != 0 ){

		exit(0);
	}

	while( 1 ){

	 int result = read( client_socket , filename ,FILENAME_SIZE );

		if( result == 0 || result == -1 ){

			printf("-------------------Closing connection---------------------Result is %d\n", result);
			close( client_socket );
			exit( 0 );
		}

	     filename[result-1] = '\0';

	     printf( "Read from socket data %s and its size is %d\n", filename, strlen(filename)  );


		if( result == -1 ){
			send( client_socket, error_message_read_name, strlen( error_message_read_name ), 0 );

			exit(-1);
		}


		printf( "Requested file is %s\n", filename );


		int fd = open( filename, O_RDONLY );

		if( fd == -1 ){

			char* errorInfo = strerror( errno );
			send( client_socket, errorInfo, strlen( errorInfo ), 0 );
			continue;
		}

		int bytes;

		// Starting to send the file

		while( ( bytes = read( fd, file, 300 ) ) != -1 ) {

			file[bytes] = '\0';
			send( client_socket, file, strlen( file ), 0 );


		}

	}

}
