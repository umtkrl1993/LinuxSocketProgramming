#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>

#include <stdlib.h>    //malloc
#include <string.h>    //strlen
#include <sys/socket.h>
#include <netinet/if_ether.h>  //For ETH_P_ALL

#include <sys/time.h>
#include "processorUtilities.h"
#include <pthread.h>
#include <time.h>

#include "aes256.h"


#define TOTAL_PACKET_LIMIT 16384
#define PACKET_BATCH_SIZE 16384
#define PACKET_SIZE 65536
#define THREAD_NUMBER 16
static struct packet_info* processed_packet;
static unsigned char** packet_buffer;
static int thread_index_counter = -1;
static FILE* log_file;


pthread_mutex_t packet_lock;

struct thread_args{

	int thread_index;
	unsigned char* packet_bufer;
	struct packet_info* processed_packet;
};




static int openSnifferSocket(){

	int sniffer_socket = socket( AF_PACKET , SOCK_RAW , htons( ETH_P_ALL ) ) ;

	if( sniffer_socket == -1 ){

		perror( " Could not open sniffer socket: ");
		exit( -1 );
	}

	return sniffer_socket;
}

static void encryptPacket( struct packet_info* processed_packet ){

	uint8_t key[32];
	for ( int i = 0; i < sizeof(key);i++) key[i] = i;

	aes256_context ctx; 

	aes256_init(&ctx, key);

	aes256_encrypt_ecb( &ctx, ( processed_packet->source_mac ) );
	aes256_encrypt_ecb( &ctx, ( processed_packet->dst_mac ) );


}

void* packetProcessorThread( void* arg ){

	int packet_limit_per_thread = PACKET_BATCH_SIZE / THREAD_NUMBER;
	int index = 0;
	for( int i = 0; i < packet_limit_per_thread; i++ ){
		pthread_mutex_lock( &packet_lock );
		thread_index_counter++;
		index = thread_index_counter;

		pthread_mutex_unlock( &packet_lock );

		processEthernetHeader( packet_buffer[index], &processed_packet[index] );
		processIPHeader( packet_buffer[index], &processed_packet[index] );
		encryptPacket( &processed_packet[index]);


	}

	pthread_exit( 0 );

}

static void startProcessorThreads( unsigned char** packet_buffer, struct packet_info* processed_packet ){

	pthread_t ids[THREAD_NUMBER];

	for( int i = 0; i < THREAD_NUMBER; i++ ){
		pthread_create( &ids[i], NULL, packetProcessorThread, NULL );
	}

	for( int i = 0; i < THREAD_NUMBER; i++ ){
		pthread_join( ids[i], NULL );
	}
	

}



int main( int argc, char* argv[] ){

	struct sockaddr saddr;

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

	pthread_mutex_init( &packet_lock, NULL );
	clock_t start, end;


	while( total_packet_counter < TOTAL_PACKET_LIMIT ){

		data_size = recvfrom( sniffer_socket ,packet_buffer[packet_index_counter] ,PACKET_SIZE ,0 ,&saddr ,(socklen_t*)&socketaddr_size );


		packet_index_counter++;
		total_packet_counter++;

		if( packet_index_counter+1 == PACKET_BATCH_SIZE ){
			start = clock();
			startProcessorThreads( packet_buffer, processed_packet );
			logProcessedPackets( processed_packet, PACKET_BATCH_SIZE, log_file );
			thread_index_counter = -1;
		 	packet_index_counter = 0;

		}

	}

	end = clock();

	printf( " ------------ CPU Execution Time is %f -----------------------\n", ( ( double )( end - start ) )/ CLOCKS_PER_SEC );


	/**
		Deallocation is actually redundant here because program is exiting so allocated memories will be released automatically anyway.But it is always good to carry out deallocation.

	*/

	//deallocate memories for packet buffer and processed packets buffer
	for( int i = 0 ; i < PACKET_BATCH_SIZE; i++ ){
		free( packet_buffer[i] );
	}

	free( packet_buffer );

	free( processed_packet );



}










