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


static int _file_server_socket;

static int _createAndOpenFile( const char* filename ){

	int fd;
	if( ( fd = open( filename, O_WRONLY | O_CREAT | O_TRUNC, 0744 ) ) != -1 ){

			return fd;
	}

	return -1;
}

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
        fgets( filename, 50, stdin );

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

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
	
	char new_file[20] = "umit_read.txt";
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }

	_file_server_socket = sock;

    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     

	_handleAuthentication( sock );
	_startDataFlow( sock );
    //keep communicating with server
     
    close(sock);
    return 0;
}
