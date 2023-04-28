#include "ft_ping.h"


void create_icmp_packet(){

    struct s_icmp_packet packet;

    // Not nescessary since we can do all the packet
    // int data_size = PACKET_SIZE - sizeof(struct icmphdr);
    // ft_memset(packet.data, 0, data_size); // making sure the data packet is clean (set to 0) before operating on it

    ft_memset(&packet, 0, PACKET_SIZE); // making sure the packet is clean (set to 0) before operating on it


    // describe in a struct the icmp header
    // here our ECHO_Request
    packet.header.type = ICMP_ECHO;
    packet.header.code = 0;
    // this information should be turn in network byte order with htons
    // but it is not nescessary for the program to work
    // its more for other program to be able to detect correctly the data
    packet.header.un.echo.id = g_ping->program_id;
    packet.header.un.echo.sequence = g_ping->sequence_number;
    packet.header.checksum = 0;

    // timestamp_creation(&packet.data, data_size);
    set_timestamp(&packet.data);


    // We calculate checksum and put it in the correct field
    packet.header.checksum = compute_icmp_checksum((uint16_t*)&packet, PACKET_SIZE);

    // We copy the definitive packet to the struct in order to send it
    ft_memcpy(g_ping->packet, &packet, PACKET_SIZE);
}
