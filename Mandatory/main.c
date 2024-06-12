/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:53:17 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/11 23:13:28 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_lsof(void)
{
	system("leaks pipex");
}

void	first_cmd(t_pipex *pipex, char **env)
{
	if (close(pipex->outile_fd) == -1)
		err_exit("close");
	if (dup2(pipex->infile_fd, 0) == -1)
		err_exit("dup infile to stdin");
	if (close(pipex->infile_fd) == -1 || errno == EBADF)
		err_exit("close infile of pipe");
	if (close(pipex->fds[0]) == -1 || errno == EBADF)
		err_exit("close the read end of the pipe");
	if (dup2(pipex->fds[1], 1) == -1)
		err_exit("dup write-end of the pip to stdout");
	if (close(pipex->fds[1]) == -1 || errno == EBADF)
		err_exit("close write end of pipe");
	if (check_executable(pipex->env_path, &pipex->path_cmd1,
			pipex->first_cmd[0]))
		execve(pipex->path_cmd1, pipex->first_cmd, env);
	ft_free(pipex->env_path);
	ft_free(pipex->first_cmd);
	err_exit("execve");
}

void	second_cmd(t_pipex *pipex, char **env)
{
	if (close(pipex->infile_fd) == -1 || errno == EBADF)
		err_exit("close");
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
	if (check_executable(pipex->env_path, &pipex->path_cmd2,
			pipex->second_cmd[0]))
		execve(pipex->path_cmd2, pipex->second_cmd, env);
	ft_free(pipex->env_path);
	ft_free(pipex->first_cmd);
	err_exit("execve");
}

void	parent(t_pipex *pipex, pid_t pid1, char **env)
{
	pid_t	pid;
	pid_t	wpids[2];
	int		status[2];

	pid = fork();
	if (pid == -1)
		err_exit("fork");
	if (pid == 0)
		second_cmd(pipex, env);
	else
	{
		ft_parent(pipex);
		wpids[0] = waitpid(pid1, &status[0], 0);
		if (wpids[0] == -1)
			err_exit("waitpid");
		if (!WIFEXITED(status[0]))
			err_exit("the child does not complete normaly");
		wpids[1] = waitpid(pid, &status[1], 0);
		if (wpids[1] == -1)
			err_exit("waitpid");
		if (WIFEXITED(status[1]))
			exit(WEXITSTATUS(status[1]));
		else
			err_exit("the child does not complete normaly\n");
	}
}

int	main(int ac, char *av[], char *env[])
{
	t_pipex	pipex;
	pid_t	pid1;

	check_args(ac, av, &pipex);
	check_env(env, &pipex);
	if (pipe(pipex.fds) == -1)
		err_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		err_exit("fork");
	else if (pid1 == 0)
		first_cmd(&pipex, env);
	else
		parent(&pipex, pid1, env);
	exit(EXIT_SUCCESS);
}
