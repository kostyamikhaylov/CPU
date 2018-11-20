#ifndef EXEC
#define EXEC

#define RAM_SIZE 4096

char ram[RAM_SIZE];

struct Registers
{
	DATA_TYPE rax;
	DATA_TYPE rbx;
	DATA_TYPE rcx;
	DATA_TYPE rdx;
} regs;

/*
-----------------------------------------
____________LIST OF COMMANDS_____________


0	end
1	push
2	pop
3	add
4	sub
5	mul
6	div
7	in
8	out
9	call
10	ret
11	push rax
12	push rbx
13	push rcx
14	push rdx
21	pop rax
22	pop rbx
23	pop rcx
24	pop rdx
30	jmp [label/cmdnum]
31	ja
32	jb
33	je
34	jae
35	jbe
36	jne
41	sqrt
42	sqr
101	push [adr/reg]	adr >= 0, reg < 0
102	pop [adr/reg]	adr >= 0, reg < 0


-----------------------------------------
*/

/*
-----------------------------------------
____________LIST OF REGISTERS____________


1	rax
2	rbx
3	rcx
4	rdx

-----------------------------------------
*/

int Execute (int fd);

long int GetFileSize (int fd);
int ExecStr (const char *str, long int len);
int MemCopyAdr (DATA_TYPE dest, DATA_TYPE *x_ptr, int size);
DATA_TYPE MemGet (const char *str);
long int FindCmd (const char *str, int cmd_num, long int len);
int ArgsNum (int cmd);
int CmdNum (const char *str, long int position, long int len);

int Execute (int fd)
{
	ASSERT (fd >= 0);

	long int size = 0;
	char *str = NULL;

	size = GetFileSize (fd);
	str = (char *) calloc (size + 1, sizeof (*str));

	lseek (fd, 0, SEEK_SET);
	read (fd, str, size);

//	puts (str);
	ExecStr (str, size);

	free (str);
	return 0;
}

long int GetFileSize (int fd)
{
	long int bgng = 0, end = 0;

	ASSERT (fd >= 0);
	
	bgng = lseek (fd, 0, SEEK_SET);
	end = lseek (fd, 0, SEEK_END);
	
	return end - bgng;
}

int ExecStr (const char *str, long int len)
{
	ASSERT (str);

	long int i = 0;
	stack *stk = NULL;
	DATA_TYPE x = (DATA_TYPE) 0, x1 = (DATA_TYPE) 0, x2 = (DATA_TYPE) 0;

	while (i < len)
	{
		switch (str[i])
		{
			case 0:
				return 0;
				break;

			case 1:
				stk = (stk) ? Push (stk, (DATA_TYPE) str[i + 1]) : CreateStack ((DATA_TYPE) str[i + 1]);
				i += 4;
				break;

			case 2:
				x = Pop (&stk);
				MemCopyAdr ((DATA_TYPE) str[i + 1], &x, sizeof (DATA_TYPE));
				i += 4;
				break;
			
			case 3:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				x = x1 + x2;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 4:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				x = x1 - x2;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 5:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				x = x1 * x2;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 6:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				x = x1 / x2;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;

			case 7:
				scanf ("%d", &x);
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 8:
				x = Pop (&stk);
				printf ("Out: %d\n", x);
				break;
			
			case 9:
//				printf ("call: i = %ld, cmdnum = %d, ptr = %d\n", i, CmdNum (str, i, len), (int) str[i + 1]);
				stk = (stk) ? Push (stk, (DATA_TYPE) (CmdNum (str, i, len) + 1)) : CreateStack ((DATA_TYPE) (CmdNum (str, i, len) + 1));
				i = FindCmd (str, (int) str[i + 1], len);
				i--;
				break;
			
			case 10:
				x = Pop (&stk);
//				printf ("ret: i = %ld, cmdnum = %d, ptr = %d\n", i, CmdNum (str, i, len), (int) x);
				i = FindCmd (str, (int) x, len);
				i--;
				break;
			
			case 11:
				x = regs.rax;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 12:
				x = regs.rbx;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 13:
				x = regs.rcx;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 14:
				x = regs.rdx;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 21:
				x = Pop (&stk);
				regs.rax = x;
				break;
			
			case 22:
				x = Pop (&stk);
				regs.rbx = x;
				break;
			
			case 23:
				x = Pop (&stk);
				regs.rcx = x;
				break;
			
			case 24:
				x = Pop (&stk);
				regs.rdx = x;
				break;
			
			case 30:
				i = FindCmd (str, (int) str[i + 1], len);
				i--;
				break;
			
			case 31:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 > x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 32:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 < x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 33:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 == x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 34:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 >= x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 35:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 <= x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 36:
				x2 = Pop (&stk);
				x1 = Pop (&stk);
				if (x1 != x2)
				{
					i = FindCmd (str, (int) str[i + 1], len);
					i--;
				}
				else
					i += sizeof (DATA_TYPE);
				break;
			
			case 41:
				x = Pop (&stk);
				x = (DATA_TYPE) sqrt (x);
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 42:
				x = Pop (&stk);
				x = x * x;
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				break;
			
			case 101:
				x = MemGet (str + i + 1);
				stk = (stk) ? Push (stk, x) : CreateStack (x);
				i += 4;
				break;

			case 102:
				x = Pop (&stk);
				MemCopyAdr ((DATA_TYPE) str[i + 1], &x, sizeof (DATA_TYPE));
				i += 4;
				break;
			
			default:
				fprintf (stderr, "Can't execute command №%d\n", (int) str[i]);
				return 1;
				break;
		}

		i++;
	}

	return 0;
}

