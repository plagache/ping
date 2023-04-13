#ifndef FT_PING_H
# define FT_PING_H

#include <bits/types/struct_timeval.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <sys/time.h> // gettimeofday header
#include <unistd.h> // getpid header
#include <netinet/in.h>
#include <netinet/ip.h> // header for the ip struct
#include <netinet/ip_icmp.h> // header for the icmp struct
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stddef.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>



#define BUFFER_SIZE 1024
// #define PACKET_SIZE 4096
#define PACKET_SIZE 64
#define IP_HEADER_SIZE sizeof(struct iphdr)
#define PACKET_DATA_SIZE (PACKET_SIZE - sizeof(struct icmphdr))

// # define TRUE           0
// # define FALSE          1
# define FAILURE        1
# define SUCCESS        0




# define PING_OPTIONS       "vh"
# define ON                 1
# define OFF                0

/*
**  Definition des Structure de Ping
*/

typedef struct          s_options_ping
{
    unsigned char       verbose: 1;
    unsigned char       help: 1;

}                       t_options_ping;


typedef struct          s_socket
{
    int domain;
    int type;
    int port;
    int protocol;
    int file_descriptor;

}                       t_socket;


typedef struct          s_icmp_packet
{
    struct icmphdr header;
    char    data[PACKET_SIZE - sizeof(struct icmphdr)];

}                       t_icmp_packet ;


typedef struct          s_ft_ping
{
    int                 parsed_arguments;

    t_options_ping      options;

    int                 count;

    char                *host;

    struct sockaddr_in  internet_address;

    t_socket            socket;

    char                packet[PACKET_SIZE];

    t_icmp_packet       icmp_packet;

    int                 program_id;

    int                 sequence_number;

    size_t              bytes_received;

    struct addrinfo     *result;

}                       t_ft_ping;


extern t_ft_ping *g_ping;

/* Parsing */

int ft_lexer(int ac, char **av);
int test_option();

int is_an_option(char *argument);
int parse_option(char *opt);
int activate_options(char c);

int is_an_host(char *argument);
int parse_host(char *host);


/* address */

int converter_address_binary();
int get_address_information();


/* socket */

void setting_socket_option();
int create_socket_file_descriptor(t_socket *sock);
void raw_socket_definition();


/* packet */

uint16_t calculate_icmp_checksum(uint16_t *addr, int size);
void icmp_packet_creation();
void sending_packets(int file_descriptor);


/* timestamp */
unsigned long timestamp_compare(struct timeval message_timestamp);
void *timestamp_creation(void *destination);


/* request */

void sending_icmp_echo_request(int file_descriptor);


/* reply */

void print_information_from_received_message(char buffer[BUFFER_SIZE]);
void waiting_icmp_echo_reply();


/* signal */

void stop_program(int signal_number);


#endif
