#include <stddef.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ft_ping.h"

int main(int ac, char **av){

    if (ft_lexer(ac, av) != ac)
    {
        // fprintf (stderr, "%s\n", av[1]);
        // fprintf (stderr, "%i\n", EXIT_FAILURE);
        // fprintf (stderr, "%i\n", EXIT_SUCCESS);
        // fprintf (stderr, "%i\n", EOF);
        // fprintf(stdout, "%lu ||| %lu\n", sizeof(t_options_ping), sizeof(unsigned int));
        // inet_ntop(AF_INET, const void *restrict cp, char *restrict buf, socklen_t len)
        // fprintf (stderr, "error on lexer ac = %i\n", ac);
        // fprintf (stderr, "error on lexer ac = %i\n", ft_lexer(ac, av));
        test_option();
        return (EXIT_FAILURE);
    }
    else if (ft_lexer(ac, av) == ac) {
        fprintf (stderr, "lexer is ok; ac = %i\n", ft_lexer(ac, av));
        test_option();
    }
    return (EXIT_SUCCESS);
}
