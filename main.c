/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:53:17 by hel-asli          #+#    #+#             */
/*   Updated: 2024/05/31 20:59:05hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE); // the exit make all the available resource available to the kernel for reallocation
	// the parent ps can recive the EXIT_STATUS using the wait fun
}

void ft_lsof(void)
{
	system("lsof -c pipex");
}

int	main(int ac, char *av[], char *env[])
{
	(void)ac;
	(void)av;
	(void)env;

	atexit(ft_lsof);
	if (ac != 5)	
	{
		fprintf(stderr, "Bad Args: < infile cmd1 cmd1 > outfile ");
		exit(EXIT_FAILURE);
	}

	int fds[2];
	if (pipe(fds) == -1)
		err_exit("pipe");

	pid_t pid1 = fork();

	if (pid1 == -1)
		err_exit("fork");
	else if (pid1 == 0)
	{
		int infile_fd = open(av[1], O_RDONLY);
		if (infile_fd == -1)
			err_exit("open");
		close(fds[0]);
		if (dup2(infile_fd, 0) == -1)
			err_exit("dup2");
		close(infile_fd);
		dup2(fds[1], 1);
		close(fds[1]);

		execve("/bin/cat", ft_split(av[2], ' '), env);
		err_exit("execve");
	}
	else
	{
		pid_t pid2 = fork();
		if (pid2 == -1)
			err_exit("fork");
		else if (pid2 == 0)
		{
			close(fds[1]);
			if (dup2(fds[0], 0) == -1)
				err_exit("dup2");
			close(fds[0]);

			int fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd_out == -1)
				err_exit("open");
			if (dup2(fd_out, 1) == -1)
				err_exit("dup2");
			close(fd_out);
			execve("/bin/ls", ft_split(av[3], ' '), env);
			err_exit("execve"); // this line is extecuted only in the case when execve Fail :)
		}
		else
		{
			close(fds[0]);	
			close(fds[1]);
			while (wait(NULL) != -1 || errno != ECHILD);
		}
	}
	return (1);
}
