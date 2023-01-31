#include <netinet/in.h>
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

#include "ft_ping.h"
#include "libft.h"


#define BUF_SIZE 500

t_ft_ping *g_ping;


void setting_socket_option(){

    // use setsockopt

}


void sending_packets(int file_descriptor){

    // declaration
    ssize_t                 byte_send;
    char                    packet[BUF_SIZE];
    int                     dest_address_len;
    struct sockaddr         dest_address;
    struct sockaddr_in      address_data;


    // definition dest_address
    dest_address_len = sizeof(dest_address);
    memset(&dest_address, 0, dest_address_len);
    dest_address.sa_family = g_ping->socket.domain;

    address_data.sin_port = 0;
    address_data.sin_family = dest_address.sa_family;
    address_data.sin_addr = g_ping->internet_address.sin_addr;
    address_data = g_ping->internet_address;


    ft_bzero(packet, BUF_SIZE);

    // ICMP ECHO_REQUEST packets

    byte_send = sendto(file_descriptor, packet, sizeof(packet), 0, (struct sockaddr *) &address_data, dest_address_len);
    // byte_send = sendto(file_descriptor, packet, sizeof(packet), MSG_CONFIRM, (struct sockaddr *) &address_data, dest_address_len);
    // byte_send = sendto(file_descriptor, packet, sizeof(packet), MSG_CONFIRM, &dest_address, dest_address_len);

    if (byte_send <= 0)
        fprintf (stderr, "Error : %s | on function sending packets.\n", strerror(errno));

    fprintf (stdout, "byte_read = %zu\n", byte_send);

}


int converter_address_binary(){


    int s;


    // struct in_addr      internet_address;
    char internet_address_string[INET_ADDRSTRLEN];


    // s = inet_pton(AF_INET, g_ping->host, internet_address_byte); // transform a text address to binary

    // fprintf (stderr, "HOST : [%s]\n", g_ping->host);
    s = inet_pton(AF_INET, g_ping->host, &g_ping->internet_address); // transform a text address to binary
    if (s == 0)
        fprintf (stderr, "not a valid address for the family : %i .\n", s);

    if (s == -1){
        perror("inet_pton");
        fprintf (stderr, "Error : %s | on function pton.\n", strerror(errno));
    }

    fprintf (stderr, "this is internet_address in byte order : [%p]\n", &g_ping->internet_address.sin_addr);


    if ( g_ping->internet_address.sin_family == AF_INET)
        fprintf (stderr, "not bad\n" );
    // we clean this
    memset(&internet_address_string, '\0', sizeof(internet_address_string));
    if (inet_ntop(AF_INET, &g_ping->internet_address, internet_address_string, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        fprintf (stderr, "Error : %s | on function ntop.\n", strerror(errno));
    }

    fprintf (stderr, "this is internet_address in string : [%s]\n", internet_address_string);

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


    sending_packets(g_ping->socket.file_descriptor);


    freeaddrinfo(g_ping->result);           /* No longer needed */
    return (EXIT_SUCCESS);
}
