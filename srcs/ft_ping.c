#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "ft_ping.h"

// verify the range
// range should be 0.0.0.0 to 255.255.255.255
int parse_host(char *host){

    if (!host){
        fprintf (stderr, "%s\n", "no 'G' host ?\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
// 2 possible format for an host
// Ip adresse : format 8.8.8.8
// host name : format text.com
// there is a list of possible local for .com .org ...
int is_an_host(char *argument){
    size_t iterator;
    iterator = 0;

    if (isdigit(argument[iterator]) != 0){
        fprintf(stdout, "ARG IS AN HOST [ V ] : %s\n", argument);
        fprintf(stdout, "HOST IS OF TYPE 0.0.0.0\n");
        return 1;
    }
    if (isalpha(argument[iterator]) != 0){
        fprintf(stdout, "ARG IS AN HOST [ V ] : %s\n", argument);
        fprintf(stdout, "HOST IS OF TYPE text.com\n");
        return 1;
    }
    fprintf(stdout, "ARG IS AN HOST [ X ] : %s\n", argument);
    return (EXIT_SUCCESS);
}


int parse_option(char *opt){
    size_t iterator;
    iterator = 1;
    if (opt[1] == '-')
        iterator = 2;

    while (opt[iterator] != '\0') {
        if (strchr(PING_OPTIONS, opt[iterator]) != NULL)
            fprintf(stdout, "OPTS IS VALID [ V ] : %c\n", opt[iterator]);
            // activate options in struct;
        else {
            fprintf (stderr, "%s '%c'\n%s", "ping: invalid option --", opt[iterator],"Try 'ping --help' or 'ping --usage' for more information\n");
            return (EXIT_FAILURE);
        }
        iterator ++;
    }
    return (0);
}


// an option can be '-' or '--'
int is_an_option(char *argument){

    if (argument[0] == '-' || (argument[0] == '-' && argument[1] == '-')){
        fprintf(stdout, "ARG IS OPTS [ V ] : %s\n", argument);
        return 0;
    }
    fprintf(stdout, "ARG IS OPTS [ X ] : %s\n", argument);
    return 1;
}

// tokenisation
// can be an option -- or -
// can be an host host(format)
int ft_lexer(int ac, char **av){

    size_t arg_number;
    size_t iterator;
    // size_t opts_number;
    // av[0] is the programme name
    iterator = 1;
    arg_number = (size_t)ac;

    while (iterator < arg_number){

        //check if its on option
        if (is_an_option(av[iterator]) == 0){
            //check option validity
            parse_option(av[iterator]);
            // activate options
        }

        // is this the host that we should treat ?
        if (is_an_host(av[iterator]) == 0){
            // if (parse_host(av[iterator])){
            //
            // }
        }
        iterator++;
    }

    fprintf(stdout, "--- we iterate on each arguments. ---\n\n");
    return (iterator - 1);
}

int main(int ac, char **av){

    if (ft_lexer(ac, av) != ac)
    {
        // fprintf (stderr, "%s\n", av[1]);
        // fprintf (stderr, "%i\n", EXIT_FAILURE);
        // fprintf (stderr, "%i\n", EXIT_SUCCESS);
        // fprintf (stderr, "%i\n", EOF);
        // fprintf (stderr, "%s\n", ping_options);
        // fprintf (stderr, "%i\n", ft_lexer(ac, av));
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
