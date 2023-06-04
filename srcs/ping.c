#include "ft_ping.h"
#include "libft.h"

t_ft_ping *g_ping;

int main(int ac, char **av)
{
	// struct addrinfo         *rp;
	t_ft_ping ping_data;

	g_ping = &ping_data;

	ft_memset(&g_ping->ip_address, 0, sizeof(g_ping->ip_address));
	g_ping->sequence_number = 1;
	g_ping->valide_message = 0;
	g_ping->packets_sent = 0;
	g_ping->program_id = getpid();
	// set with an option
	g_ping->count = 1;
	g_ping->count = 0;

	signal(SIGINT, stop_program);

	g_ping->parsed_arguments = ft_lexer(ac, av);

	if (g_ping->parsed_arguments != ac)
	{
		fprintf(stderr, "we didnt parse all the argument : arguments_parser = %i | ac = %i \n", g_ping->parsed_arguments, ac);
		return (EXIT_FAILURE);
	}
	else if (g_ping->parsed_arguments == ac)
	{
		// fprintf (stderr, "lexer is ok: ac = %i\n", g_ping->arguments_parser);
	}
	// test_option();

	if (g_ping->host == NULL)
		fprintf(stderr, "No Host");

	define_raw_socket();

	create_socket_file_descriptor(&g_ping->socket);

	setting_socket_option();

	// [0] ping google.com
	// PING google.com (172.217.20.206): 56 data bytes
	fprintf(stdout, "PING %s (%s) %lu data bytes\n", g_ping->host, g_ping->ip_address, PACKET_DATA_SIZE);

	while (g_ping->count != g_ping->sequence_number && g_ping->sequence_number != 0)
	{
		create_icmp_packet();

		sending_icmp_echo_request(g_ping->socket.file_descriptor);

		waiting_icmp_echo_reply();
		// each received_message() will also print information of this type;
		// 64 bytes from 8.8.8.8: icmp_seq=4 ttl=116 time=10.8 ms

		// when the iteration is finished
		// we display statistics free everythintg
		// and exit
		// --- 8.8.8.8 ping statistics ---
		// 4 packets transmitted, 4 received, 0% packet loss, time 3005ms
		// rtt min/avg/max/mdev = 9.019/11.103/12.798/1.395 ms
		g_ping->sequence_number++;
		// fprintf(stdout, "count %d, sequence number: %d", count, g_ping->sequence_number);
		usleep(1000000);
	}

	// fprintf(stdout, "this is after the signal handling\n");
	fprintf(stdout, "--- %s ping statistics ---\n", g_ping->host);
	fprintf(stdout, "%zu packets transmitted, %i received\n", g_ping->packets_sent, g_ping->valide_message);
	// if (g_ping->valide_message != 0 && g_ping->message_sent != 0)
	//     fprintf(stdout, "%f%% packets loss, times ??\n", (float)(g_ping->valide_message / g_ping->message_sent));
	// else
	//     fprintf(stdout, "100%% packets loss, times ??\n");

	return (EXIT_SUCCESS);
}
