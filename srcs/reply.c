#include "ft_ping.h"


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


void waiting_icmp_echo_reply(){

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
