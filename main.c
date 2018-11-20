#include "CPU.h"

#define MASH_CODE "mash_code.txt"


int main (int argc, char** argv)
{
	int fd = -1;

	if (argv[1] != NULL)
	{
		fd = open (argv[1], O_RDONLY);
	}
	else
	{
		fd = open (MASH_CODE, O_RDONLY);
	}
	
	if (fd < 0)
	{
		fprintf (stderr, "Can't open file %s\n", (argv[1] != NULL) ? argv[1] : MASH_CODE);
		exit (EXIT_FAILURE);
	}

	Execute (fd);

	close (fd);
	return 0;
}
