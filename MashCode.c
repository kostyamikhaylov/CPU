#ifndef MASH
#define MASH

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


int Translate (int fd_rd, int fd_wr);

long int GetFileSize (int fd);
int TranslateStrToMashCode (const char *str, char *new_str);
int ReadCmd (const char *str, int *arg, long int *str_ptr);
int ArgsNum (int cmd);

int Translate (int fd_rd, int fd_wr)
{
	ASSERT (fd_rd >= 0);
	ASSERT (fd_wr >= 0);

	long int size = 0;
	char *str = NULL;
	char *new_str = NULL;

	size = GetFileSize (fd_rd);
	str = (char *) calloc (size + 1, sizeof (*str));
	new_str = (char *) calloc (4 * size + 1, sizeof (*new_str));

	lseek (fd_rd, 0, SEEK_SET);
	read (fd_rd, str, size);

//	puts (str);
	size = TranslateStrToMashCode (str, new_str);

	lseek (fd_wr, 0, SEEK_SET);
	write (fd_wr, new_str, size);

	free (str);
	free (new_str);
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

int TranslateStrToMashCode (const char *str, char *new_str)
{
	ASSERT (str);
	ASSERT (new_str);

	long int i = 0, j = 0;
	long int len = strlen (str);
	int argument = 0;
	int *arg = &argument;

	while (i < len)
	{
		if (isspace (str[i]))
		{
//			printf ("Space\n");
			i++;
			continue;
		}

		switch (ReadCmd (str + i, arg, &i))
		{
			case 0:
				new_str[j++] = (char) 0;
				break;

			case 1:
				new_str[j++] = (char) 1;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;

			case 2:
				new_str[j++] = (char) 2;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 3:
				new_str[j++] = (char) 3;
				break;
			
			case 4:
				new_str[j++] = (char) 4;
				break;
			
			case 5:
				new_str[j++] = (char) 5;
				break;
			
			case 6:
				new_str[j++] = (char) 6;
				break;

			case 7:
				new_str[j++] = (char) 7;
				break;
			
			case 8:
				new_str[j++] = (char) 8;
				break;
			
			case 9:
				new_str[j++] = (char) 9;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 10:
				new_str[j++] = (char) 10;
				break;
			
			case 11:
				new_str[j++] = (char) 11;
				break;
			
			case 12:
				new_str[j++] = (char) 12;
				break;
			
			case 13:
				new_str[j++] = (char) 13;
				break;
			
			case 14:
				new_str[j++] = (char) 14;
				break;
			
			case 21:
				new_str[j++] = (char) 21;
				break;
			
			case 22:
				new_str[j++] = (char) 22;
				break;
			
			case 23:
				new_str[j++] = (char) 23;
				break;
			
			case 24:
				new_str[j++] = (char) 24;
				break;
			
			case 30:
				new_str[j++] = (char) 30;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 31:
				new_str[j++] = (char) 31;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 32:
				new_str[j++] = (char) 32;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 33:
				new_str[j++] = (char) 33;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 34:
				new_str[j++] = (char) 34;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 35:
				new_str[j++] = (char) 35;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 36:
				new_str[j++] = (char) 36;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 41:
				new_str[j++] = (char) 41;
				break;
			
			case 42:
				new_str[j++] = (char) 42;
				break;
			
			case 101:
				new_str[j++] = (char) 101;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			case 102:
				new_str[j++] = (char) 102;
				memcpy (new_str + j, arg, sizeof (int));
				j += sizeof (int);
				break;
			
			default:
				fprintf (stderr, "Wrong symbol\n");
				break;
		}
	}
	
	return j;
}

int ReadCmd (const char *str, int *arg, long int *str_ptr)
{
	ASSERT (str);
	ASSERT (arg);
	ASSERT (str_ptr);

	static int enter = 0;
	int param_num = 0;
	int i = 0, i_save = 0;
	int command = -1;
/*
	printf ("Enter № %d\n", enter++);
	printf ("String begins with:\n-----\n");
	write (0, str, 15);
	printf ("\n------\n");
	printf ("str_ptr = %ld\n", *str_ptr);
*/
	sscanf (str, "%d", &command);

	while (isdigit (str[i++]))
		;
	*str_ptr += i;
	i_save = i;
	
	param_num = ArgsNum (command);

	if (param_num == 0)
		;
	else if (param_num == 1)
	{
		sscanf (str + i, "%d", arg);
		
		while (isspace (str[i++]))
			;
		if (str[i] == '-')
			i++;
		while (isdigit (str[i++]))
			;

		*str_ptr += i - i_save;
	}
/*
	printf ("str_ptr = %ld\n", *str_ptr);
	printf ("ReadCmd: cmd = %d, arg = %d\n", command, (param_num == 0) ? 666 : *arg);
	printf ("ReadCmd: cur symb is <%c>\n", *(str + (int) *str_ptr));
*/
	return command;
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

#endif	// MASH

