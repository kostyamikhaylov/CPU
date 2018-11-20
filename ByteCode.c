#ifndef BYTE
#define BYTE

#define CMD_MAX_LEN 4
#define STR_SIZE 20
#define LABELS_NUMBER 10
#define LABEL_MAX_LEN 20

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

struct label
{
	int cmdnum;
	char name[LABEL_MAX_LEN + 1];
} labels[LABELS_NUMBER];

int op_num = 1;


int Translate (int fd_rd, int fd_wr);

long int GetFileSize (int fd);
int TranslateStrToByteCode (const char *str, char *new_str);
int ReadCmd (const char *str, int *arg, long int *str_ptr);
int ChooseCmd (char *cmd);
long int IntToStr (int arg, char *str);
void ReverseStr (char *str);
int ArgsNum (int cmd);
int ChooseReg (char *cmd);
int ChooseCmdReg (int cmd, int reg);
long int ProcessLabel (const char *str, int *arg, int is_label);
long int FindLabel (const char *str, int *arg);
long int AddLabel (const char *str);
int IsJump (int cmd);

int Translate (int fd_rd, int fd_wr)
{
	ASSERT (fd_rd >= 0);
	ASSERT (fd_wr >= 0);

	long int size = 0;
	char *str = NULL;
	char *new_str = NULL;

	size = GetFileSize (fd_rd);
	str = (char *) calloc (size + 1, sizeof (*str));
	new_str = (char *) calloc (size + 1, sizeof (*new_str));

	lseek (fd_rd, 0, SEEK_SET);
	read (fd_rd, str, size);

//	puts (str);
	TranslateStrToByteCode (str, new_str);
    memset (new_str, '\0', size);
	TranslateStrToByteCode (str, new_str);

	size = strlen (new_str);
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

int TranslateStrToByteCode (const char *str, char *new_str)
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
			i++;
			continue;
		}

		switch (ReadCmd (str + i, arg, &i))
		{
			case 0:
				new_str[j++] = '0';
				new_str[j++] = ' ';
				break;

			case 1:
				new_str[j++] = '1';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;

			case 2:
				new_str[j++] = '2';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 3:
				new_str[j++] = '3';
				new_str[j++] = ' ';
				break;
			
			case 4:
				new_str[j++] = '4';
				new_str[j++] = ' ';
				break;
			
			case 5:
				new_str[j++] = '5';
				new_str[j++] = ' ';
				break;
			
			case 6:
				new_str[j++] = '6';
				new_str[j++] = ' ';
				break;

			case 7:
				new_str[j++] = '7';
				new_str[j++] = ' ';
				break;
			
			case 8:
				new_str[j++] = '8';
				new_str[j++] = ' ';
				break;
			
			case 9:
				new_str[j++] = '9';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 10:
				new_str[j++] = '1';
				new_str[j++] = '0';
				new_str[j++] = ' ';
				break;
			
			case 11:
				new_str[j++] = '1';
				new_str[j++] = '1';
				new_str[j++] = ' ';
				break;
			
			case 12:
				new_str[j++] = '1';
				new_str[j++] = '2';
				new_str[j++] = ' ';
				break;
			
			case 13:
				new_str[j++] = '1';
				new_str[j++] = '3';
				new_str[j++] = ' ';
				break;
			
			case 14:
				new_str[j++] = '1';
				new_str[j++] = '4';
				new_str[j++] = ' ';
				break;
			
			case 21:
				new_str[j++] = '2';
				new_str[j++] = '1';
				new_str[j++] = ' ';
				break;
			
			case 22:
				new_str[j++] = '2';
				new_str[j++] = '2';
				new_str[j++] = ' ';
				break;
			
			case 23:
				new_str[j++] = '2';
				new_str[j++] = '3';
				new_str[j++] = ' ';
				break;
			
			case 24:
				new_str[j++] = '2';
				new_str[j++] = '4';
				new_str[j++] = ' ';
				break;
		
			case -30:	//label
				op_num--;
				break;

			case 30:
				new_str[j++] = '3';
				new_str[j++] = '0';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 31:
				new_str[j++] = '3';
				new_str[j++] = '1';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 32:
				new_str[j++] = '3';
				new_str[j++] = '2';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 33:
				new_str[j++] = '3';
				new_str[j++] = '3';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 34:
				new_str[j++] = '3';
				new_str[j++] = '4';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 35:
				new_str[j++] = '3';
				new_str[j++] = '5';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 36:
				new_str[j++] = '3';
				new_str[j++] = '6';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 41:
				new_str[j++] = '4';
				new_str[j++] = '1';
				new_str[j++] = ' ';
				break;
			
			case 42:
				new_str[j++] = '4';
				new_str[j++] = '2';
				new_str[j++] = ' ';
				break;
			
			case 101:
				new_str[j++] = '1';
				new_str[j++] = '0';
				new_str[j++] = '1';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			case 102:
				new_str[j++] = '1';
				new_str[j++] = '0';
				new_str[j++] = '2';
				new_str[j++] = ' ';
				j += IntToStr (*arg, new_str + j);
				new_str[j++] = ' ';
				break;
			
			default:
				fprintf (stderr, "Can't recognize command\n");
				exit (EXIT_FAILURE);
				break;
		}

		op_num++;
	}
}

