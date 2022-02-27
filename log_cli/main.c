#include <stdio.h>
#include <errno.h>
#include <unistd.h>

# define	H_METHOD	1
# define	H_ENDPOINT	2
# define	H_AGENT		4
# define	H_STATUS	8
# define	H_DATE		16
# define	H_COUNT		32
# define	H_END		64

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
		printed += printf("| METHOD ");
	if (flag & H_ENDPOINT)
		printed += printf("|            ENDPOINT            ");
	if (flag & H_AGENT)
		printed += printf("| USER AGENT ");
	if (flag & H_STATUS)
		printed += printf("| STATUS ");
	if (flag & H_DATE)
		printed += printf("|        TIMESTAMP        ");
	if (flag & H_END)
		printf("|\n");
	return (printed);
}

int	standart_header(void)
{
	int	quant;

	quant = headers(H_METHOD | H_ENDPOINT | H_AGENT | H_STATUS | H_DATE | H_END);
	draw_line(quant);
	return (quant);
}

int	parse_flags(int argc, char *argv[])
{
}

int main(int argc, char *argv[])
{
	int			header_size;
	struct arg	aux;
	FILE		*fd;
	
	if (argc > 1)
	{
		if (parse_flags(argc, argv) == -1)
			return (-1);
	}
	else if (access("./Logs/log", R_OK) == 0 || access("../Logs/log", R_OK) == 0)
	{
		header_size = standart_header();
		fd = fopen("./Logs/log", "r");
		if (fd == NULL)
		{
			fprintf(stderr, "Failed to open!\n");
		}
	}
	else
		printf("There is no log file\n");
}
