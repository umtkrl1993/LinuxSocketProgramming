/*
    C socket server example
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdlib.h>
#include<pthread.h>
#include<fcntl.h>
#include<errno.h>
#include"socket.h"


#define FILENAME_SIZE 50
#define BUFFER_SIZE 2000
#define DATABASE_PORT 1194
#define DATABASE_HOST "127.0.0.1"

const char* error_message_read_name = "Could not read file name";
const char* error_message_open_file = "Could not open the file";
const char* error_message_read_file = "Could not read the file";
const char* user_name_question = "Please enter username :";
const char* password_question = " Please enter password :";



static int _handleAuthentication( const char* username, const char* password ){

	char string_to_be_send[51];
	char response_from_dbserver[30];
	int return_value = 0;
	socket_desc database_socket = openTCPSocket();
	connectToServer( database_socket, DATABASE_HOST, DATABASE_PORT );
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

//There is only one chance to authenticate for now
//In the future three right will be implemented
void* workerThread( void* arg ){

	int read_bytes;
	int client_socket = *( int* )arg;
	int is_authenticated = 1;
	char filename[FILENAME_SIZE];
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

  		 int result = read(client_socket , filename ,FILENAME_SIZE );

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

int main(int argc , char *argv[])
{
    int  client_sock , c , read_size;
	socket_desc server_socket;
    struct sockaddr_in server , client;
    char* client_message = (char*)malloc( sizeof(char)*2000);

    memset( client_message, 0, 2000 );
     
	server_socket = openTCPSocket();	
	
	bindSocket( server_socket, 8888 );

    puts("bind done");
     
    //Listen
    listen(server_socket , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
 
	while( 1 ){    
    //accept connection from an incoming client
    client_sock = accept( server_socket, (struct sockaddr *)&client, (socklen_t*)&c);

	puts("Connection accepted");

	pthread_t id;	

	pthread_create( &id, NULL, workerThread, &client_sock );
	//pthread_join( id, NULL );

	}

    return 0;
}
