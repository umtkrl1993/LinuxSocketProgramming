/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include "socket.h"


static int _file_server_socket;

static int _createAndOpenFile( const char* filename ){

	int fd;
	if( ( fd = open( filename, O_WRONLY | O_CREAT | O_TRUNC, 0644 ) ) != -1 ){

			return fd;
	}

	return -1;
}


//gets username and password from command line
static void _handleAuthentication( int socket ){

	char username[20];
	char password[10];
	int error = -1;


	while( error == -1 ){
		printf("Please enter username: " );
		fgets( username, 20, stdin );

		if( ( error = send( socket, username, strlen( username) , 0 ) ) == -1 ){
	
			perror("Please try again error sending username ");
			continue;
		}

		printf("Please enter password: " );
		fgets( password, 10, stdin );

		if( ( error = send( socket, password, strlen( password ), 0 ) ) == -1 ){

			perror("Please enter password again error sending password ");
			continue;
		}
	}

}

static void _startDataFlow( int sock ){

	char filename[50];
	char filecontent[2000];

    while(1)
    {

        printf("Enter filename : ");
        fgets( filename, 100, stdin );

		int fd = _createAndOpenFile( "send_from_server" );

        //Send some data
        if( send(sock , filename , strlen(filename) , 0) < 0)
        {
            puts("Send failed");
			exit( -1 );
        }
         
        //Receive a reply from the server
		int read_size;
        if(  ( read_size = recv(sock , filecontent , 2000 , 0) ) < 0)
        {
            puts("recv failed");
            break;
        }

		filecontent[read_size] = '\0';

		printf( "Received file is %s\n", filecontent );
		
		write( fd, filecontent, strlen( filecontent ) );

		close( fd );

    }

}
 

void handleInterruptSignal( int signo ){

	if( signo == SIGINT ){
		close( _file_server_socket );
	}

}

/*
static void _connectToRemoteServer( const char* args[] ){


}
*/


//arg[1] is host ip
//arg[2] is port number
int main(int argc , char *argv[])
{


	if( argc != 3 ){

		printf("You have to provide host address and port number !!!");
		exit( 0 );
	}

	int port = atoi( argv[2] );

	if( port == 0 ){

		printf("No valid port number");
		exit( 0 );
	}

	socket_desc sock;
    struct sockaddr_in server;
	
	char new_file[20] = "umit_read.txt";
     
    //Create socket
	
	sock = openTCPSocket();
    if (sock == -1)
    {
        printf("Could not create socket");
		exit(1);
    }

	int return_val = connectToServer( sock, &server, argv[1], port );

	if( return_val != 0 )
	{
		
		printf("Could not connect to remote server");
		exit(1);
	}

	_file_server_socket = sock;
	_handleAuthentication( sock );
	_startDataFlow( sock );
    //keep communicating with server
     
    close(sock);
    return 0;
}
