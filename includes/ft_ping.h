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
#include "libft.h"



// #define IP_MAX_SIZE 65535
// #define BUFFER_SIZE 1024
// #define PACKET_SIZE 4096
#define PACKET_SIZE 64
// #define PACKET_SIZE 65535
#define IP_HEADER_SIZE sizeof(struct iphdr)
// #define PACKET_SIZE 1492 - IP_HEADER_SIZE
#define BUFFER_SIZE 1492 - IP_HEADER_SIZE
#define PACKET_DATA_SIZE (PACKET_SIZE - sizeof(struct icmphdr))

// # define TRUE           0
// # define FALSE          1
# define FAILURE        1
# define SUCCESS        0
# define INVALID        1
# define VALID          0




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


typedef struct          s_icmp_packet_reply
{
    struct              iphdr ip_header;
    struct              icmphdr icmp_header;
    struct              timeval timestamp;
    uint8_t             data;

}                       t_icmp_packet_reply;


typedef struct          s_icmp_packet
{
    struct              icmphdr header;
    char                data[PACKET_SIZE - sizeof(struct icmphdr)];

}                       t_icmp_packet ;


typedef struct          s_ft_ping
{
    int                 parsed_arguments;

    t_options_ping      options;

    int                 count;

    char                *host;

    char                *hostname;

    char                *ip_address;

    struct sockaddr_in  ipv4_address;

    t_socket            socket;

    char                packet[PACKET_SIZE];

    t_icmp_packet       icmp_packet;

    t_icmp_packet_reply icmp_echo_reply;

    int                 program_id;

    int                 sequence_number;

    size_t              packets_sent;

    size_t              packets_recv;

    int                 valide_message;

    ssize_t             bytes_received;

    int                 bytes_send;

    // struct addrinfo     *result;

}                       t_ft_ping;


extern t_ft_ping *g_ping;


/* Parsing */

int ft_lexer(int ac, char **av);
int test_option();

int is_an_option(char *argument);
int parse_option(char *opt);
int activate_options(char c);

int set_host_type(char *argument);
int set_host(char *host);


/* address */

void ip_address_to_struct();
void hostname_to_struct();


/* socket */

void setting_socket_option();
int create_socket_file_descriptor(t_socket *sock);
void define_raw_socket();

/* checksum */

int validate_icmp_checksum();
uint16_t compute_icmp_checksum(uint16_t *addr, int size);

/* packet */

void create_icmp_packet();


/* timestamp */
unsigned long compare_timestamp(struct timeval message_timestamp);
void *create_timestamp(void *destination);


/* request */

void sending_icmp_echo_request(int file_descriptor);


/* reply */

int check_reply_validity(t_icmp_packet_reply* echo_reply);
void print_information_from_received_message(t_icmp_packet_reply* echo_reply);
void waiting_icmp_echo_reply();


/* signal */

void stop_program(int signal_number);


/* print_memory */

void print_memory(void *buffer, ssize_t size);

#endif
