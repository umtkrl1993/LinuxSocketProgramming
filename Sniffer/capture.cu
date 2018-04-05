
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

#define CHECK(call)                                                            \
{                                                                              \
    const cudaError_t error = call;                                            \
    if (error != cudaSuccess)                                                  \
    {                                                                          \
        printf("Error: %s:%d, ", __FILE__, __LINE__);                 \
        printf("code: %d, reason: %s\n", error,                       \
                cudaGetErrorString(error));                                    \
        exit(1);                                                               \
    }                                                                          \
}


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

__global__ void processPacket( unsigned char* packet_buffer, int size, struct packet_info* info, int size_info ){

    unsigned char* tmp_buffer = packet_buffer;
    struct packet_info* temp = info;

    tmp_buffer = packet_buffer +  ( threadIdx.x * size );
    temp = info + ( threadIdx.x * size_info );

    struct ethhdr *eth = ( struct ethhdr *)tmp_buffer;

    printf( "Adress of temp %p\n", temp );

    temp->source_ip = threadIdx.x;
    temp->dst_ip = threadIdx.x + 32;

    temp->dst_mac[0] = eth->h_dest[0];
    temp->dst_mac[1] = eth->h_dest[1];
    temp->dst_mac[2] = eth->h_dest[2];
    temp->dst_mac[3] = eth->h_dest[3];
    temp->dst_mac[4] = eth->h_dest[4];
    temp->dst_mac[5] = eth->h_dest[5];
    printf(" GPU Resolved with thread id %d |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", threadIdx.x,temp->dst_mac[0] , temp->dst_mac[1] , temp->dst_mac[2] , temp->dst_mac[3] , temp->dst_mac[4] , temp->dst_mac[5] );

}


static void printHeaderOnCPU( struct packet_info* info ){

	struct packet_info * tmp = info;
	for( int i = 0; i < 32; i++ ){
		 tmp = info + ( i * sizeof( struct packet_info ));
		 printf(" CPU Resolved |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n",tmp->dst_mac[0] , tmp->dst_mac[1] , tmp->dst_mac[2] , tmp->dst_mac[3] , tmp->dst_mac[4] , tmp->dst_mac[5] );

	}

	tmp = info;
	for( int i = 0; i < 32; i++ ){
		tmp = info + ( i * sizeof (struct packet_info ) );
		printf( "resolved ids is %d and %d \n", tmp->source_ip, tmp->dst_ip );

	}
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

    cudaMemset( info_device, THREAD_NUMBER_X* sizeof( struct packet_info ), 0 );


    sniffer_socket = openSnifferSocket();
    dim3 block(THREAD_NUMBER_X);
    dim3 grid(1);

    unsigned char* initial_address = packet_buffer;

    unsigned char* dummy = ( unsigned char* )malloc( sizeof( unsigned char ) * PACKET_SIZE ) ;

    int size_info = sizeof( struct packet_info );

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
             processPacket<<<grid, 16>>>( packet_buffer_device, PACKET_SIZE, info_device, sizeof( struct packet_info ) );

             CHECK(cudaMemcpy( info , info_device,THREAD_NUMBER_X * sizeof(struct packet_info), cudaMemcpyDeviceToHost ));
             CHECK(cudaGetLastError());
             cudaFree( info_device );
             cudaFree( packet_buffer_device );
             cudaThreadExit();
             printHeaderOnCPU( info );
             //unsigned char* addr = info->dst_mac;
            // printf( "READ FROM GPU -Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5] );
             packet_index_counter = 0;

             exit(0);
        }




    }


}
