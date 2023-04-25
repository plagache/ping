#include "ft_ping.h"

int check_reply_validity(t_icmp_packet_reply* echo_reply){

    int validity;

    validity = VALID;

    if (g_ping->ipv4_address.sin_addr.s_addr != echo_reply->ip_header.saddr)
        validity = INVALID;

    if (echo_reply->icmp_header.un.echo.id != g_ping->program_id){
        validity = INVALID;
    }

    if (echo_reply->icmp_header.type != 0){
        validity = INVALID;
    }

    if (validate_icmp_checksum() != VALID){
        // fprintf (stdout, "INVALID PACKET \n");
        validity = INVALID;
    }

    if (validity == VALID)
        g_ping->valide_message++;

    return validity;
}

void print_information_from_received_message(t_icmp_packet_reply* echo_reply){

    char source_address_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &echo_reply->ip_header.saddr, source_address_string, INET_ADDRSTRLEN);
    char destination_address_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &echo_reply->ip_header.daddr, destination_address_string, INET_ADDRSTRLEN);

    // fprintf(stdout, "IP header: ttl=%d version=%d\n protocol=%d, check=%d\n frag_off=%d, id=%d, ihl=%d\n source=%s, destination=%s\n",
    //        echo_reply->ip_header.ttl,
    //        echo_reply->ip_header.version,
    //        echo_reply->ip_header.protocol,
    //        echo_reply->ip_header.check,
    //        echo_reply->ip_header.frag_off,
    //        echo_reply->ip_header.id,
    //        echo_reply->ip_header.ihl,
    //        source_address_string,
    //        destination_address_string);

    // verify_checksum ?

    // we display only the messages that correspond to the id of our program
        // fprintf(stdout, "program id :%d\nid of the icmp response:%d\n",
        //         g_ping->program_id,
        //         echo_reply->icmp_header.un.echo.id
        //         );
    if (echo_reply->icmp_header.un.echo.id == g_ping->program_id){
        fprintf(stdout, "%lu bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
                g_ping->bytes_received - sizeof(struct iphdr),
                source_address_string,
                echo_reply->icmp_header.un.echo.sequence,
                echo_reply->ip_header.ttl,
                (double)compare_timestamp(echo_reply->timestamp) / 1000
                );
    }
}


void waiting_icmp_echo_reply(){

    uint8_t buffer[BUFFER_SIZE];
    // struct sockaddr_in sender_address;
    struct msghdr message_header;
    struct iovec in_out_vector;
    // fprintf(stdout, "buffer avant memset%s\n", buffer);
    ft_memset(&buffer, 0, sizeof(buffer));
    ft_memset(&message_header, 0, sizeof(message_header));
    ft_memset(&in_out_vector, 0, sizeof(in_out_vector));

    in_out_vector.iov_base = buffer;
    in_out_vector.iov_len = sizeof(buffer);
    //
    // message_header.msg_name = &sender_address;
    // message_header.msg_namelen = sizeof(sender_address);
    message_header.msg_iov = &in_out_vector;
    message_header.msg_iovlen = 1;
    // message_header.msg_control = NULL;
    // message_header.msg_controllen = 0;
    // message_header.msg_flags = 0;
    //
    g_ping->bytes_received = recvmsg(g_ping->socket.file_descriptor, &message_header, 0);

    if (g_ping->bytes_received < 0) {
        perror("recvmsg");
        exit(1);
    }

    // fprintf (stdout, "byte_received with recvmsg = %zu\n", g_ping->bytes_received);
    // print_information_from_received_message(in_out_vector.iov_base);
    if (g_ping->bytes_received != 0){
        // fprintf (stdout, "byte_received with recvmsg = %zu\n", g_ping->bytes_received);
        // fprintf (stdout, "icmp_header size = %zu\n", g_ping->bytes_received - sizeof(struct iphdr));
        g_ping->icmp_echo_reply = *(t_icmp_packet_reply*)in_out_vector.iov_base;
        // print_memory(&g_ping->icmp_echo_reply, sizeof(g_ping->icmp_echo_reply));
        if (check_reply_validity(&g_ping->icmp_echo_reply) == VALID)
            print_information_from_received_message(&g_ping->icmp_echo_reply);
    }

}
