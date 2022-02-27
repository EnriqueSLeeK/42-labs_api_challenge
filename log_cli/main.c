#include <stdio.h>
#include <errno.h>
#include <unistd.h>

void	draw_line(int quant)
{
	for (int i = 0; i < quant; i++)
		printf("-");
	printf("\n");
}

int main(int argc, int *argv[])
{
	if (access("./Logs/log", R_OK) == 0 || access("../Logs/log", R_OK) == 0)
	{
		printf("There is logs to read!\n");
	}
	else
		printf("There is no log file\n");
}
