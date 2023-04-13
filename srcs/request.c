#include "ft_ping.h"

void sending_icmp_echo_request(int file_descriptor){

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
