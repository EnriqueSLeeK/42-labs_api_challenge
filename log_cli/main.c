#include "cli.h"

int main(int argc, char *argv[])
{
	int			header_size;
	FILE		*fd;
	char		buff[1024];
	char		*sections[5];

	init(argc, argv, &header_size, &fd);
	while (fgets(buff, 1024, fd) != NULL)
	{
		parse_line(buff, sections);
		content(sections);
	}
	fclose(fd);
	draw_line(header_size);
	return (0);
}
