/*
    C socket server example
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include "socket.h"
#include <signal.h>
#include "defs.h"

static FILE* log_file;


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

	if( strcmp( response_from_dbserver, "correct") == 0 ){
		return_value = 0;
	}

	else if( strcmp( response_from_dbserver, "incorrect") == 0 ){
		return_value = 1;
	}

	else{
		return_value = 2;
	}

	close( database_socket );
	return return_value;

}


static int _createAndRedirectLogger( const char* filename ){

	if ( ( logger = open( filename, O_RDWR | O_CREAT | O_APPEND, 0644 ) ) == -1 ){

		return -1;
	}

		
	if( dup2( logger, 1 ) == -1 ){

		fflush( stdout );
		close( 1 );

		return -1;
	}

	return 0;




	//freopen( filename, "a+", stdout );

}


static void _createLogger( const char* plog_file ){

	log_file = fopen( plog_file, "a" );

}

static void _handleInterruptSignal( int signo ){

	if( signo == SIGINT ){
		printf("Closing files\n");
		close( logger );
		exit(0);
	}
}

//There is only one chance to authenticate for now
//In the future three right will be implemented
void* workerThread( void* arg ){

	int read_bytes;
	int client_socket = *( int* )arg;
	int is_authenticated = 1;
	char filename[MAX_FILENAME_LENGTH];
	char file[300];
	char username[20];
	char password[10];

	read_bytes = read( client_socket, username, 20 );
	username[read_bytes-1] = '\0';
	read_bytes = read( client_socket, password, 10 );
	password[read_bytes-1] = '\0';

	is_authenticated = _handleAuthentication( username, password );

	if( is_authenticated != 0 ){

		pthread_exit(0);
	}

	while( 1 ){

		 printf("Waiting to read data ...\n");

  		 int result = read(client_socket , filename ,MAX_FILENAME_LENGTH );

		if( result == 0 || result == -1 ){

			printf("-------------------Closing connection---------------------Result is %d\n", result);
			close( client_socket );
			pthread_exit( 0 ); 
		}

	     filename[result-1] = '\0';

	     printf( "Read from socket data %s and its size is %d\n", filename, strlen(filename)  );


		if( result == -1 ){
			send( client_socket, error_message_read_name, strlen( error_message_read_name ), 0 );

			pthread_exit( ( void* )1 );
		}


		printf( "Requested file is %s\n", filename );

	
		int fd = open( filename, O_RDONLY );

		if( fd == -1 ){

			char* errorInfo = strerror( errno );
			send( client_socket, errorInfo, strlen( errorInfo ), 0 );
			continue;
		}

		if( fd == -1 ) {
			perror( "ERROR: " );
			send( client_socket, error_message_open_file, strlen( error_message_open_file), 0 );
			continue;
		}

		int bytes;
		if( ( bytes = read( fd, file, 300 ) ) != -1 ) {

			file[bytes] = '\0';
			send( client_socket, file, strlen( file ), 0 );		
		}

		else{
			
			send( client_socket, file, strlen( file ), 0 );
			
		}
		
	}

	pthread_exit( ( void*) 0 );

}

void* loggerThread( void* args ){

	FILE* logger_desc = fdopen( logger, "a" );
	FILE* thread_logger = fopen( "threadLogger.txt", "w");

	while( 1 ){

		int ret = fflush( logger_desc );

		fprintf( thread_logger, "fflush return value %d\n", ret );

		fsync( logger );

		fflush( thread_logger );

		sleep(4);


	}

}

int main(int argc , char *argv[])
{
    int  client_sock , c , read_size;
	socket_desc server_socket;
    struct sockaddr_in server , client;
    char* client_message = (char*)malloc( sizeof(char)*2000);

    signal( SIGINT, _handleInterruptSignal );
    memset( client_message, 0, 2000 );
     
	server_socket = openTCPSocket();	
	
	bindSocket( server_socket, &server, 1100 );


	int ret = _createAndRedirectLogger( "connectionLog.txt" );

	printf( "return value from %d\n\n", ret);

    printf( "Binding is done \n");

    fflush(stdout);
    fsync(1);
   
    //Listen
    listen(server_socket , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");

    fsync(logger);
   
    c = sizeof(struct sockaddr_in);


 	pthread_t loggerid;

 	pthread_create( &loggerid, NULL, loggerThread, NULL );
	while( 1 ){    
    //accept connection from an incoming client

    client_sock = accept( server_socket, (struct sockaddr *)&client, (socklen_t*)&c);

    printf("Connection established \n");
    FILE* logger_desc = fdopen( logger, "a" );
    fflush( logger_desc );
    fsync( logger );


	

	pthread_t id;	

	pthread_create( &id, NULL, workerThread, &client_sock );
	//pthread_join( id, NULL );

	}

	 close(logger);

    return 0;
}
