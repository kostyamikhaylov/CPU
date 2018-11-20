#ifndef HEADER
#define HEADER

#define DATA_TYPE int 
#define HASH_SUM_TYPE long int
#define CANARY_SYMB 0x69

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
#include <math.h>

typedef struct Stack
{
	DATA_TYPE *data;
	struct Stack *next;
} stack;

int Execute (int fd);

stack *CreateStack (DATA_TYPE x);
stack *Push (stack *stk, DATA_TYPE x);
DATA_TYPE Pop (stack **stk);
void DeleteStack (stack **stk);
int Verify (stack *stk);
int DumpElem (stack *elem);

#include "Stack.c"
#include "CPU.c"

#endif	//HEADER
