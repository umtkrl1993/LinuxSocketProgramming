#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
 
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>


#define TOTAL_PACKET_LIMIT 4096
#define PACKET_BATCH_SIZE 32
#define PACKET_SIZE 65536
#define ETH_ADDR_LEN 6

static FILE* log_file;

struct packet_info{

    unsigned char source_mac[ETH_ADDR_LEN];
    unsigned char dst_mac[ETH_ADDR_LEN];
    unsigned int source_ip;
    unsigned int dst_ip;

};

static void processEthernetHeader( const char* packet, struct packet_info* processed_packet );
static void processIPHeader( const char* packet, struct packet_info* processed_packet );



static int openSnifferSocket(){

	int sniffer_socket = socket( AF_PACKET , SOCK_RAW , htons( ETH_P_ALL ) ) ;

	if( sniffer_socket == -1 ){

		perror( " Could not open sniffer socket: ");
		exit( -1 );
	}

	return sniffer_socket;
}

static void processPacketBatch( const unsigned char** packet_batch, struct packet_info* processed_packet ,int batch_size ){

	for( int i = 0; i < batch_size; i++ ){

		
		 //Get ethernet header info
		 processEthernetHeader( packet_batch[i], &processed_packet[i] );
		 processIPHeader( packet_batch[i], &processed_packet[i] );
	
	}
}


static void processEthernetHeader( const char* packet, struct packet_info* processed_packet ){

	struct ethhdr *eth = ( struct ethhdr * )packet;

	//Get source mac address
	processed_packet->source_mac[0] = eth->h_source[0];
	processed_packet->source_mac[1] = eth->h_source[1];
	processed_packet->source_mac[2] = eth->h_source[2];
	processed_packet->source_mac[3] = eth->h_source[3];
	processed_packet->source_mac[4] = eth->h_source[4];
	processed_packet->source_mac[5] = eth->h_source[5];

	//Get destination mac address
	processed_packet->dst_mac[0] = eth->h_dest[0];
	processed_packet->dst_mac[1] = eth->h_dest[1];
	processed_packet->dst_mac[2] = eth->h_dest[2];
	processed_packet->dst_mac[3] = eth->h_dest[3];
	processed_packet->dst_mac[4] = eth->h_dest[4];
	processed_packet->dst_mac[5] = eth->h_dest[5];

}

static void processIPHeader( const char* packet, struct packet_info* processed_packet ){

	 struct iphdr *iph = ( struct iphdr* )( packet + sizeof( struct ethhdr ) );


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

	
	unsigned char* buffer = malloc( sizeof( unsigned char ) * PACKET_SIZE );

	// Initialization to capture packets

	log_file = fopen( "packet_logfile.txt", "w" );
	sniffer_socket = openSnifferSocket();

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

	processed_packet = ( struct packet_info * ) malloc( sizeof( struct packet_info ) * PACKET_BATCH_SIZE );


	socketaddr_size = sizeof( struct sockaddr );

	while( total_packet_counter < TOTAL_PACKET_LIMIT ){

		data_size = recvfrom( sniffer_socket ,packet_buffer[packet_index_counter] ,PACKET_SIZE ,0 ,&saddr ,(socklen_t*)&socketaddr_size );


		packet_index_counter++;
		total_packet_counter++;

		if( packet_index_counter+1 == PACKET_BATCH_SIZE ){
		 	processPacketBatch( packet_buffer, processed_packet ,PACKET_BATCH_SIZE );
		 	packet_index_counter = 0;
		}

	}




}










