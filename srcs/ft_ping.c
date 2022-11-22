#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


// range should be 0.0.0.0 to 255.255.255.255
//
//
int check_host_validity(char *host){

    if (!host){
        fprintf (stderr, "%s\n", "ping: unknown host\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}


int check_option_validity(char *opt){
    size_t iterator;
    iterator = 0;

    fprintf (stderr, "%s %s %s", "ping: invalid option --", opt,"Try 'ping --help' or 'ping --usage' for more information\n");
    return (0);
}


// tokenisation 
// can be an option -- or -
// can be the host
int ft_lexer_parser(int ac, char **av){

    size_t arg_number;
    size_t iterator;
    size_t opts_number;
    iterator = 0;
    arg_number = (size_t)ac;

    while (iterator < arg_number){

        //check if its on option
        if (av[iterator][0] == '-' || (av[iterator][0] == '-' && av[iterator][1] == '-')){
            //check option validity
            check_option_validity(av[iterator]);
            // activate options
            // could be in a struct
        }

        // this is the host that we should treat
        if (av[iterator][0] != '-'){
        }
        fprintf(stdout,  "%s\n", av[iterator]);
        iterator++;
    }


    if (ac == 1) {
        fprintf (stderr, "%s %s", "ping: missing host operand\n", "Try 'ping --help' or 'ping --usage' for more information\n");
        return 1;
    }
    if (ac <= 2) {
        fprintf (stderr, "%s", "ping: usage error: Destination address required\n");
        return 1;
    }
    else {
        fprintf (stdout, "%s\n", "sufficiant argument");
        return 0;
    }
    return 2;
}

int main(int ac, char **av){

    if (ft_lexer_parser(ac, av) == 0)
    {
        // fprintf (stderr, "%s\n", av[1]);
        fprintf (stderr, "%i\n", EXIT_FAILURE);
        fprintf (stderr, "%i\n", EXIT_SUCCESS);
        fprintf (stderr, "%i\n", EOF);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
