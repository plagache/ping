#include "ft_ping.h"
#include "libft.h"


// rfc1071
// the icmp checksum field is 2 bytes or 16 bits
// 1 bytes = 1 octet
// 1 octet = 8 bits
// 2 bytes = 16 bits
uint16_t calculate_icmp_checksum(uint16_t *addr, int size) {
    // we store the sum in 2 times the size of the checksum
    uint16_t checksum = 0;
    uint32_t sum = 0;

    // we are adding 
    while (size > 1) {
        sum += *addr++;
        size -= sizeof(uint16_t);
    }

    // if we have an odd size of of bytes
    if (size > 0) {
        sum += *(u_char *) addr;
    }

    /*  Fold 32-bit sum to 16 bits */
    // tant qu'il reste des bits dans la partie de gauche
    while ((sum >> 16) != 0)
           sum = (sum >> 16) + (sum & 0xffff);
           // sum = (sum & 0xffff) + (sum >> 16);
    // the and (&) operator example : x & 0xFFFF = x
    // the >> operator shift to the right

    // the 1's complement of a binary number is the value obtain by inverting all the bits in the binary
    // this name come from the fact that adding the two value would result in all 1 activated;
    // the ~ (NOT) operator invert all bits
    checksum = (uint16_t) ~sum;
    return checksum;
}


void icmp_packet_creation(){

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
    timestamp_creation(&packet.data);


    // We calculate checksum and put it in the correct field
    packet.header.checksum = calculate_icmp_checksum((unsigned short*)&packet, PACKET_SIZE);

    // We copy the definitive packet to the struct in order to send it
    ft_memcpy(g_ping->packet, &packet, PACKET_SIZE);
}

void sending_packets(int file_descriptor){

    // declaration
    ssize_t                 byte_send;
    struct sockaddr_in      address_data;

    g_ping->internet_address.sin_family = g_ping->socket.domain;
    // fprintf (stdout, "family  = %i\n", g_ping->internet_address.sin_family);
    g_ping->internet_address.sin_port = g_ping->socket.port;
    // fprintf (stdout, "port = %i\n", g_ping->internet_address.sin_port);

    byte_send = sendto(file_descriptor, &g_ping->packet, sizeof(g_ping->packet), 0, (struct sockaddr *) &g_ping->internet_address, sizeof(address_data));

    if (byte_send <= 0)
        fprintf (stderr, "Error : %s | on function sending packets.\n", strerror(errno));

    // fprintf (stdout, "byte_send with sendto = %zu\n", byte_send);
}

