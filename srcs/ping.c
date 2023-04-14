#include "ft_ping.h"


t_ft_ping *g_ping;


int main(int ac, char **av){

    // int                     getaddrinfo_return;
    // struct addrinfo         *rp;
    t_ft_ping               ping_data;

    g_ping = &ping_data;

    g_ping->sequence_number = 1;
    g_ping->program_id = getpid();
    // set with an option
    g_ping->count = 1;
    g_ping->count = 0;

    signal(SIGINT, stop_program);

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

    while(g_ping->count != g_ping->sequence_number && g_ping->sequence_number != 0){

        icmp_packet_creation();

        sending_icmp_echo_request(g_ping->socket.file_descriptor);

        waiting_icmp_echo_reply();
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

    fprintf(stdout, "this is after the signal handling\n");

    // freeaddrinfo(g_ping->result);           /* No longer needed */

    return (EXIT_SUCCESS);
}