int MemCopyAdr (DATA_TYPE s, DATA_TYPE *x_ptr, int size)
{
	ASSERT (x_ptr);

	int i = 0;
	char c = 0;

	if (s < 0)
	{
		switch (s)
		{
			case -1:
				ram[regs.rax * sizeof (int)] = * (int *) x_ptr;
				return 0;
				break;

			case -2:
				ram[regs.rbx * sizeof (int)] = * (int *) x_ptr;
				return 0;
				break;

			case -3:
				ram[regs.rcx * sizeof (int)] = * (int *) x_ptr;
				return 0;
				break;

			case -4:
				ram[regs.rdx * sizeof (int)] = * (int *) x_ptr;
				return 0;
				break;
			
			default:
				fprintf (stderr, "Wrong adr %d\n", s);
				return 1;
				break;
		}
	}
	
	ram[s * sizeof (int)] = * (int *) x_ptr;
	
	return 0;
}

DATA_TYPE MemGet (const char *str)
{
	int adr = * (int *) str;
	
	if (adr < 0)
	{
		switch (adr)
		{
			case -1:
				return regs.rax;
				break;

			case -2:
				return regs.rbx;
				break;

			case -3:
				return regs.rcx;
				break;

			case -4:
				return regs.rdx;
				break;
			
			default:
				fprintf (stderr, "Wrong adr %d\n", adr);
				return 0;
				break;
		}
	}
	
	return (DATA_TYPE) *((char *) ram + adr * sizeof (int));
}

long int FindCmd (const char *str, int cmd_num, long int len)
{
	long int i = 0;
	int cur_cmd = 1;
	int args_num = 0;
	int cmd = 0;

	while (i < len)
	{
		if (cur_cmd == cmd_num)
			return i;

		cmd = (int) str[i];
		args_num = ArgsNum (cmd);
		while (args_num > 0)
		{
			i += sizeof (DATA_TYPE);
			args_num--;
		}
		
		cur_cmd++;
		i++;
	}

	return len;
}

int CmdNum (const char *str, long int position, long int len)
{
	long int i = 0;
	int cur_cmd = 1;
	int args_num = 0;
	int cmd = 0;

	while (i < len)
	{
		if (i == position)
			return cur_cmd;

		cmd = (int) str[i];
		args_num = ArgsNum (cmd);
		while (args_num > 0)
		{
			i += sizeof (DATA_TYPE);
			args_num--;
		}
		
		cur_cmd++;
		i++;
	}

	return cur_cmd;
}

int ArgsNum (int cmd)
{
	switch (cmd)
	{
		case 0: return 0; break;
		case 1: return 1; break;
		case 2: return 1; break;
		case 3: return 0; break;
		case 4: return 0; break;
		case 5: return 0; break;
		case 6: return 0; break;
		case 7: return 0; break;
		case 8: return 0; break;
		case 9: return 1; break;
		case 10: return 0; break;
		case 11: return 0; break;
		case 12: return 0; break;
		case 13: return 0; break;
		case 14: return 0; break;
		case 21: return 0; break;
		case 22: return 0; break;
		case 23: return 0; break;
		case 24: return 0; break;
		case 30: return 1; break;
		case 31: return 1; break;
		case 32: return 1; break;
		case 33: return 1; break;
		case 34: return 1; break;
		case 35: return 1; break;
		case 36: return 1; break;
		case 41: return 0; break;
		case 42: return 0; break;
		case 101: return 1; break;
		case 102: return 1; break;
		default: return -1; break;
	}
}

#endif	// EXEC

