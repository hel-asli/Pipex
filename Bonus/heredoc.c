/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 20:10:52 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 02:11:38 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_cmd1(t_pipex *pipex, int fds[2])
{
	cmd1_helper(pipex, fds);
	dup2(pipex->infile_fd, STDIN_FILENO);
	close(pipex->infile_fd);
	dup2(fds[1], 1);
	close(fds[1]);
	close(fds[0]);
	if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
	free_res(pipex);
	err_exit("execve");
}

void	execute_cmd2(t_pipex *pipex, int fds[2])
{
	cmd2_helper(pipex, fds);
	dup2(pipex->outfile_fd, STDOUT_FILENO);
	close(pipex->outfile_fd);
	dup2(fds[0], 0);
	close(fds[1]);
	close(fds[0]);
	if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
	free_res(pipex);
	err_exit("execve");
}

void	parent(t_pipex *pipex, pid_t id, int fds[2])
{
	pid_t	id1;
	int		status[2];

	id1 = fork();
	if (id1 < 0)
		err_exit("fork");
	if (id1 == 0)
		execute_cmd2(pipex, fds);
	else
	{
		close(fds[0]);
		close(fds[1]);
		status[0] = 0;
		if (waitpid(id, &status[0], 0) < 0)
			err_exit("waitpid");
		status[1] = 0;
		if (waitpid(id1, &status[1], 0) < 0)
			err_exit("waitpid");
	}
	unlink(pipex->here_doc);
	free_res(pipex);
	exit(WEXITSTATUS(status[1]));
}

void	heredoc_implement(t_pipex *pipex)
{
	pid_t	n;
	pid_t	id;
	int		fds[2];

	pipex->here_doc = get_file_name();
	if (!pipex->here_doc)
		return ;
	heredoc_file(pipex);
	n = pipe(fds);
	if (n == -1)
		err_exit("pipe");
	id = fork();
	if (id == -1)
		err_exit("fork");
	if (id == 0)
		execute_cmd1(pipex, fds);
	else
		parent(pipex, id, fds);
}