int ReadCmd (const char *str, int *arg, long int *str_ptr)
{
	ASSERT (str);
	ASSERT (arg);
	ASSERT (str_ptr);

	static int enter = 0;
	int param_num = 0;
	int i = 0, j = 0, j_old = 0;
	int command = -1;
	int reg = -1;
	char cmd[CMD_MAX_LEN + 1];

	while (islower (str[i]) && (i < CMD_MAX_LEN))
	{
		cmd[i] = str[i];
		i++;
	}

	if ((i == CMD_MAX_LEN + 1) && (str[i - 1] != '\0'))
	{
//		printf ("cmd = %s, i = %d\n", cmd, i);
		*str_ptr += ProcessLabel (str, arg, 1);
/*
		printf ("str_ptr = %ld\n", *str_ptr);
		printf ("ReadCmd: cmd = %d, arg = %d\n", command, (param_num == 0) ? 666 : *arg);
		printf ("ReadCmd: cur symb is <%c>\n", *(str + (int) *str_ptr));
*/
		return -30;
	}
	
	j = i;
	j_old = j;
	

//	printf ("str[j] = '%c', j = %d\n", str[j], j);
	
	while (i <= CMD_MAX_LEN)
	{
		cmd[i++] = '\0';
	}
	
	command = ChooseCmd (cmd);
	
	if (command == -1)
	{
//		printf ("cmd = %s (%d), i = %d\n", cmd, command, i);
		
		j = ProcessLabel (str, arg, 1);
/*
		printf ("str_ptr = %ld\n", *str_ptr);
		printf ("ReadCmd: cmd = %d, arg = %d\n", command, (param_num == 0) ? 666 : *arg);
		printf ("ReadCmd: cur symb is <%c>\n", *(str + (int) *str_ptr));
*/
		*str_ptr += j;
		return -30;
	}
	
	param_num = ArgsNum (command);
	
	while (isspace (str[j]))
		j++;
	*str_ptr += j;
	j_old = j;
	
	if (param_num == 0)
		;
	else if (param_num == 1)
	{
		if (islower (str[j]))
		{
			i = 0;
				
			if (IsJump (command))
			{
//				printf ("cmd = %d\n", command);
				j += ProcessLabel (str + j, arg, 0);
				*str_ptr += j - j_old;
				return command;
			}


			while (islower (str[j]) && (i < CMD_MAX_LEN))
			{
				cmd[i] = str[j];
				i++;
				j++;
			}

			*str_ptr += i;
			
			while (i <= CMD_MAX_LEN)
			{
				cmd[i++] = '\0';
			}

			reg = ChooseReg (cmd);

//			printf ("reg = %d\n", reg);

			command = ChooseCmdReg (command, reg);
			
//			printf ("command = %d\n", command);
		}
		else if (isdigit (str[j]))
		{
			sscanf (str + j, "%d", arg);

			while (isdigit (str[j]))
				j++;
		
			*str_ptr += j - j_old;
		}
		else if (str[j] == '[')
		{
			j++;
			
			if (islower (str[j]))
			{
				i = 0;

				while (islower (str[j]) && (i < CMD_MAX_LEN))
				{
					cmd[i] = str[j];
					i++;
					j++;
				}

				while (i <= CMD_MAX_LEN)
				{
					cmd[i++] = '\0';
				}

				*arg = -1 * ChooseReg (cmd);

//				printf ("reg = %d\n", reg);
				
//				printf ("command = %d\n", command);
			}
			else if (isdigit (str[j]))
			{
				sscanf (str + j, "%d", arg);

				while (isdigit (str[j]))
					j++;
			}
		
			command += 100;

			if (str[j] == ']')
				j++;
			else
			{
				fprintf (stderr, "Wrong symbol instead of ']': '%c'", str[j]);
				j++;
			}
			
			*str_ptr += j - j_old;
		}
	}
/*
	printf ("str_ptr = %ld\n", *str_ptr);
	printf ("ReadCmd: cmd = %d, arg = %d\n", command, (param_num == 0) ? 666 : *arg);
	printf ("ReadCmd: cur symb is <%c>\n", *(str + (int) *str_ptr));
*/
	return command;
}

int ChooseCmd (char *cmd)
{
	if (!strcmp (cmd, "end"))
		return 0;

	if (!strcmp (cmd, "push"))
		return 1;

	if (!strcmp (cmd, "pop"))
		return 2;

	if (!strcmp (cmd, "add"))
		return 3;

	if (!strcmp (cmd, "sub"))
		return 4;

	if (!strcmp (cmd, "mul"))
		return 5;

	if (!strcmp (cmd, "div"))
		return 6;

	if (!strcmp (cmd, "in"))
		return 7;

	if (!strcmp (cmd, "out"))
		return 8;

	if (!strcmp (cmd, "call"))
		return 9;

	if (!strcmp (cmd, "ret"))
		return 10;

	if (!strcmp (cmd, "jmp"))
		return 30;

	if (!strcmp (cmd, "ja"))
		return 31;
	
	if (!strcmp (cmd, "jb"))
		return 32;
	
	if (!strcmp (cmd, "je"))
		return 33;
	
	if (!strcmp (cmd, "jae"))
		return 34;
	
	if (!strcmp (cmd, "jbe"))
		return 35;
	
	if (!strcmp (cmd, "jne"))
		return 36;
	
	if (!strcmp (cmd, "sqrt"))
		return 41;

	if (!strcmp (cmd, "sqr"))
		return 42;
	
	return -1;
}

