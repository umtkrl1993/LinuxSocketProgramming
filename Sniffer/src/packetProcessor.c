#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#include <stdlib.h>    //malloc
#include <string.h>    //strlen
#include <sys/socket.h>
#include <netinet/if_ether.h>  //For ETH_P_ALL

#include <sys/time.h>
#include "processorUtilities.h"


#define TOTAL_PACKET_LIMIT 4096
#define PACKET_BATCH_SIZE 32
#define PACKET_SIZE 65536


static FILE* log_file;


static int openSnifferSocket(){

	int sniffer_socket = socket( AF_PACKET , SOCK_RAW , htons( ETH_P_ALL ) ) ;

	if( sniffer_socket == -1 ){

		perror( " Could not open sniffer socket: ");
		exit( -1 );
	}

	return sniffer_socket;
}



int main( int argc, char* argv[] ){

	struct sockaddr saddr;
	struct packet_info* processed_packet;
	unsigned char** packet_buffer;
	int sniffer_socket;
	int total_packet_counter = 0;
	int data_size;
	int socketaddr_size;
	int packet_index_counter = 0;

	
	// Initialization to capture packets

	log_file = fopen( "packet_logfile.txt", "w" );
	sniffer_socket = openSnifferSocket();


	// Starting to do necessary memory allocations

	//Allocating a buffer for the captured packets
	packet_buffer = ( unsigned char** )malloc( sizeof( unsigned char* ) * PACKET_BATCH_SIZE );

	if( packet_buffer == NULL ){
		perror("Error allocation memory: ");
		exit( -1 );
	}

	for( int counter = 0; counter < PACKET_BATCH_SIZE; counter++ ){
		packet_buffer[counter] = ( unsigned char* ) malloc( sizeof( unsigned char ) * PACKET_SIZE );
		if( packet_buffer[counter] == NULL ){
			perror("Error allocating memory: " );
			exit(-1);
		}
	}

	//Allocating memory for the processed packets
	processed_packet = ( struct packet_info * ) malloc( sizeof( struct packet_info ) * PACKET_BATCH_SIZE );


	socketaddr_size = sizeof( struct sockaddr );

	/**
	Start to capture network packets. Captured packets are not processed immediately since packets are buffered until they reach the predefined size limit.
	As soon as packet buffer is full, packets are sent to processPacketBatch method.
	*/

	while( total_packet_counter < TOTAL_PACKET_LIMIT ){

		data_size = recvfrom( sniffer_socket ,packet_buffer[packet_index_counter] ,PACKET_SIZE ,0 ,&saddr ,(socklen_t*)&socketaddr_size );


		packet_index_counter++;
		total_packet_counter++;

		if( packet_index_counter+1 == PACKET_BATCH_SIZE ){
		 	processPacketBatch( packet_buffer, processed_packet ,PACKET_BATCH_SIZE );
		 	logProcessedPackets( processed_packet, PACKET_BATCH_SIZE, log_file );
		 	packet_index_counter = 0;
		}

	}

}










