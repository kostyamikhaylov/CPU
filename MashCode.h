#ifndef HEADER
#define HEADER

#define ASSERT(CONDITION) 																	\
	if(!(CONDITION))																		\
	{																						\
		fprintf (stderr, "Assertation failed, file %s, function %s, line %d\n",				\
				__FILE__, __PRETTY_FUNCTION__, __LINE__);									\
		exit (EXIT_SUCCESS);																\
	}

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int Translate (int fd_rd, int fd_wr);

#include "MashCode.c"

#endif	//HEADER
