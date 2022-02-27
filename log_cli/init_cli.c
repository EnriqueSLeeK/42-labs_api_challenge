#include "cli.h"

int	set_format(char *format, int flag, int fmt_code)
{
	char	*token;
	int			tmp;
	static	int fmt;

	if (flag == SET)
	{

		fmt = 0;
		fmt |= H_END;
		tmp = fmt;
		token = strtok(format, " ");
		while (token != NULL)
		{
			if (strcmp(token, "%m") == 0)
				fmt |= H_METHOD;
			else if (strcmp(token, "%e") == 0)
				fmt |= H_ENDPOINT;
			else if (strcmp(token, "%a") == 0)
				fmt |= H_AGENT;
			else if (strcmp(token, "%k") == 0)
				fmt |= H_STATUS;
			else if (strcmp(token, "%t") == 0)
				fmt |= H_DATE;
			else
				return (-1);
			if (tmp != fmt)
				token = strtok(NULL, " ");
			else
				return (-1);
			tmp = fmt;
		}
	}
	else if (flag == SET_DIRECT)
		fmt = fmt_code;
	else if (flag == GET)
		return (fmt);
	return (1);
}

int	init_flags(int argc, char *argv[], t_flag *args)
{
	int flag;

	flag = 0;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp("-d", argv[i]) == 0)
		{
			if (i + 1 < argc)
			{
				if (flag & 1 || readable(argv[i + 1]) == 0)
					return (-1);
				if (i == 1 || i == 3)
				{
					args -> dir = argv[i + 1];
					flag |= 1;
				}
				else
					return (-1);
			}
			else
				return (-1);
		}
		else if(strcmp("-f", argv[i]) == 0)
		{
			if (i + 1 < argc)
			{
				if (flag & 2 || set_format(argv[i + 1], SET, 0) == -1)
					return (-1);
				if (i == 1 || i == 3)
				{
					args -> header_format = argv[i + 1];
					flag |= 2;
				}
				else
					return (-1);
			}
			else
				return (-1);
		}
	}
	return (flag);
}

void	init(int argc, char *argv[], int *header_size, FILE **fd)
{
	int		flag;
	t_flag	args;

	flag = 0;
	if (argc > 1)
	{
		flag = init_flags(argc, argv, &args);
		if (flag == -1)
			usage();
		if (flag & 1)
		{
			*fd = fopen(args.dir, "r");
			if (*fd == NULL)
				error_exit("Failed opening the file");
		}
		else
		{
			*fd = fopen("./Logs/log", "r");
			if (*fd == NULL)
				error_exit("Failed opening the file");
		}
		if (flag & 2)
			*header_size = custom_header();
		else
			*header_size = standart_header();
	}
	else
	{
		if (readable("./Logs/log"))
		{
			*header_size = standart_header();
			*fd = fopen("./Logs/log", "r");
			if (*fd == NULL)
				error_exit("Failed to open!\n");
		}
		else
			error_exit("There is no log file\n");
	}
}
