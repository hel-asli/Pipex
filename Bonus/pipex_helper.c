/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 02:47:16 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 05:22:17 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_res(t_pipex *pipex)
{
	if (!pipex)
		return ;
	if (pipex->cmd_path)
	{
		free(pipex->cmd_path);
		pipex->cmd_path = NULL;
	}
	if (pipex->cmd)
	{
		ft_free(pipex->cmd);
		pipex->cmd_path = NULL;
	}
	if (pipex->env_path)
	{
		ft_free(pipex->env_path);
		pipex->cmd_path = NULL;
	}
	if (pipex->here_doc)
	{
		free(pipex->here_doc);
		pipex->here_doc = NULL;
	}
	if (pipex->fds)
		free_fds(pipex->fds, pipex->ac - 4);
}

void	err_handler(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	first_cmd_helper(t_pipex *pipex, int **fds, int j)
{
	pipex->infile_fd = open(pipex->av[1], O_RDONLY);
	if (pipex->infile_fd < 0)
	{
		close_pipes(fds, j);
		free_res(pipex);
		free(pipex->ids);
		err_exit("open infile");
	}
	pipex->cmd = ft_split(pipex->av[2], ' ');
	if (!pipex->cmd)
	{
		close_pipes(fds, pipex->ac - 4);
		close(pipex->infile_fd);
		free_res(pipex);
		free(pipex->ids);
		err_handler(FAIL_MSG);
	}
}

void	last_cmd_helper(t_pipex *pipex, int **fds, int j)
{
	pipex->outfile_fd = open(pipex->av[pipex->ac - 1],
			O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile_fd == -1)
	{
		close_pipes(fds, pipex->ac - 4);
		free_res(pipex);
		free(pipex->ids);
		err_exit("open outfile");
	}
	pipex->cmd = ft_split(pipex->av[j + 2], ' ');
	if (!pipex->cmd)
	{
		close_pipes(fds, pipex->ac - 4);
		close(pipex->outfile_fd);
		free_res(pipex);
		free(pipex->ids);
		err_handler(FAIL_MSG);
	}
}
