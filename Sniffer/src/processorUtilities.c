#include "processorUtilities.h"
#include <net/ethernet.h>  //For ether_header
#include <netinet/ip.h>
#include <netinet/if_ether.h>  //For ETH_P_ALL
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> //For standard things

void processPacketBatch( const unsigned char** packet_batch, struct packet_info* processed_packet ,int batch_size ){

	for( int i = 0; i < batch_size; i++ ){

		
		 //Get ethernet header info
		 processEthernetHeader( packet_batch[i], &processed_packet[i] );
		 processIPHeader( packet_batch[i], &processed_packet[i] );
	
	}
	
}

void processEthernetHeader( const char* packet, struct packet_info* processed_packet ){

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

void processIPHeader( const char* packet, struct packet_info* processed_packet ){

	 struct iphdr *iph = ( struct iphdr* )( packet + sizeof( struct ethhdr ) );

	 processed_packet->source_ipaddr.sin_addr.s_addr = iph->saddr;
	 processed_packet->dst_ipaddr.sin_addr.s_addr = iph->daddr;

}


void logProcessedPackets( const struct packet_info* processed_packet, int batch_size, FILE* log_file ){

	for( int i = 0; i < batch_size; i++ ){

		struct packet_info tmp = processed_packet[i];

		fprintf(log_file , "\n");
    	fprintf(log_file , "Ethernet Header\n");
    	fprintf(log_file , "   |-Source MAC Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", tmp.source_mac[0] ,tmp.source_mac[1] ,tmp.source_mac[2] , tmp.source_mac[3] , tmp.source_mac[4] , tmp.source_mac[5] );
    	fprintf(log_file , "   |-Destination MAC Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", tmp.dst_mac[0] , tmp.dst_mac[1] , tmp.dst_mac[2] , tmp.dst_mac[3] , tmp.dst_mac[4] , tmp.dst_mac[5] );
    	fprintf(log_file , "   |-Source IP        : %s\n",inet_ntoa(tmp.source_ipaddr.sin_addr));
   		fprintf(log_file , "   |-Destination IP   : %s\n",inet_ntoa(tmp.dst_ipaddr.sin_addr));
	}

}
