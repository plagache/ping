#include <netinet/in.h>
#include <netinet/ip.h>
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

#include <sys/time.h> // gettimeofday header

#define BUFFER_SIZE 1024

#include <unistd.h> // getpid header


#include "ft_ping.h"
#include "libft.h"


t_ft_ping *g_ping;

void print_information_from_received_message(char buffer[BUFFER_SIZE]){

    struct iphdr* ip_header = (struct iphdr*) buffer;
    struct icmp* icmp_header = (struct icmp*) (buffer + sizeof(struct iphdr));

    char source_address_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_header->saddr, source_address_string, INET_ADDRSTRLEN);
    char destination_address_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_header->daddr, destination_address_string, INET_ADDRSTRLEN);

    // fprintf(stdout, "IP header: ttl=%d version=%d\n protocol=%d, check=%d\n frag_off=%d, id=%d, ihl=%d\n source=%s, destination=%s\n",
    //        ip_header->ttl,
    //        ip_header->version,
    //        ip_header->protocol,
    //        ip_header->check,
    //        ip_header->frag_off,
    //        ip_header->id,
    //        ip_header->ihl,
    //        source_address_string,
    //        destination_address_string);


    // we display only the messages that correspond to the id of our program
    if (icmp_header->icmp_id == g_ping->program_id){
        // fprintf(stdout, "id of the icmp response:%d\n",
        //         icmp_header->icmp_id
        //         );
        fprintf(stdout, "%d bytes from %s: icmp_seq=%d ttl=%d time=%d\n",
                64,
                source_address_string,
                // ntohs(icmp_header->icmp_seq),
                icmp_header->icmp_seq,
                ip_header->ttl,
                24359
                );
    }
}

void received_message(){

    ssize_t                 bytes_received;

    char buffer[BUFFER_SIZE];
    struct sockaddr_in sender_address;
    struct msghdr msg;
    struct iovec iov;

    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);

    msg.msg_name = &sender_address;
    msg.msg_namelen = sizeof(sender_address);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_flags = 0;

    bytes_received = recvmsg(g_ping->socket.file_descriptor, &msg, 0);

    if (bytes_received < 0) {
        perror("recvmsg");
        exit(1);
    }
    if (bytes_received != 0){
        // fprintf (stdout, "byte_received with recvmsg = %zu\n", bytes_received);
        // fprintf(stdout, "Received ICMP packet from %s\n", inet_ntoa(sender_address.sin_addr));
        print_information_from_received_message(buffer);
    }

}

unsigned short calculate_icmp_checksum(unsigned short *buffer, int size) {
    unsigned long sum = 0;
    while (size > 1) {
        sum += *buffer++;
        size -= 2;
    }
    if (size == 1) {
        sum += *(unsigned char*) buffer;
    }
    sum = (sum >> 16) + (sum & 0xffff); // put it in correct order
    sum += (sum >> 16);
    return (unsigned short) ~sum;
}


void icmp_packet_creation(){

    struct icmp icmp_header;
    ft_memset(&icmp_header, 0, sizeof(icmp_header)); // making sure the packet is clean (set to 0) before operating on it

    // describe in a struct the icmp header
    icmp_header.icmp_type = ICMP_ECHO;
    icmp_header.icmp_code = 0;
    // icmp_header.icmp_id = htons(getpid()); // htons set the byte in network order
    icmp_header.icmp_id = g_ping->program_id; // but not nescessary
    // icmp_header.icmp_seq = htons(g_ping->sequence_number);
    icmp_header.icmp_seq = g_ping->sequence_number;
    // icmp_header.icmp_hun.ih_idseq = g_ping->sequence_number;
    icmp_header.icmp_cksum = 0;
    icmp_header.icmp_cksum = calculate_icmp_checksum((unsigned short*)&icmp_header, sizeof(struct icmphdr));



    // data to set in the ICMP packet
    // char *data = "Hello, world!";
    // the timestamp should be the data
    // maybe a option to set on the socket

    // struct timeval tv;
    // gettimeofday(&tv, NULL);
    // unsigned long *timestamp_ptr = (unsigned long*)icmp_header.icmp_data;
    // *timestamp_ptr = ((unsigned long)tv.tv_sec) * 1000 + ((unsigned long)tv.tv_usec) / 1000; // store timestamp in milliseconds


    // copy the data into the data area of the ICMP packet
    // pointer to the start of the data area in the ICMP packet
    // ft_memcpy(icmp_header.icmp_data + sizeof(struct icmphdr), data, ft_strlen(data));

    // ft_memcpy(g_ping->packet, "Hello, world!", 13);
    // ft_memcpy(&g_ping->packet, &icmp_header, sizeof(struct icmphdr));
    //


    // copy the described icmp header in the char packet
    ft_memcpy(g_ping->packet, &icmp_header, sizeof(icmp_header));
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
    setsockopt(g_ping->socket.file_descriptor, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    int enable = 0;
    setsockopt(g_ping->socket.file_descriptor, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable));

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


int converter_address_binary(){


    int s;


    // struct in_addr      internet_address;
    char internet_address_string[INET_ADDRSTRLEN];


    // fprintf (stderr, "HOST : [%s]\n", g_ping->host);
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

    // fprintf (stderr, "ft_ping.c ln:119 Internet_address in string : [%s]\n", internet_address_string);

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

    // fprintf (stderr, "file descriptor : [%i]\n", sock->file_descriptor);

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

void stop_program(int signal_number){

    fprintf(stdout, "We received the signal to stop %d\n", signal_number);
    fprintf(stdout, "this is the end of the program");
    freeaddrinfo(g_ping->result);           /* No longer needed */
    exit(EXIT_SUCCESS);
    fprintf(stdout, "this is after the exit of the program");

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

    g_ping->program_id = getpid();

    converter_address_binary();

    raw_socket_definition();

    create_socket_file_descriptor(&g_ping->socket);

    setting_socket_option();

    // iterate here
    // first printf
    // PING Host IP in string 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
    fprintf(stdout, "PING %s (%s) 56(84) bytes of data.\n", g_ping->host, g_ping->host);

    // wait with usleep 1 seconde between iteration
    //
    g_ping->sequence_number = 1;

    g_ping->count = 4;
    g_ping->count = 0;

    while(g_ping->count >= g_ping->sequence_number || g_ping->count == 0){

        icmp_packet_creation();

        sending_packets(g_ping->socket.file_descriptor);

        received_message();
        // each received_message() will also print information of this type;
        // 64 bytes from 8.8.8.8: icmp_seq=4 ttl=116 time=10.8 ms


        // when the iteration is finished
        // we display statistics free everythintg
        // and exit
        // --- 8.8.8.8 ping statistics ---
        // 4 packets transmitted, 4 received, 0% packet loss, time 3005ms
        // rtt min/avg/max/mdev = 9.019/11.103/12.798/1.395 ms
        g_ping->sequence_number++;
        // fprintf(stdout, "count %d, sequence number: %d", count, g_ping->sequence_number);
        usleep(1000000);
    }


    freeaddrinfo(g_ping->result);           /* No longer needed */

    return (EXIT_SUCCESS);
}
