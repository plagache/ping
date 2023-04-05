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

#include <signal.h>
#include <sys/time.h> // gettimeofday header
#include <time.h>

#define BUFFER_SIZE 1024

#include <unistd.h> // getpid header


#include "ft_ping.h"
#include "libft.h"


t_ft_ping *g_ping;


unsigned long timestamp_compare(void *message_timestamp){

    // the real ping probably use the struct timespec than can contain Nano seconds
    // this is why it has more frame
    struct timeval time_value;
    // the second argument can be set te specify a particular timezone
    // we putt NULL to let the kernel set the timezone
    gettimeofday(&time_value, NULL);

    unsigned long diff = 0;

    // Here we print the time that gettimeofday give us
    // printf("Seconds: %ld\nMicroseconds: %ld\n", time_value.tv_sec, time_value.tv_usec);
    // printf("Human time: %s", ctime(&time_value.tv_sec));


    // we declare a structure to contain our data
    // we then copy the content of this struct in our void *;
    struct s_timestamp_container current_timestamp;
    current_timestamp.tv_sec = time_value.tv_sec;
    current_timestamp.tv_usec = time_value.tv_usec;
    // ft_memcpy(message_timestamp, &current_timestamp, sizeof(struct s_timestamp_container));
    
    struct s_timestamp_container* received_timestamp = (struct s_timestamp_container*) message_timestamp;


    if (current_timestamp.tv_sec == received_timestamp->tv_sec)
    {
        diff = current_timestamp.tv_usec - received_timestamp->tv_usec;
    }

    return (diff);
}


void print_information_from_received_message(char buffer[BUFFER_SIZE]){

    struct iphdr* ip_header = (struct iphdr*) buffer;
    struct icmphdr* icmp_header = (struct icmphdr*) (buffer + sizeof(struct iphdr));
    struct s_timestamp_container* timestamp = (struct s_timestamp_container*) (buffer + sizeof(struct iphdr) + sizeof(struct icmphdr));

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
    if (icmp_header->un.echo.id == g_ping->program_id){
        // fprintf(stdout, "id of the icmp response:%d\n",
        //         icmp_header->icmp_id
        //         );
        fprintf(stdout, "from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                source_address_string,
                icmp_header->un.echo.sequence,
                ip_header->ttl,
                (double)timestamp_compare(timestamp) / 1000
                );
    }
}

void received_message(){

    ssize_t                 bytes_received;

    char buffer[BUFFER_SIZE];
    struct sockaddr_in sender_address;
    struct msghdr message_header;
    struct iovec in_out_vector;

    in_out_vector.iov_base = buffer;
    in_out_vector.iov_len = sizeof(buffer);

    message_header.msg_name = &sender_address;
    message_header.msg_namelen = sizeof(sender_address);
    message_header.msg_iov = &in_out_vector;
    message_header.msg_iovlen = 1;
    message_header.msg_control = NULL;
    message_header.msg_controllen = 0;
    message_header.msg_flags = 0;

    bytes_received = recvmsg(g_ping->socket.file_descriptor, &message_header, 0);

    if (bytes_received < 0) {
        perror("recvmsg");
        exit(1);
    }
    if (bytes_received != 0){
        // fprintf (stdout, "byte_received with recvmsg = %zu\n", bytes_received);
        fprintf (stdout, "%zu bytes ", bytes_received - sizeof(struct iphdr));
        // fprintf(stdout, "Received ICMP packet from %s\n", inet_ntoa(sender_address.sin_addr));
        print_information_from_received_message(buffer);
    }

}

// the checksum function 
// we iterate on each bytes(8 octets / unsigned short)

            /* Compute Internet Checksum for "count" bytes
            *         beginning at location "addr".
            */
unsigned short calculate_icmp_checksum(unsigned short *addr, int size) {
    unsigned long checksum = 0;
    unsigned long sum = 0;

    // fprintf(stdout, "sum=%lu\n", sum);
    // fprintf(stdout, "Starting size=%u\n", size);

    while (size > 1) {
        sum += *addr++;
        size -= sizeof(unsigned short);
        // fprintf(stdout, "size > 1 sum=%lu\n", sum);
        // fprintf(stdout, "New size=%u\n", size);
    }

    if (size > 0) {
        sum += *(unsigned char*) addr;
        // fprintf(stdout, "size == 1 sum=%lu\n", sum);
        // fprintf(stdout, "New size=%u\n", size);
    }
    sum = (sum >> 16) + (sum & 0xffff); // put it in correct order
    // fprintf(stdout, "final calcul for sum %lu\n", sum);
    sum += (sum >> 16);
    // fprintf(stdout, "sum that is return %lu\n", sum);


    checksum = ~sum;    // the 1's complement of this sum is placed in the
                        // checksum field.
    // fprintf(stdout, "the return checksum =%lu is the 1's complement of the sum=%lu\n", checksum, sum);
    return (unsigned short) checksum;
}


void *timestamp_creation(void *destination){

    // the real ping probably use the struct timespec than can contain Nano seconds
    // this is why it has more frame
    struct timeval time_value;
    // the second argument can be set te specify a particular timezone
    // we putt NULL to let the kernel set the timezone
    gettimeofday(&time_value, NULL);


    // Here we print the time that gettimeofday give us
    // printf("Seconds: %ld\nMicroseconds: %ld\n", time_value.tv_sec, time_value.tv_usec);
    // printf("Human time: %s", ctime(&time_value.tv_sec));


    // we declare a structure to contain our data
    // we then copy the content of this struct in our void *;
    struct s_timestamp_container timestamp;
    timestamp.tv_sec = time_value.tv_sec;
    timestamp.tv_usec = time_value.tv_usec;
    ft_memcpy(destination, &timestamp, sizeof(struct s_timestamp_container));


    return (destination);
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
    int ttl;
    ttl = time_to_live;
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
    // freeaddrinfo(g_ping->result);           /* No longer needed */
    g_ping->sequence_number = 0;
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
    fprintf(stdout, "PING %s (%s) %lu(%lu) bytes of data.\n",
            g_ping->host,
            g_ping->host,
            PACKET_DATA_SIZE,
            sizeof(struct iphdr) + PACKET_SIZE
            );
    // wait with usleep 1 seconde between iteration
    //
    g_ping->sequence_number = 1;

    g_ping->count = 1;
    g_ping->count = 0;

    while((g_ping->count >= g_ping->sequence_number || g_ping->count == 0) && g_ping->sequence_number != 0){

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

    signal(SIGINT, stop_program);
    fprintf(stdout, "this is after the signal handling");

    // freeaddrinfo(g_ping->result);           /* No longer needed */

    return (EXIT_SUCCESS);
}
