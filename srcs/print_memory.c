#include "ft_ping.h"
#include <sys/types.h>

void print_memory(void *buffer, ssize_t size){

    uint8_t* octet;
    ssize_t iterator;

    iterator = 0;

    octet = buffer;

    fprintf(stdout, "start memory for %zu bytes :\n", size);
    while (iterator < size) {
        fprintf(stdout, " %02hhX", octet[iterator]);
        if ( (iterator % 4) - 3  == 0 && iterator != 0)
            fprintf(stdout, "|");
        if ( (iterator % 8) - 3  == 0 && iterator != 0)
            fprintf(stdout, "\n");
        iterator++;
    }
    fprintf(stdout, "\nend memory!\n");
}
