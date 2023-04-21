#include "ft_ping.h"
#include <signal.h>


void stop_program(int signal_number){

    // fprintf(stdout, "We received the signal number %d to stop\n", signal_number);
    // freeaddrinfo(g_ping->result);           /* No longer needed */
    if (signal_number == SIGINT){
        // g_ping->message_sent = g_ping->sequence_number;
        g_ping->sequence_number = 0;
    }
    // fprintf(stdout, "We have set the sequence_number to %i\n", g_ping->sequence_number);
}


