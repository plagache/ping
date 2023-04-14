#include "ft_ping.h"

// 2 types of function here 
// detecte an option
// set the options

// verify the range
// range should be 0.0.0.0 to 255.255.255.255
int parse_host(char *host){

    if (!host){
        fprintf (stderr, "%s\n", "no 'G' host ?\n");
        return (EXIT_FAILURE);
    }
    else if (host) {
        g_ping->host = host;
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
        return (EXIT_SUCCESS);
    }

    if (isalpha(argument[iterator]) != 0){
        fprintf(stdout, "ARG IS AN HOST [ V ] : %s\n", argument);
        fprintf(stdout, "HOST IS OF TYPE text.com\n");
        return (EXIT_SUCCESS);
    }
    fprintf(stdout, "ARG IS AN HOST [ X ] : %s\n", argument);
    return (EXIT_FAILURE);
}


int activate_options(char c){

    if (c == 'v')
        g_ping->options.verbose |= ON;
    if (c == 'h')
        g_ping->options.help = g_ping->options.help | ON;
    return 0;

}


int parse_option(char *opt){
    size_t iterator;
    iterator = 1;
    if (opt[1] == '-')
        iterator = 2;

    while (opt[iterator] != '\0') {
        if (strchr(PING_OPTIONS, opt[iterator]) != NULL)
            activate_options(opt[iterator]);
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
        // fprintf(stdout, "ARG [ %s ] \\ IS / OPTS\n", argument);
        return 0;
    }
    // fprintf(stdout, "ARG [ %s ] / IS NOT \\ OPTS\n", argument);
    return 1;
}

// tokenisation
// We have 3 type of token in our programme
// can be an option -- or -
// can be an host host(format)
// can be the programme name (1st argument)
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
            if (parse_option(av[iterator]) == EXIT_FAILURE)
                return EXIT_FAILURE;
            // activate options
        }

        // is this the host that we should treat ?
        else if (is_an_host(av[iterator]) == SUCCESS){
            if (parse_host(av[iterator]) == EXIT_FAILURE)
                return EXIT_FAILURE;
        }
        iterator++;
    }
    // fprintf(stdout, "--- we iterate on each arguments without failing . ---\n-- maybe a good sign --\n\n");
    return (iterator);
}


int test_option(){

    if (g_ping->options.verbose & ON)
        fprintf(stdout, "--- we selected -v option. ---\n\n");
    if (g_ping->options.help & ON)
        fprintf(stdout, "--- we selected -h option. ---\n\n");

    fprintf(stdout, "--- Number of argument parsed = %i. ---\n\n", g_ping->parsed_arguments);

    return 0;
}


