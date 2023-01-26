#include <stddef.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "ft_ping.h"


#define BUF_SIZE 500

t_ft_ping *g_ping;

// void test(){
//
//     fprintf(stderr, "integer in test before = %i\n", g_ping->integer);
//
//     g_ping->integer = 4;
//
//     fprintf(stderr, "integer in test before = %i\n", g_ping->integer);
//
// }

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
    fprintf (stderr, "address = %s\n", (char *)g_ping->result->ai_addr);
    fprintf (stderr, "family = %i\n", g_ping->result->ai_family);

    return getaddrinfo_return;
}

int print_result() {

    int                     socket_fd;
    int                     len;
    ssize_t                 byte_read;
    char                    buffer[BUF_SIZE];
    struct sockaddr         dest_address;
    int dest_address_len;
    // struct addrinfo         *result;
    // struct addrinfo         *result_pointer;
    //

    dest_address_len = sizeof(dest_address);
    memset(&dest_address, 0, dest_address_len);
    // dest_address->sa_data = g_ping->result->ai_addr;
    strcpy(dest_address.sa_data, "8.8.8.8");
    fprintf (stderr, "sa_data = %s\n", dest_address.sa_data);
           // (const char *)
    dest_address.sa_family = AF_INET;

    len = 2;
    fprintf (stderr, "result = %p\n", g_ping->result->ai_next);
    // socket_fd = socket( g_ping->result->ai_family, g_ping->result->ai_socktype, g_ping->result->ai_protocol);
    socket_fd = socket(dest_address.sa_family, 0, 0);
    fprintf (stderr, "socket_fd = %i\n", socket_fd);
    // strcpy("thats myshit!", (const char *)buffer);
    // fprintf (stderr, "buffer = %s\n", buffer);
    // byte_read = sendto(socket_fd, buffer, len, MSG_CONFIRM, &dest_address, dest_address_len);
    byte_read = sendto(socket_fd, buffer, len, MSG_CONFIRM, &dest_address, dest_address_len);
    fprintf (stdout, "byte_read = %zu\n", byte_read);
    // fprintf (stderr, "byte_read = %zu\n", byte_read);
    // if (byte_read == -1)
    //     return (EXIT_FAILURE);

    // result_pointer = g_ping->result;
    //
    // while (result_pointer->ai_next != NULL){
    //     socket_fd = socket(result_pointer->ai_family, result_pointer->ai_socktype, result_pointer->ai_protocol);
    //     byte_read = sendto(socket_fd, buffer, len, MSG_CONFIRM, NULL, 0);
    //     // fprintf(stderr, "byte_sent: %lu\n", byte_read);
    //     if (byte_read == -1)
    //         return (EXIT_FAILURE);
    //     result_pointer = result_pointer->ai_next;
    // }
    return byte_read;
    return 0;
}


int main(int ac, char **av){

    int                     getaddrinfo_return;
    // struct addrinfo         *rp;
    t_ft_ping               global_ping;

    g_ping = &global_ping;

    // g_ping->integer = 9;
    //
    // test();
    //
    // int t = g_ping->integer;
    //
    // fprintf(stderr, "integer = %i\n", t);
    //
    // if (ac != 0){
    //     fprintf (stderr, "%s\n", av[1]);
    //     fprintf (stderr, "%i\n", EXIT_FAILURE);
    //     fprintf (stderr, "%i\n", EXIT_SUCCESS);
    //     fprintf (stderr, "%i\n", EOF);
    // }

    g_ping->arguments_parser = ft_lexer(ac, av);

    if (g_ping->arguments_parser != ac)
    {
        // fprintf(stdout, "%lu ||| %lu\n", sizeof(t_options_ping), sizeof(unsigned int));
        // inet_ntop(AF_INET, const void *restrict cp, char *restrict buf, socklen_t len)
        // fprintf (stderr, "error on lexer ac = %i\n", ac);
        // fprintf (stderr, "error on lexer ac = %i\n", ft_lexer(ac, av));
        test_option();
        return (EXIT_FAILURE);
    }
    else if (g_ping->arguments_parser == ac) {
        fprintf (stderr, "lexer is ok; ac = %i\n", g_ping->arguments_parser);
        test_option();
    }

    // unsigned char buf[sizeof(struct in6_addr)];
    // fprintf(stderr, "av[2]: %s\n", av[2]);
    // int s = inet_pton(hints.ai_family, av[2], buf);


    getaddrinfo_return = get_address_information();

    if (getaddrinfo_return != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_return));
        exit(EXIT_FAILURE);
    }

    if (print_result() == -1)
        return (EXIT_FAILURE);

    freeaddrinfo(g_ping->result);           /* No longer needed */
    return (EXIT_SUCCESS);
}
