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


#define FILENAME_SIZE 50
#define BUFFER_SIZE 2000


const char* error_message_read_name = "Could not read file name";
const char* error_message_open_file = "Could not open the file";
const char* error_message_read_file = "Could not read the file";
const char* user_name_question = "Please enter username :";
const char* password_question = " Please enter password :";

/**

returns 0 if successful 
return -1 if it is not successful

*/
int getCredentials( int socket ){

	char username[20];
	char password[10];

	send( socket, user_name_question, strlen( user_name_question ), 0 );

	int read_bytes = read( socket, username, strlen( username ) );

	username[read_bytes-1] = '\0';

	send( socket, password_question, strlen( password_question ), 0 );

	read_bytes = read( socket, password, strlen( password ) );

	password[read_bytes-1] = '\0';

	printf("Username is %s and password is %s \n", username, password );

}

void* workerThread( void* arg ){

	int read_bytes;
	int client_socket = *( int* )arg;
	char filename[FILENAME_SIZE];
	char file[300];
	char user_name[20];
	char password[10];

	int login_result = getCredentials( client_socket );

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

		if( fd == -1 ) {
			perror( "ERROR: " );
			send( client_socket, error_message_open_file, strlen( error_message_open_file), 0 );
			continue;
		}

		int bytes;
		if( ( bytes = read( fd, file, 300 ) ) != -1 ) {

			file[bytes-1] = '\0';
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
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char* client_message = (char*)malloc( sizeof(char)*2000);

    memset( client_message, 0, 2000 );
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
 
	while( 1 ){    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

	puts("Connection accepted");

	pthread_t id;	

	pthread_create( &id, NULL, workerThread, &client_sock );
	//pthread_join( id, NULL );

	}

    return 0;
}
