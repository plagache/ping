#ifndef FT_PING_H
# define FT_PING_H


#include <netinet/in.h>
# include <stdint.h>
# include <stddef.h>

#include <netinet/ip.h> // header for the ip struct
#include <netinet/ip_icmp.h> // header for the icmp struct
#include <sys/select.h>
#include <time.h>

// #define PACKET_SIZE 4096
#define PACKET_SIZE 64
#define IP_HEADER_SIZE sizeof(struct iphdr)
#define PACKET_DATA_SIZE (PACKET_SIZE - sizeof(struct icmphdr))

// # define TRUE           0
// # define FALSE          1
# define FAILURE        1
# define SUCCESS        0


/*
**  Definition des Structure de Ping
*/


# define PING_OPTIONS       "vh"
# define ON                 1
# define OFF                0


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


typedef struct s_icmp_packet
{
    struct icmphdr header;
    char    data[PACKET_SIZE - sizeof(struct icmphdr)];

} t_icmp_packet ;


typedef struct          s_ft_ping
{
    int                 arguments_parser;

    char                *host;

    t_options_ping      options;

    t_socket            socket;

    char                packet[PACKET_SIZE];

    t_icmp_packet       icmp_packet;

    int                 sequence_number;

    int                 count;

    int                 program_id;

    struct addrinfo     *result;

    struct sockaddr_in      internet_address;

}                       t_ft_ping;


extern t_ft_ping *g_ping;

// Parsing function

int ft_lexer(int ac, char **av);
int test_option();

int is_an_option(char *argument);
int parse_option(char *opt);
int activate_options(char c);

int is_an_host(char *argument);
int parse_host(char *host);


#endif
