#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

# define	SET_DIRECT	2
# define	SET			1
# define	GET			0

# define	H_METHOD	1
# define	H_ENDPOINT	2
# define	H_AGENT		4
# define	H_STATUS	8
# define	H_DATE		16
# define	H_COUNT		32
# define	H_END		64

# define	LINE_SIZE	1024

typedef struct	log_line
{
	char *method;
	char *endpoint;
	char *user_agent;
	char *status;
	char *timestamp;
}	t_log;

typedef struct	flag
{
	char	*header_format;
	char	*dir;
}	t_flag;

// Init functions
int		init_flag(int argc, char *argv[], t_flag *args);
void	init(int argc, char *argv[], int *header_size, FILE **fd);

// Utils
void	content(char **sections);
void	parse_line(char *line, char **args);
int		set_format(char *format, int flag, int fmt_code);
int		custom_header(void);
int		readable(char *path);
void	error_exit(char *msg);
int		standart_header(void);
void	draw_line(int quant);
void	padding(int quant);
int		headers(int flag);
void	usage(void);
