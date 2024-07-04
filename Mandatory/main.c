/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:53:17 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/04 14:16:03 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(t_pipex *pipex, char *av[], char **env)
{
	first_child_helper(pipex, av);
	if (dup2(pipex->infile_fd, STDIN_FILENO) == -1)
		err_exit("dup infile to stdin");
	if (close(pipex->infile_fd) == -1)
		err_exit("close infile of pipe");
	if (dup2(pipex->fds[1], STDOUT_FILENO) == -1)
		err_exit("dup write-end of the pipe to stdout");
	if (close(pipex->fds[0]) == -1)
		err_exit("close the read end of the pipe");
	if (close(pipex->fds[1]) == -1)
		err_exit("close write end of pipe");
	if (check_executable(pipex))
	{
		execve(pipex->path, pipex->cmd, env);
		if (pipex->env_path)
			ft_free(pipex->env_path);
		ft_free(pipex->cmd);
		err_exit("execve");
	}
	if (pipex->env_path)
		ft_free(pipex->env_path);
	ft_free(pipex->cmd);
	err_exit("access");
}

void	second_child(t_pipex *pipex, char **av, char **env)
{
	second_child_helper(pipex, av);
	if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
		err_exit("dup the outfile to the stdout");
	if (close(pipex->outfile_fd) == -1)
		err_exit("close the outfile");
	if (dup2(pipex->fds[0], STDIN_FILENO) == -1)
		err_exit("dup the read end of the pipe to stdin");
	if (close(pipex->fds[1]) == -1)
		err_exit("close the write end of the pipe");
	if (close(pipex->fds[0]) == -1)
		err_exit("close the read end of the pipe");
	if (check_executable(pipex))
	{
		execve(pipex->path, pipex->cmd, env);
		if (pipex->env_path)
			ft_free(pipex->env_path);
		ft_free(pipex->cmd);
		err_exit("execve");
	}
	if (pipex->env_path)
		ft_free(pipex->env_path);
	ft_free(pipex->cmd);
	err_exit("access");
}

void	parent(t_pipex *pipex, pid_t pid1, char **av, char **env)
{
	pid_t	pid;
	int		status[2];

	status[0] = 0;
	status[1] = 0;
	pid = fork();
	if (pid == -1)
		err_exit("fork");
	if (pid == 0)
		second_child(pipex, av, env);
	else
	{
		if (close(pipex->fds[0]) < 0 || close(pipex->fds[1]) < 0)
			err_exit("close");
		if (pipex->env_path)
			ft_free(pipex->env_path);
		if (waitpid(pid1, &status[0], 0) == -1)
			err_exit("waitpid");
		if (waitpid(pid, &status[1], 0) == -1)
			err_exit("waitpid");
		exit(WEXITSTATUS(status[1]));
	}
}

int	main(int ac, char *av[], char *env[])
{
	t_pipex	pipex;
	pid_t	pid1;

	check_args(ac, env, &pipex);
	if (pipe(pipex.fds) == -1)
	{
		if (pipex.env_path)
			ft_free(pipex.env_path);
		err_exit("pipe");
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		if (close(pipex.fds[0]) < 0 || close(pipex.fds[1]) < 0)
			err_exit("close");
		if (pipex.env_path)
			ft_free(pipex.env_path);
		err_exit("fork");
	}
	else if (pid1 == 0)
		first_child(&pipex, av, env);
	else
		parent(&pipex, pid1, av, env);
	exit(EXIT_SUCCESS);
}
