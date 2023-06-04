#include "ft_ping.h"

int validate_icmp_checksum()
{
	t_icmp_packet_reply reply;
	uint16_t stored_checksum;

	reply = g_ping->icmp_echo_reply;
	stored_checksum = reply.icmp_header.checksum;
	reply.icmp_header.checksum = 0;

	if (stored_checksum == compute_icmp_checksum((uint16_t *)&reply, sizeof(reply)))
		return VALID;
	else
		fprintf(stdout, "INVALID PACKET \n");

	return INVALID;
}

// rfc1071
// the icmp checksum field is 2 bytes or 16 bits
// 1 bytes = 1 octet
// 1 octet = 8 bits
// 2 bytes = 16 bits
uint16_t compute_icmp_checksum(uint16_t *addr, int size)
{
	// we store the sum in 2 times the size of the checksum
	uint16_t checksum = 0;
	uint32_t sum = 0;

	// we are adding
	while (size > 1)
	{
		sum += *addr++;
		size -= sizeof(uint16_t);
	}

	// if we have an odd size of of bytes
	if (size > 0)
	{
		sum += *(u_char *)addr;
	}

	/*  Fold 32-bit sum to 16 bits */
	// tant qu'il reste des bits dans la partie de gauche
	while ((sum >> 16) != 0)
		sum = (sum >> 16) + (sum & 0xffff);
	// sum = (sum & 0xffff) + (sum >> 16);
	// the and (&) operator example : x & 0xFFFF = x
	// the >> operator shift to the right

	// the 1's complement of a binary number is the value obtain by inverting all the bits in the binary
	// this name come from the fact that adding the two value would result in all 1 activated;
	// the ~ (NOT) operator invert all bits
	checksum = (uint16_t)~sum;
	return checksum;
}
