#include "ft_ping.h"
#include "libft.h"


void stop_program(int signal_number){

    fprintf(stdout, "We received the signal to stop %d\n", signal_number);
    fprintf(stdout, "this is the end of the program");
    // freeaddrinfo(g_ping->result);           /* No longer needed */
    g_ping->sequence_number = 0;
    exit(EXIT_SUCCESS);
    fprintf(stdout, "this is after the exit of the program");

}


