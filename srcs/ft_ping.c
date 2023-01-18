#include <stddef.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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

    t_ft_ping global_ping;

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
    return (EXIT_SUCCESS);
}
