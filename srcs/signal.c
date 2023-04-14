#include "ft_ping.h"


void stop_program(int signal_number){

    fprintf(stdout, "We received the signal number %d to stop\n", signal_number);
    // freeaddrinfo(g_ping->result);           /* No longer needed */
    g_ping->sequence_number = 0;
    fprintf(stdout, "We have set the sequence_number to %i\n", g_ping->sequence_number);
}


