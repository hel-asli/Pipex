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


void ft_lsof(void)
{
	system("lsof -c pipex");
}



void first_cmd(t_pipex *pipex , char **env)
{
	if (dup2(pipex->infile_fd, 0) == -1)
		err_exit("dup infile to stdin");
	if (close(pipex->infile_fd) == -1 || errno == EBADF)
	if (close(pipex->fds[0]) == -1 || errno == EBADF)
		err_exit("close the read end of the pipe");
	if (dup2(pipex->fds[1], 1) == -1)
		err_exit("dup write-end of the pip to stdout");
	if (close(pipex->fds[1]) == -1 || errno == EBADF)
		close(pipex->fds[1]);
	if (check_executable(pipex->env_path, &pipex->path_cmd1, pipex->first_cmd[0]))
		execve(pipex->path_cmd1, pipex->first_cmd, env);
	err_exit("execve");
}

void second_cmd(t_pipex *pipex, char **env)
{
	if (close(pipex->fds[1]) == -1 || errno == EBADF)
		err_exit("close the write end of the pipe");
	if (dup2(pipex->fds[0], 0) == -1)
		err_exit("dup the read end of the pipe to stdin");
	if (close(pipex->fds[0]) == -1 || errno == EBADF)
		err_exit("close the read end of the pipe");
	if (dup2(pipex->outile_fd, 1) == -1)
		err_exit("dup the outfile to the stdout");
	if (close(pipex->outile_fd) == -1 || errno == EBADF)
		err_exit("close the outfile");
	if (check_executable(pipex->env_path, &pipex->path_cmd2, pipex->second_cmd[0]))
		execve(pipex->path_cmd2, pipex->second_cmd, env);
	err_exit("execve"); // this line is extecuted only in the case when execve Fail :)
}

void parent(t_pipex *pipex, char **env)
{
	pid_t pid = fork();
	if (pid == -1)
		err_exit("fork");
	if (pid == 0)
		second_cmd(pipex, env);
	else
	{
		if (close(pipex->fds[1]) == -1 || errno == EBADF)
			err_exit("close the write end of the pipe");
		if (close(pipex->fds[0]) == -1 || errno == EBADF)
			err_exit("close the read end of the pipe");
		while (wait(NULL) != -1 || errno != ECHILD);
	}
}



int	main(int ac, char *av[], char *env[])
{
	t_pipex pipex;

	check_args(ac, av, &pipex);
	check_env(env, &pipex);
	if (pipe(pipex.fds) == -1)
		err_exit("pipe");
	pid_t pid1 = fork();

	if (pid1 == -1)
		err_exit("fork");
	else if (pid1 == 0)
	{
		first_cmd(&pipex, env);
	
	}
	else
	{
		parent(&pipex, env);
	}
	return (0);
}

