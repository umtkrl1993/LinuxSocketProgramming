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


#define FILENAME_SIZE 50
#define BUFFER_SIZE 2000
#define DATABASE_PORT 1194
#define DATABASE_HOST "127.0.0.1"

const char* error_message_read_name = "Could not read file name";
const char* error_message_open_file = "Could not open the file";
const char* error_message_read_file = "Could not read the file";
const char* user_name_question = "Please enter username :";
const char* password_question = " Please enter password :";

FILE* logger = NULL;

static const int DEFAULT_SOCK = 1100;



static int _redirectStdout( const char* filename ){

	int fd = open( filename, O_WRONLY | O_CREAT | O_TRUNC, 0644 );

	if( fd != -1 ){

		if( dup2( fd, 1 ) != -1 ){

			logger = fdopen( fd, "w" );
			return 0;
		}
	}

	return -1;

}

void sig_child( int signo ){

	printf( "client terminated\n" );
	pid_t pid;
	int status;

	while( ( pid = waitpid( -1, &status, WNOHANG)) > 0 ){
		printf( "Fetched a childs exit status with pid %d\n", pid );
	}
}



int main(int argc , char *argv[])
{
    int  client_sock , c , read_size;
	socket_desc server_socket;
    struct sockaddr_in server , client;

    memset( &server, 0, sizeof( struct sockaddr_in ) );
	memset( &client, 0, sizeof( struct sockaddr_in ) );

	signal(SIGCHLD, sig_child);

	_redirectStdout( "connectionLogger.txt" );

	server_socket = openTCPSocket();	
	
	bindSocket( server_socket, &server, 1100 );

    puts("bind done");
	fflush( logger );
     
    //Listen
    listen(server_socket , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
	fflush( logger );
    c = sizeof(struct sockaddr_in);
 
	while( 1 ){    
    //accept connection from an incoming client
    client_sock = accept( server_socket, (struct sockaddr *)&client, (socklen_t*)&c);

    printf("Connection is established\n");
	fflush( logger );

	pid_t pid;

	if( ( pid = fork() ) < 0 ){
	
		printf( "SYSTEM ERROR!!\n");

	}

	else if( pid == 0 ){

		close( server_socket );
		workerProcess( client_sock );
		exit( 0 );
	}

		close( client_sock );

	}

    return 0;
}
