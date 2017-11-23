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


#define FILENAME_SIZE 50
#define BUFFER_SIZE 2000


const char* error_message_read_name = "Could not read file name";
const char* error_message_open_file = "Could not open the file";
const char* error_message_read_file = "Could not read the file";

void* workerThread( void* arg ){

	int client_socket = ( int* )arg;

	char filename[FILENAME_SIZE];

    int result = read(client_socket , filename ,FILENAME_SIZE );

	if( result == -1 ){
		send( client_socket, error_message_read_name, strlen( error_message_read_name ), 0 );

		pthread_exit( ( void* )1 );
	}

	char file[BUFFER_SIZE];
	int fd;

	if( ( fd = open( filename, O_RDONLY ) ) != -1 ){

		if( read( fd, file, BUFFER_SIZE ) != -1 ){

			send( client_socket, file, strlen( file ), 0 );		
		}

		else{

			pthread_exit( ( void* )1 );
		}		

	} 

	else{

		send( client_socket, error_message_open_file, strlen( error_message_open_file ), 0 ); 
		pthread_exit( ( void* ) 1 );
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
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

	int id;	

	pthread_create( &id, NULL, workerThread, &client_sock );

    return 0;
}
