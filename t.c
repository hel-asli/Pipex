#include "pipex.h"
#include <stdio.h>

void err_exit(char *str)
{
	perror(str);
	exit(exit_failure);
}

int main(void)
{
	//int fds[2];
	pid_t pid;
	//char buffer[6];

	/*
	if (pipe(fds) == -1)
		err_exit("pipe");
		*/
	int fd = open("ooo", O_RDWR);
	if (fd == -1)
		err_exit("open");
	pid = fork();
	if (pid == -1)
		err_exit("fork");

	if (pid == 0)
	{
		/*
		close(fds[0]); // close the read end of the pipe
		write(fds[1], "Hello parent!", 13);
		close(fds[1]); // close the write end of the pipe
					   // */
		printf("form child\n");	
		sleep(10);
	//	exit(EXIT_SUCCESS);
	}
	else
	{
		pid_t i = fork();

		if (i == -1)
			err_exit("fork");
		if (i == 0)
			printf("from child number 2\n");
		else
		{
			while (wait(NULL) != -1 || errno != ECHILD);
			printf("from parent\n");
		}
		sleep(10);
}
	return (0);
}
