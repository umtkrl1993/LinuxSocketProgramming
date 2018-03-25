
#include <cuda_runtime.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <stdlib.h>
#include "raw_socket.h"
#include <pthread.h>

#define ETH_ADDR_LEN 6
#define IP_ADDR_LEN 4
#define THREAD_NUMBER_X 32
#define PACKET_SIZE  65536

struct packet_info{

    unsigned char source_mac[ETH_ADDR_LEN];
    unsigned char dst_mac[ETH_ADDR_LEN];
    unsigned int source_ip;
    unsigned int dst_ip;

};


static int openSnifferSocket(){

    int sniffer_socket = socket( AF_PACKET , SOCK_RAW , htons( ETH_P_ALL ) ) ;

    if( sniffer_socket == -1 ){

        perror( " Could not open sniffer socket: ");
        exit( -1 );
    }

    return sniffer_socket;
}


__device__ void resolveEthernetHeader( unsigned char* packet_buffer, struct packet_info* info )
{
   
    struct ethhdr *eth = ( struct ethhdr *)packet_buffer;

    info->dst_mac[0] = eth->h_dest[0];
    info->dst_mac[1] = eth->h_dest[1];
    info->dst_mac[2] = eth->h_dest[2];
    info->dst_mac[3] = eth->h_dest[3];
    info->dst_mac[4] = eth->h_dest[4];
    info->dst_mac[5] = eth->h_dest[5];

    printf(" GPU Resolved  |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", info->dst_mac[0] , info->dst_mac[1] , info->dst_mac[2] , info->dst_mac[3] , info->dst_mac[4] , info->dst_mac[5] );
}

__global__ void processPacket( unsigned char* packet_buffer, int size, struct packet_info* info ){

    unsigned int thread_index = threadIdx.x;
    printf( "Thread with id %d \n", thread_index );
    packet_buffer = packet_buffer +  threadIdx.x * size;
    //resolveEthernetHeader( packet_buffer, info );

    struct ethhdr *eth = ( struct ethhdr *)packet_buffer;

       info->dst_mac[0] = eth->h_dest[0];
       info->dst_mac[1] = eth->h_dest[1];
       info->dst_mac[2] = eth->h_dest[2];
       info->dst_mac[3] = eth->h_dest[3];
       info->dst_mac[4] = eth->h_dest[4];
       info->dst_mac[5] = eth->h_dest[5];

       printf(" GPU Resolved  |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", info->dst_mac[0] , info->dst_mac[1] , info->dst_mac[2] , info->dst_mac[3] , info->dst_mac[4] , info->dst_mac[5] );

}


static void printHeaderOnCPU( unsigned char* packet_buffer ){

	unsigned char* initial_address = packet_buffer;

	for( int i = 0; i < 31 ; i++ ){

		packet_buffer = packet_buffer + i * PACKET_SIZE;
		struct ethhdr *eth = ( struct ethhdr *)packet_buffer;


		printf( "Ethernet Header - %d -\n", i);
		printf( "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
		printf( "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
		packet_buffer = initial_address;

	}

	fflush( stdout );

}

int main( int argc, char* argv[] ){

    int sockaddr_size, data_size, sniffer_socket;
    int packet_counter = 0;
    int packet_index_counter = 0;
    struct sockaddr saddr; 
    unsigned char* packet_buffer;
    unsigned char* packet_buffer_device;
    struct packet_info* info_device;
    struct packet_info* info;

    packet_buffer = ( unsigned char* )malloc( sizeof( unsigned char ) * THREAD_NUMBER_X * PACKET_SIZE ) ;
    info = ( struct packet_info* )malloc( THREAD_NUMBER_X * sizeof( struct packet_info ) );
    cudaMalloc( (void**) &info_device, THREAD_NUMBER_X * sizeof( struct packet_info ) );
    cudaMalloc( (void**) &packet_buffer_device, THREAD_NUMBER_X * PACKET_SIZE * sizeof( unsigned char ) );


    sniffer_socket = openSnifferSocket();
    dim3 block(THREAD_NUMBER_X);
    dim3 grid(1);

    unsigned char* initial_address = packet_buffer;

    unsigned char* dummy = ( unsigned char* )malloc( sizeof( unsigned char ) * PACKET_SIZE ) ;
//
    while(1){
        data_size = recvfrom( sniffer_socket, packet_buffer, PACKET_SIZE, 0, &saddr, (socklen_t*)&sockaddr_size );


        packet_counter++;
        packet_index_counter++;
        packet_buffer = packet_buffer + PACKET_SIZE;

        if( packet_index_counter + 1 == 32 ){
        	packet_buffer = initial_address;
        	//printHeaderOnCPU( packet_buffer );
             cudaMemcpy( packet_buffer_device, packet_buffer ,THREAD_NUMBER_X * PACKET_SIZE, cudaMemcpyHostToDevice );
             processPacket<<<1, 32>>>( packet_buffer_device, PACKET_SIZE, info_device );
             cudaMemcpy( info, info_device, sizeof( struct packet_info ), cudaMemcpyDeviceToHost );
             //unsigned char* addr = info->dst_mac;
            // printf( "READ FROM GPU -Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5] );
             packet_index_counter = 0;
        }




    }


}
