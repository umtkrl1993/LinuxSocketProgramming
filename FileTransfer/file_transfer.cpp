#include <fstream>
#include <iostream>
#include <dirent.h>

using namespace std;

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include "TCPSocketHandler.h"
#include <memory>

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_BLOCK_SIZE 500

bool isDirectory( const char* filename ){

    struct stat buf;

    if( lstat( filename, &buf ) < 0 ){
        //handle error
    }


     S_ISDIR( buf.st_mode );
}


int getFileSize( const char* filename ){

    struct stat buf;

    if( lstat( filename, &buf ) < 0 ){
        // handle error
    }

    if( !S_ISREG( buf.st_mode )  ){
        //handle error
    }

    else if( !S_ISDIR( buf.st_mode) ){

    }

    return buf.st_size;

}

bool checkFileReadAccess( const char* filename ){

    if( access( filename, R_OK ) < 0 )
        return false;

    return true;
}

static unique_ptr<TCPSocketHandler> openSocket( struct sockaddr_in& bind_address, int& socket_desc ){


    unique_ptr<TCPSocketHandler> socket_handler( new TCPSocketHandler() );

    socket_desc = socket_handler->openSocket();

    socket_handler->bindSocket( &bind_address, 9001 );
    socket_handler->listenSocket( 5 );

    return socket_handler;
}

/**
 *
 *
 * @param dir_path shouldn't include '/' at the end
 * @return
 */
static char* getDirName( const char* dir_path ){

    int len = strlen( dir_path );

    int index = len - 1;

    while( dir_path[index] != '/' )
        index--;

    int dir_name_size = len - index;

    char* dir_name = (char*)malloc( dir_name_size + 1 );

    index++;
    int counter = 0;
    while( index < len ){
        dir_name[counter++] = dir_path[index];
        index++;
    }

    dir_name[counter] = '\0';

    return dir_name;


}


/**
 *
 * @param conn_id
 * @param dir_path folder name etc if requested folder /home/umit/cpp, dir_path is cpp
 */
void sendDirectoryContent( int conn_id, const char* dir_path ){


    DIR* dp;
    struct dirent* dent;
    struct stat buf;
    char file_name_buff[256];

    char dir_message[] = "directory";
    char file_message[] = "file";

    char current_path[50];

    getcwd(current_path, 50);

    ofstream log_file( "directory.log");



    if( ( dp = opendir( dir_path ) ) != NULL ){
        while( ( dent = readdir( dp ) ) != NULL ){
            if( strcmp( dent->d_name, "." ) == 0 || strcmp( dent->d_name, ".." ) == 0 )
                continue;

            int bytes = snprintf( file_name_buff, 256,"%s%c%s", dir_path, '/', dent->d_name );

            if( isDirectory( file_name_buff ) ){

       //         printf("before copying directory\n");



                printf("Directory name %s\n", file_name_buff);
                send( conn_id, dir_message, strlen( &dir_message[0] ), 0 );

                send( conn_id, file_name_buff, strlen( &file_name_buff[0] ), 0 );

                sendDirectoryContent( conn_id, file_name_buff );

            }

            // if regular file
            else{
           //     send( conn_id, file_message, strlen( dir_message ), 0 );
           //     send( conn_id, file_name_buff, strlen( dir_name_buff ), 0 );

                printf("Filename %s\n", file_name_buff );

                //send_content

            }


        }

    }


}




int main( int argc, char* argv[] ){

    char* dir = argv[1];
//
//    char* dir_name = getDirName(dir);
//
//    printf( "%s\n", dir_name );



    //int size = myfile.gcount();

    unsigned int size = getFileSize("binary");

    cout<<"size is " << size<<endl;


	ifstream myfile("binary", ios::in | ios::binary );
	char* buffer = new char[24000];
	myfile.read( buffer, 24000 );

	char* back_buffer = buffer;

	struct sockaddr_in bind_address;
	int sock_addr_size = sizeof(struct sockaddr_in);

	struct sockaddr_in* client_addr;
	int reuse = 1;
	int socket_desc;

	unique_ptr<TCPSocketHandler> socket_handler = openSocket( bind_address, socket_desc );

	setsockopt( socket_desc, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int) );

	int conn_id = accept( socket_desc, (struct sockaddr *)&client_addr, (socklen_t*)&sock_addr_size );

	cout<<"connection estab\n";

	char filename[256];

	int read_size = recv( conn_id, filename, 255, 0 );

	if( read_size == -1 ){
	    //error
	}

	printf("readed file size %d\n", read_size );

	filename[read_size-2] = '\0';

	printf("requested filename is %s\n", filename );

	if( isDirectory(filename) ){

	    cout<<"Requesting directory\n";
	    chdir( dir );
	    chdir("..");
        char* dir_name = NULL;
        dir_name = getDirName( filename );
        sendDirectoryContent( conn_id, filename );
        free( dir_name );
	}

	else{


	}






//	unsigned int size_send = size % FILE_BLOCK_SIZE;
//
//	if( size_send != 0 ){
//        send(conn_id, buffer, size_send, 0);
//        buffer = buffer + size_send;
//	}
//
//
//
//	while( size_send < size ) {
//        send(conn_id, buffer, FILE_BLOCK_SIZE, 0);
//        buffer = buffer + FILE_BLOCK_SIZE;
//        size_send += FILE_BLOCK_SIZE;
//    }
//
//	ofstream outFile;
//	outFile.open("data.bin", ios::out | ios::binary );
//
//	outFile.write( back_buffer, size_send );
//
//	outFile.close();

	close(conn_id);



}