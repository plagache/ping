#include "ft_ping.h"

unsigned long timestamp_compare(struct timeval message_timestamp){

    struct timeval current_timestamp;
    // the second argument of gettimeofday can be set:
    // to specifique timezone
    // we set NULL to let the kernel set the timezone
    gettimeofday(&current_timestamp, NULL);

    unsigned long time_difference;
    time_difference = 0;
    // lettre grec Mu 10^(-6) is write u
    unsigned long Mu_time_difference;
    Mu_time_difference = 0;


    time_difference = current_timestamp.tv_sec - message_timestamp.tv_sec;
    Mu_time_difference = current_timestamp.tv_usec - message_timestamp.tv_usec;

    unsigned long diff = time_difference + Mu_time_difference;

    return (diff);
}


void *timestamp_creation(void *destination){

    // there is the struct timespec that can store nano second
    struct timeval timestamp;

    // the second argument can be set te specify a particular timezone
    // we putt NULL to let the kernel set the timezone
    gettimeofday(&timestamp, NULL);

    // Here we print the time that gettimeofday give us
    // printf("Seconds: %ld\nMicroseconds: %ld\n", timestamp.tv_sec, timestamp.tv_usec);
    // printf("Human time: %s", ctime(&timestamp.tv_sec));

    // we then copy the content of this struct in our void *;
    ft_memcpy(destination, &timestamp, sizeof(struct timeval));

    return (destination);
}

