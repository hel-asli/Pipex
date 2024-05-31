#include "pipex.h"

void err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

int main(void)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
		err_exit(NULL);
	if (pid == 0)
	{
		printf("I am the child process.\n");
		sleep(2);
		exit(EXIT_SUCCESS);
	}
	/*
	if (unlink("ooo") == 0)
		printf("File scuccesully deleted");
	else
		printf("Error deleting file");
	*/
	else
	{
		printf("I am the parent process.\n");
		wait(NULL);
		printf("Child process terminate after a 2s delay.\n");
	}

	return (0);
}
