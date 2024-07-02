/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:05:07 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 01:49:40 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void first_cmd(t_pipex *pipex, pid_t **fds, int j)
{
	first_cmd_helper(pipex, fds, j);
	if (dup2(pipex->infile_fd, STDIN_FILENO) == -1)
		err_exit("dup");
	if (close(pipex->infile_fd) == -1)
		err_exit("close");
	if (dup2(fds[j][1], STDOUT_FILENO) == -1)
		err_exit("dup");
	close_pipes(fds, pipex->ac - 4);
	if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
	free_res(pipex);
	free(pipex->ids);
	err_exit("execve first_cmd");
}

void last_cmd(t_pipex *pipex, pid_t **fds, int j)
{
	last_cmd_helper(pipex, fds, j);
	if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
		err_exit("dup");
	if (close(pipex->outfile_fd) == -1)
		err_exit("close");
	if (dup2(fds[j - 1][0], STDIN_FILENO) == -1)
		err_exit("dup2");
	close_pipes(fds, pipex->ac - 4);
	if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
	free_res(pipex);
	free(pipex->ids);
	err_exit("execve last_cmd");
}

void other_cmd(t_pipex *pipex, pid_t **fds, int j)
{
	pipex->cmd = ft_split(pipex->av[j + 2], ' ');
	if (!pipex->cmd)
	{
		free_res(pipex);
		free(pipex->ids);
		close_pipes(fds, pipex->ac - 4);
		err_handler("split\n");
	}
	dup2(fds[j - 1][0], STDIN_FILENO);
	dup2(fds[j][1], STDOUT_FILENO);
	close_pipes(fds, pipex->ac - 4);
	if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
	free_res(pipex);
	free(pipex->ids);
	err_exit("execve other_cmd");
}