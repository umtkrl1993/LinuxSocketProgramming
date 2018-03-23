
#ifndef _PROCESSORUTILITIES_H
#define _PROCESSORUTILITIES_H

#define ETH_ADDR_LEN 6
#include <arpa/inet.h>
#include <stdio.h>


struct packet_info{

    unsigned char source_mac[ETH_ADDR_LEN];
    unsigned char dst_mac[ETH_ADDR_LEN];
    struct sockaddr_in source_ipaddr;
    struct sockaddr_in dst_ipaddr;

};

void processPacketBatch( const unsigned char** packet_batch, struct packet_info* processed_packet ,int batch_size );
void processEthernetHeader( const char* packet, struct packet_info* processed_packet );
void processIPHeader( const char* packet, struct packet_info* processed_packet );
void logProcessedPackets( const struct packet_info*, int batch_size, FILE* log_file );



#endif