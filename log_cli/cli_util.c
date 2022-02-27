#include "cli.h"

void	parse_line(char *line, char **args)
{
	char	*token;

	token = strtok(line, ",");
	for (int i = 0; i < 5; i++)
	{
		args[i] = token;
		token = strtok(NULL, ",");
	}
}

void	padding(int quant)
{
	for (int i = 0; i <= quant + 1; i++)
		printf(" ");
}

void	draw_line(int quant)
{
	for (int i = 0; i <= quant; i++)
		printf("-");
	printf("\n");
}

int	headers(int flag)
{
	int	printed;

	printed = 0;
	if (flag & H_METHOD)
		printed += printf("|  METHOD ");
	if (flag & H_ENDPOINT)
		printed += printf("|            ENDPOINT            ");
	if (flag & H_AGENT)
		printed += printf("| USER AGENT ");
	if (flag & H_STATUS)
		printed += printf("| STATUS ");
	if (flag & H_DATE)
		printed += printf("|         TIMESTAMP        ");
	if (flag & H_END)
		printf("|\n");
	return (printed);
}

static void	print_content(int decide, char *section, int counter)
{
	if (decide)
	{
		counter -= printf("| %.*s", counter - 1, section);
		padding(counter);
	}
}

void	content(char **sections)
{
	int	flag;

	flag = set_format(NULL, GET, 0);
	print_content(flag & H_METHOD, sections[0], 8);
	print_content(flag & H_ENDPOINT, sections[1], 31);
	print_content(flag & H_AGENT, sections[2], 11);
	print_content(flag & H_STATUS, sections[3], 7);
	print_content(flag & H_DATE, sections[4], 25);
	if (flag & H_END)
		printf("|\n");
}

int	standart_header(void)
{
	int	quant;

	set_format(NULL, SET_DIRECT, H_METHOD | H_ENDPOINT | H_AGENT | H_STATUS | H_DATE | H_END);
	quant = headers(set_format(NULL, GET, 0));
	draw_line(quant);
	return (quant);
}

int	custom_header(void)
{
	int quant;
	int	check;

	check = set_format(NULL, GET, 0);
	if (check == 0)
		return (0);
	quant = headers(check | H_END);
	draw_line(quant);
	return (quant);
}

void	usage(void)
{
	printf("./log_cli -d <log_file_path> -f <format>\n");
	printf("Flags are optional\n");
	exit(0);
}

int	readable(char *path)
{
	if (access(path, R_OK) == 0)
		return (1);
	return (0);
}

void	error_exit(char *msg)
{
	fprintf(stderr, msg);
	exit(2);
}
