
#include <cuda_runtime.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/if_ether.h>
#include <stdlib.h>

#define ETH_ADDR_LEN 6
#define IP_ADDR_LEN 4

const int PACKET_BUFFER_SIZE = 65536;

struct packet_info{

    unsigned char source_mac[ETH_ADDR_LEN];
    unsigned char dst_mac[ETH_ADDR_LEN];
    unsigned int source_ip;
    unsigned int dst_ip;

};



int openRawEthernetSocket(){


    int raw_socket = socket( AF_PACKET, SOCK_RAW, htons( ETH_P_ALL ) );

    if( raw_socket < 0 ){

        perror( "Could not open socket: " );
        exit( -1 );
    }


    return raw_socket;

}


__device__ void printHello(){

    printf("I am from utility function\n");
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

    printf(" GPU Calculated  |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", info->dst_mac[0] , info->dst_mac[1] , info->dst_mac[2] , info->dst_mac[3] , info->dst_mac[4] , info->dst_mac[5] );
}

__global__ void processPacket( unsigned char* packet_buffer, int size, struct packet_info* info ){

    resolveEthernetHeader( packet_buffer, info );

}

int main( int argc, char* argv[] ){


	int sockaddr_size, data_size;
	struct sockaddr saddr;

	int sniffer_socket = openRawEthernetSocket();

	unsigned char* packet_buffer = new unsigned char [PACKET_BUFFER_SIZE];

    unsigned char* packet_buffer_device;

    struct packet_info* info_device;

    struct packet_info* info;

    info = ( struct packet_info* )malloc( sizeof( struct packet_info* ) );

    cudaMalloc( (void**) &info_device, sizeof( struct packet_info ));

    cudaMalloc( (void**) &packet_buffer_device, PACKET_BUFFER_SIZE );

  //  cudaMemset( info_device, 0, sizeof( struct packet_info ) );


	sockaddr_size = sizeof( struct sockaddr );
    dim3 block(1);
    dim3 grid(1);

	while(1){

		data_size = recvfrom( sniffer_socket, packet_buffer, PACKET_BUFFER_SIZE, 0, &saddr, (socklen_t*)&sockaddr_size );
		packet_buffer[data_size] = '\0';
        cudaMemcpy( packet_buffer_device, packet_buffer ,data_size, cudaMemcpyHostToDevice );
		//struct ethhdr *eth = (struct ethhdr *)packet_buffer;
		//printf(" CPU Calculated  |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
		//printf( "Packet captured\n %d \n\n", data_size );


		processPacket<<<grid, block>>>( packet_buffer_device, data_size, info_device );

        cudaMemcpy( info, info_device, sizeof( struct packet_info ), cudaMemcpyDeviceToHost );
        unsigned char* addr = info->dst_mac;
        printf( "READ FROM GPU -Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5] );

		//packet_buffer[data_size+1] = '\0';


	}


}
