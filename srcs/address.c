#include "ft_ping.h"


void ip_address_to_struct(){


    int s;

    char internet_address_string[INET_ADDRSTRLEN];

    // we set the family to ip version 4 no other possibilities
    // ft_memset(g_ping->ipv4_address, 0, sizeof(&g_ping->ipv4_address));
    // ft_memset(&g_ping->ipv4_address->sin_addr, 0, sizeof(g_ping->ipv4_address->sin_addr));
    g_ping->ipv4_address.sin_family = AF_INET;
    // fprintf (stdout, "family  = %i\n", g_ping->ipv4_address.sin_family);
    g_ping->ipv4_address.sin_port = 0;
    // fprintf (stdout, "port = %i\n", g_ping->ipv4_address.sin_port);


    fprintf(stderr, "String Ip address: %s\n", g_ping->ip_address);
    s = inet_pton(AF_INET, g_ping->ip_address, &g_ping->ipv4_address.sin_addr); // transform an IP address string to binary
    if (s == 0)
        fprintf (stderr, "not a valid address for the family : %i .\n", s);

    if (s == -1){
        perror("inet_pton");
        fprintf (stderr, "Error : %s | on function pton.\n", strerror(errno));
    }

    // fprintf (stderr, "HOST : [%s]\n", g_ping->host);
    s = inet_pton(AF_INET, g_ping->host, &g_ping->ipv4_address.sin_addr); // transform an IP address string to binary

    unsigned char* bytes = (unsigned char*)&g_ping->ipv4_address.sin_addr;
    // print_memory(&g_ping->ipv4_address.sin_addr , sizeof(g_ping->ipv4_address.sin_addr));
    printf("Bytes IP address: %d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);


    if ( g_ping->ipv4_address.sin_family == AF_INET)
        fprintf (stderr, "not bad\n" );
    // we clean this
    ft_memset(&internet_address_string, '\0', sizeof(internet_address_string));
    if (inet_ntop(AF_INET, &g_ping->ipv4_address.sin_addr, internet_address_string, INET_ADDRSTRLEN) == NULL){
        perror("inet_ntop");
        fprintf (stderr, "Error : %s | on function ntop.\n", strerror(errno));
    }
}


void hostname_to_struct(){

    int                     getaddrinfo_return;
    struct addrinfo         hints;
    struct addrinfo         *result;

    ft_memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 */
    hints.ai_socktype = 0; /* RAW socket */
    // hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    // hints.ai_protocol = 0;          /* Any protocol */
    // hints.ai_canonname = NULL;
    // hints.ai_addr = NULL;
    // hints.ai_addr = g_ping->host;
    // hints.ai_next = NULL;

    // put the hostname resolved in the ip_address;

    fprintf(stderr, "hostname: %s\n", g_ping->hostname);
    // getaddrinfo_return = getaddrinfo(g_ping->hostname, NULL, &hints, &g_ping->result);
    getaddrinfo_return = getaddrinfo(g_ping->hostname, NULL, &hints, &result);
    g_ping->ipv4_address = *(struct sockaddr_in *) result->ai_addr;
    char internet_address_string[INET_ADDRSTRLEN];
    inet_ntop(hints.ai_family, &g_ping->ipv4_address.sin_addr, internet_address_string, sizeof internet_address_string);
    fprintf(stdout, "%s | getaddrinfo return : %i\n", internet_address_string, getaddrinfo_return);

    unsigned char* bytes = (unsigned char*)&g_ping->ipv4_address.sin_addr;
    // hex_dump(&g_ping->ipv4_address.sin_addr , sizeof(g_ping->ipv4_address.sin_addr));
    printf("Bytes IP address: %d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    g_ping->ip_address = internet_address_string;
    // fprintf (stderr, "address = %s\n", (char *)g_ping->result->ai_addr);
    // fprintf (stderr, "family = %i\n", g_ping->result->ai_family);
    if (getaddrinfo_return != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(getaddrinfo_return));
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */
}

