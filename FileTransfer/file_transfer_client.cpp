#include <fstream>
#include <iostream>

using namespace std;

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <memory>


int main( int argc, char* argv[] ){

	struct sockaddr_in addr;
	addr.sin_addr.s_addr = inet_addr( "192.168.1.106" );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 9000 );

	char* buff = new char[24000];

	char* back_buff = buff;

	int socket_descriptor = socket( AF_INET, SOCK_STREAM, 0 );

	int return_val = connect( socket_descriptor, ( struct sockaddr*)&addr, sizeof( struct sockaddr_in ) );

	if( return_val == -1 ){
		cout<<"error"<<endl;
	}

	else{
		cout<<"connection"<<endl;
	}

	unsigned int read_size = 0;


	unsigned int size = 0;

	    while( true ) {
            size = recv(socket_descriptor, buff, 500, 0);

            if( size == -1 || size == 0 ) break;

            read_size += size;
            buff += size;

        }

	cout<<"read size " << read_size<<endl;

	ofstream myFile;
	myFile.open ("data.bin", ios::out | ios::out | ios::binary);
    myFile.write (back_buff, read_size);

	
}