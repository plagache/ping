#include <bits/types/struct_timeval.h>
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




void print_information_from_received_message(char buffer[BUFFER_SIZE]){

    struct iphdr* ip_header = (struct iphdr*) buffer;
    struct icmphdr* icmp_header = (struct icmphdr*) (buffer + sizeof(struct iphdr));
    struct timeval* timestamp = (struct timeval*) (buffer + sizeof(struct iphdr) + sizeof(struct icmphdr));

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

    // verify_checksum ?

    // we display only the messages that correspond to the id of our program
    if (icmp_header->un.echo.id == g_ping->program_id){
        // fprintf(stdout, "id of the icmp response:%d\n",
        //         icmp_header->icmp_id
        //         );
        fprintf(stdout, "%lu bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                g_ping->bytes_received - sizeof(struct iphdr),
                source_address_string,
                icmp_header->un.echo.sequence,
                ip_header->ttl,
                (double)timestamp_compare(*timestamp) / 1000
                );
    }
}

void received_message(){

    char buffer[BUFFER_SIZE];
    // struct sockaddr_in sender_address;
    struct msghdr message_header;
    struct iovec in_out_vector;

    in_out_vector.iov_base = buffer;
    in_out_vector.iov_len = sizeof(buffer);

    // message_header.msg_name = &sender_address;
    // message_header.msg_namelen = sizeof(sender_address);
    message_header.msg_iov = &in_out_vector;
    message_header.msg_iovlen = 1;
    message_header.msg_control = NULL;
    message_header.msg_controllen = 0;
    message_header.msg_flags = 0;

    g_ping->bytes_received = recvmsg(g_ping->socket.file_descriptor, &message_header, 0);

    if (g_ping->bytes_received < 0) {
        perror("recvmsg");
        exit(1);
    }

    if (g_ping->bytes_received != 0){
        // fprintf (stdout, "byte_received with recvmsg = %zu\n", g_ping->bytes_received);
        // fprintf (stdout, "%zu bytes ", g_ping->bytes_received - sizeof(struct iphdr));
        // fprintf(stdout, "Received ICMP packet from %s\n", inet_ntoa(sender_address.sin_addr));
        print_information_from_received_message(buffer);
    }

}
int main(int ac, char **av){

    // int                     getaddrinfo_return;
    // struct addrinfo         *rp;
    t_ft_ping               ping_data;

    g_ping = &ping_data;


    g_ping->parsed_arguments = ft_lexer(ac, av);

    if (g_ping->parsed_arguments != ac)
    {
        fprintf (stderr, "we didnt parse all the argument : arguments_parser = %i | ac = %i \n", g_ping->parsed_arguments, ac);
        return (EXIT_FAILURE);
    }
    else if (g_ping->parsed_arguments == ac) {
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
