/*
    C ECHO client example using sockets
*/
#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <unistd.h>
#include <fcntl.h>


int createAndOpenFile( const char* filename ){

	int fd;
	if( ( fd = open( filename, O_WRONLY | O_CREAT | O_TRUNC ) ) != -1 ){

			return fd;
	}

	return -1;

}
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char file_name[50] , file_content[2000];
	
	char new_file[20] = "umit_read.txt";
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
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
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
        printf("Enter filename : ");
        //scanf("%s\n" , message);
        fgets( file_name, 50, stdin );

		int fd = createAndOpenFile( "send_from_server" );
		
        //Send some data
        if( send(sock , file_name , strlen(file_name) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
         
        //Receive a reply from the server
		int read_size;
        if(  ( read_size = recv(sock , file_content , 2000 , 0) ) < 0)
        {
            puts("recv failed");
            break;
        }

		printf( "Received file is %s\n", file_content );
		
		
		write( fd, file_content, strlen( file_content ) );

		close( fd );

         
    }
     
    close(sock);
    return 0;
}
