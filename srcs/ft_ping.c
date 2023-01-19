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

int main(int ac, char **av){

    int                     socket;
    struct addrinfo         hints;
    struct addrinfo         *result;
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

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    // hints.ai_socktype = SOCK_RAW; /* RAW socket */
    hints.ai_socktype = SOCK_DGRAM; /* RAW socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    socket = getaddrinfo(NULL, av[2], &hints, &result);

    if (socket != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(socket));
        exit(EXIT_FAILURE);
    }

       // socket_fd = socket(int socket_family, int socket_type, int protocol);
    // sendto(socket_fd, buf, len, flags, NULL, 0);

    return (EXIT_SUCCESS);
}
