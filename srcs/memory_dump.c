#include "ft_ping.h"

void hex_dump(void *buffer, ssize_t size)
{
	uint8_t *octet;
	ssize_t counter;

	octet = buffer;
	counter = 0;

	while (counter < size)
	{
		if (counter != 0 && counter % 8 == 0)
			fprintf(stdout, " ");
		if (counter != 0 && counter % 16 == 0)
			fprintf(stdout, "\n");
		fprintf(stdout, " %2.2X", octet[counter]);
		counter++;
	}
	fprintf(stdout, "\n ------------[ hexdump : %zu bytes ]------------\n", size);
}
