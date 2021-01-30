#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <sys/time.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <signal.h>

/*
int echo_function(){

}
*/

int main(int ac, char **av){

    if (ac != 2) {
        fprintf (stderr, "%s", "ping: usage error: Destination address required\n");
    }
    else {
        fprintf (stderr, "%s\n", av[1]);
    }

    return (0);
}