void ReverseStr (char *str)
{
	int i = 0, j = 0;
	char c = 0;

	for (i = 0, j = strlen (str) - 1; i < j; i++, j--)
	{
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
}

long int IntToStr (int arg, char *str)
{
	int i = 0, j = 0;
	int negative = 0;
	char *string = (char *) calloc (STR_SIZE, sizeof (*string));

	if (arg < 0)
	{
		negative = 1;
		arg *= -1;
	}

	do
	{
		string[i++] = arg % 10 + '0';
	} while ((arg /= 10) != 0);

	if (negative)
		string[i] = '-';
	
	ReverseStr (string);

	do
	{
		str[j] = string[j];
	} while (string[j++] != '\0');

	free (string);

	return j - 1;
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

int ChooseReg (char *cmd)
{
	if (!strcmp (cmd, "rax"))
		return 1;

	if (!strcmp (cmd, "rbx"))
		return 2;

	if (!strcmp (cmd, "rcx"))
		return 3;

	if (!strcmp (cmd, "rdx"))
		return 4;
}

int ChooseCmdReg (int cmd, int reg)
{
	if (cmd == 1)
	{
		switch (reg)
		{
			case 1:
				return 11;
				break;

			case 2:
				return 12;
				break;

			case 3:
				return 13;
				break;

			case 4:
				return 14;
				break;

			default:
				fprintf (stderr, "Unknown register %d\n", reg);
				return -1;
				break;
		}
	}
	
	if (cmd == 2)
	{
		switch (reg)
		{
			case 1:
				return 21;
				break;

			case 2:
				return 22;
				break;

			case 3:
				return 23;
				break;

			case 4:
				return 24;
				break;

			default:
				fprintf (stderr, "Unknown register %d\n", reg);
				return -1;
				break;
		}

		fprintf (stderr, "No command was found\n");
		return -1;
	}
}

long int ProcessLabel (const char *str, int *arg, int is_label)
{
	if (is_label)
	{
		return AddLabel (str);
	}

	else
	{
		return FindLabel (str, arg);
	}
}

int IsJump (int cmd)
{
	switch (cmd)
	{
		case 9:
			return 1;
			break;
	
		case 30:
			return 1;
			break;
	
		case 31:
			return 1;
			break;
	
		case 32:
			return 1;
			break;
	
		case 33:
			return 1;
			break;
	
		case 34:
			return 1;
			break;
	
		case 35:
			return 1;
			break;
	
		case 36:
			return 1;
			break;
	
		default:
			return 0;
			break;
	}
}

long int FindLabel (const char *str, int *arg)
{
	long int i = 0;
	int label_counter = 0;
	char *cmd = (char *) calloc (LABEL_MAX_LEN + 1, sizeof (*cmd));
//	puts (str);

	while (islower (str[i]) || isdigit (str[i]))
	{
		cmd[i] = str[i];
		i++;
	}
//	puts (cmd);

	for (label_counter = 0; label_counter < LABELS_NUMBER; label_counter++)
	{
		if (!strcmp (labels[label_counter].name, cmd))
		{
			*arg = labels[label_counter].cmdnum;
//			printf ("Returning %ld\n", i);
			return i;
		}
	}

	free (cmd);

	return i;
	
}

long int AddLabel (const char *str)
{
	long int i = 0;
	int label_counter = 0;
	char *cmd = (char *) calloc (LABEL_MAX_LEN + 1, sizeof (*cmd));
	
//	puts (str);

	while (islower (str[i]) || isdigit (str[i]))
	{
		cmd[i] = str[i];
		i++;
	}
//	puts (cmd);

	if (str[i] != ':')
	{
		fprintf (stderr, "Syntax error: no ':' after label\n");
		return i;
	}
	i++;

	for (label_counter = 0; label_counter < LABELS_NUMBER; label_counter++)
	{
		if (!strcmp (labels[label_counter].name, cmd))
			return i;
	}
//	puts ("Here");
	
	for (label_counter = 0; label_counter < LABELS_NUMBER; label_counter++)
	{
		if (labels[label_counter].name[0] == '\0')
		{
			for (i = 0; cmd[i] != '\0'; i++)
			{
				labels[label_counter].name[i] = cmd[i];
			}

			labels[label_counter].cmdnum = op_num;
			
//			printf ("Changed, label_counter = %d, op_num = %d, name = %s\n", label_counter, op_num, labels[label_counter].name);

			return i + 1;
		}
	}

	fprintf (stderr, "Too many labels\n");

	free (cmd);

	return i + 1;
}

#endif	// BYTE

