#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>

#include <unistd.h> // getpid header


#include "ft_ping.h"
#include "libft.h"


t_ft_ping *g_ping;


unsigned short calculate_checksum(unsigned short *buffer, int size) {
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buffer++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char*) buffer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return (unsigned short) ~sum;
}


void icmp_packet_creation(){

    struct icmp icmp_header;
    ft_memset(&icmp_header, 0, sizeof(icmp_header)); // making sure the packet is clean (set to 0) before operating on it

    // describe in a struct the icmp header
    icmp_header.icmp_type = ICMP_ECHO;
    icmp_header.icmp_code = 0;
    icmp_header.icmp_id = htons(getpid()); // htons set the byte in network order
    icmp_header.icmp_seq = htons(1);
    icmp_header.icmp_cksum = 0;
    icmp_header.icmp_cksum = calculate_checksum((unsigned short*)&icmp_header, sizeof(struct icmphdr));

    // copy the described icmp header in the char packet
    ft_memcpy(g_ping->packet, &icmp_header, sizeof(icmp_header));

    // ft_memcpy(g_ping->packet, "Hello, world!", 13);
    // ft_memcpy(&g_ping->packet, &icmp_header, sizeof(struct icmphdr));
}

void setting_socket_option(){

    // // declaration
    // int                     setsockopt_return;
    // struct timeval          timeout;
    // int                     timeout_len;
    // int                     timeout_value;
    // SOL_SOCKET              socket_level;
    // SO_RCVTIMEO             socket_option_name;
    // struct sockaddr         socket_option_value;
    // socklen_t               socket_option_len;
    // use setsockopt
    int time_to_live;
    time_to_live = 63;
    int ttl = time_to_live;
    int enable = 0;
    setsockopt(g_ping->socket.file_descriptor, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));
    setsockopt(g_ping->socket.file_descriptor, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
    // int protocol = IPPROTO_IP;
    // setsockopt(g_ping->socket.file_descriptor, IPPROTO_IP, IP_PROTOCOL, &protocol, sizeof(protocol));
}


void sending_packets(int file_descriptor){

    // declaration
    ssize_t                 byte_send;
    struct sockaddr_in      address_data;


    // ICMP ECHO_REQUEST packets


    // use the function sendto to send the packet
    // and use the file_descriptor to send the packet
    // address_data = g_ping->internet_address;
    // fprintf (stdout, "address = %p\n", &address_data.sin_addr);
    // address_data.sin_family = g_ping->socket.domain;
    // fprintf (stdout, "family  = %i\n", address_data.sin_family);
    // address_data.sin_port = 0;
    // fprintf (stdout, "port = %i\n", address_data.sin_port);
    // fprintf (stdout, "sin_zero = %s\n", address_data.sin_zero);
    // fprintf (stdout, "INET family  = %i\n", AF_INET);
    // byte_send = sendto(file_descriptor, g_ping->packet, sizeof(g_ping->packet), 0, (struct sockaddr *) &address_data, sizeof(address_data));
    //
    // if (byte_send <= 0)
    //     fprintf (stderr, "Error : %s | on function sending packets.\n", strerror(errno));

    g_ping->internet_address.sin_family = g_ping->socket.domain;
    fprintf (stdout, "family  = %i\n", g_ping->internet_address.sin_family);
    // fprintf (stdout, "address = %p\n", &g_ping->internet_address);
    g_ping->internet_address.sin_port = g_ping->socket.port;
    fprintf (stdout, "port = %i\n", g_ping->internet_address.sin_port);


    byte_send = sendto(file_descriptor, &g_ping->packet, sizeof(g_ping->packet), 0, (struct sockaddr *) &g_ping->internet_address, sizeof(address_data));


    if (byte_send <= 0)
        fprintf (stderr, "Error : %s | on function sending packets.\n", strerror(errno));

    fprintf (stdout, "byte_send = %zu\n", byte_send);
}


int converter_address_binary(){


    int s;


    // struct in_addr      internet_address;
    char internet_address_string[INET_ADDRSTRLEN];


    fprintf (stderr, "HOST : [%s]\n", g_ping->host);
    s = inet_pton(AF_INET, g_ping->host, &g_ping->internet_address.sin_addr); // transform an IP address string to binary
    if (s == 0)
        fprintf (stderr, "not a valid address for the family : %i .\n", s);

    if (s == -1){
        perror("inet_pton");
        fprintf (stderr, "Error : %s | on function pton.\n", strerror(errno));
    }


    if ( g_ping->internet_address.sin_family == AF_INET)
        fprintf (stderr, "not bad\n" );
    // we clean this
    memset(&internet_address_string, '\0', sizeof(internet_address_string));
    if (inet_ntop(AF_INET, &g_ping->internet_address.sin_addr, internet_address_string, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        fprintf (stderr, "Error : %s | on function ntop.\n", strerror(errno));
    }

    fprintf (stderr, "ft_ping.c ln:119 Internet_address in string : [%s]\n", internet_address_string);

    return 0;
}


int get_address_information(){

    int                     getaddrinfo_return;
    struct addrinfo         hints;
    // struct addrinfo         *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    hints.ai_socktype = 0; /* RAW socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    // hints.ai_addr = g_ping->host;
    hints.ai_next = NULL;


    getaddrinfo_return = getaddrinfo(g_ping->host, NULL, &hints, &g_ping->result);
    // fprintf (stderr, "address = %s\n", (char *)g_ping->result->ai_addr);
    // fprintf (stderr, "family = %i\n", g_ping->result->ai_family);

    return getaddrinfo_return;
}


// in order to create a file_descriptor, you have to define the socket parameter
int create_socket_file_descriptor(t_socket *sock){

    sock->file_descriptor = socket(sock->domain, sock->type, sock->protocol);

    if (sock->file_descriptor == -1)
        fprintf (stderr, "Error : %s | on function create socket.\n", strerror(errno));

    fprintf (stderr, "file descriptor : [%i]\n", sock->file_descriptor);

    return sock->file_descriptor;
}


void raw_socket_definition() {

    g_ping->socket.type = SOCK_RAW;
    // g_ping->socket.protocol = IPPROTO_RAW;                 // cf the man page protocols definition
    // ping send ICMP messages // are this packets ?
    g_ping->socket.protocol = IPPROTO_ICMP;                 // leader gd (go to definition) for more information // from <netinet/in.h>
    g_ping->socket.domain = AF_INET;                        // domain for the IPv4
    g_ping->socket.port = 0;                        // no port so 0

}


int main(int ac, char **av){

    // int                     getaddrinfo_return;
    // struct addrinfo         *rp;
    t_ft_ping               global_ping;

    g_ping = &global_ping;


    g_ping->arguments_parser = ft_lexer(ac, av);

    if (g_ping->arguments_parser != ac)
    {
        fprintf (stderr, "we didnt parse all the argument : arguments_parser = %i | ac = %i \n", g_ping->arguments_parser, ac);
        return (EXIT_FAILURE);
    }
    else if (g_ping->arguments_parser == ac) {
        // fprintf (stderr, "lexer is ok: ac = %i\n", g_ping->arguments_parser);
    }

    // test_option();


    // getaddrinfo_return = get_address_information();
    //
    // if (getaddrinfo_return != 0) {
    //     fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_return));
    //     exit(EXIT_FAILURE);
    // }


    converter_address_binary();


    raw_socket_definition();


    create_socket_file_descriptor(&g_ping->socket);

    setting_socket_option();

    icmp_packet_creation();

    sending_packets(g_ping->socket.file_descriptor);


    freeaddrinfo(g_ping->result);           /* No longer needed */
    return (EXIT_SUCCESS);
}
