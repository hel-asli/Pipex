/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 19:59:30 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/05 19:59:43 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	execute_cmd1(t_pipex *pipex, int fds[2])
{
	cmd1_helper(pipex, fds);
	if (dup2(pipex->infile_fd, STDIN_FILENO) < 0)
		err_exit("dup2");
	if (close(pipex->infile_fd) < 0)
		err_exit("close");
	if (dup2(fds[1], 1) < 0)
		err_exit("dup2");
	if (close(fds[1]) < 0 || close(fds[0]) < 0)
		err_exit("close");
	if (check_executable(pipex))
	{
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
		free_res(pipex);
		err_exit("execve");
	}
	free_res(pipex);
	err_exit("access");
}

void	execute_cmd2(t_pipex *pipex, int fds[2])
{
	cmd2_helper(pipex, fds);
	if (dup2(pipex->outfile_fd, STDOUT_FILENO) < 0)
		err_exit("dup2");
	if (close(pipex->outfile_fd) < 0)
		err_exit("close2");
	if (dup2(fds[0], 0) < 0)
		err_exit("dup2");
	if (close(fds[1]) < 0 || close(fds[0]) < 0)
		err_exit("close");
	if (check_executable(pipex))
	{
		execve(pipex->cmd_path, pipex->cmd, pipex->env);
		free_res(pipex);
		err_exit("execve");
	}
	free_res(pipex);
	err_exit("access");
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
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		status[0] = 0;
		if (waitpid(id, &status[0], 0) < 0)
			err_exit("waitpid");
		status[1] = 0;
		if (waitpid(id1, &status[1], 0) < 0)
			err_exit("waitpid");
	}
	if (unlink(pipex->here_doc) < 0)
		err_exit("unlink");
	free_res(pipex);
	exit(WEXITSTATUS(status[1]));
}

char	*get_file_name(t_pipex *pipex)
{
	char	*ptr;
	char	*str;

	pipex->nb = get_pid(pipex);
	ptr = ft_itoa(pipex->nb);
	if (!ptr)
		return (NULL);
	str = ft_strjoin(ft_strdup("/tmp/.here_doc-XXX"), ptr);
	if (!str)
		return (free(ptr), NULL);
	free(ptr);
	return (str);
}

void	heredoc_implement(t_pipex *pipex)
{
	pid_t	n;
	pid_t	id;
	int		fds[2];

	pipex->here_doc = get_file_name(pipex);
	if (!pipex->here_doc)
	{
		free_res(pipex);
		err_handler(FAIL_MSG);
	}
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
