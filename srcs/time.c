#include "ft_ping.h"
#include <sys/select.h>
#include <time.h>

double get_time_diff(struct timeval *start, struct timeval *end)
{
	// return a diff in nMicroseconds;
	double diff;

	// diff until the second;
	diff = (double)(end->tv_sec - start->tv_sec) * MU;
	// fprintf (stdout, "diff: %e\n", diff);
	// printf("Time Diff Sec: %li\n Time diff MuSec: %li\n", time_difference,
	// Mu_time_difference);
	// printf("start: %li\nend: %li\n", start->tv_sec,
	// end->tv_sec);
	// printf("Previous MU Second: %li\nCurrent MU Seconds: %li\n",
	// message_timestamp->tv_usec, current_timestamp.tv_usec);
	// then we diff the Mu second and add it to the diff;
	diff += (double)(end->tv_usec - start->tv_usec);
	// fprintf (stdout, "diff: %e\n", diff);
	// printf("start: %li\nend: %li\n", start->tv_usec, end->tv_usec);

	return diff;
}

double get_current_time_diff(struct timeval *message_timestamp)
{
	struct timeval current_timestamp;
	gettimeofday(&current_timestamp, NULL);

	double diff = get_time_diff(message_timestamp, &current_timestamp);

	return (diff);
}

void *set_timestamp(void *destination)
{
	// there is the struct timespec that can store nano second
	struct timeval timestamp;

	// the second argument can be set te specify a particular timezone
	// we putt NULL to let the kernel set the timezone
	gettimeofday(&timestamp, NULL);

	// Here we print the time that gettimeofday give us
	// printf("Seconds: %ld\nMicroseconds: %ld\n", timestamp.tv_sec,
	// timestamp.tv_usec); printf("Human time: %s", ctime(&timestamp.tv_sec));

	// we then copy the content of this struct in our void *;
	ft_memcpy(destination, &timestamp, sizeof(struct timeval));

	return (destination);
}
