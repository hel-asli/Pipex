/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 03:28:13 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/04 23:59:47 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	error_handle(char *str)
{
	ft_putstr_fd(str, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	first_child_helper(t_pipex *pipex, char **av)
{
	pipex->infile_fd = open(av[1], O_RDONLY);
	if (pipex->infile_fd == -1)
	{
		if (pipex->env_path)
			ft_free(pipex->env_path);
		close(pipex->fds[0]);
		close(pipex->fds[1]);
		err_exit("open infile");
	}
	pipex->cmd = ft_split(av[2], ' ');
	if (!pipex->cmd)
	{
		if (pipex->env_path)
			ft_free(pipex->env_path);
		close(pipex->fds[0]);
		close(pipex->fds[1]);
		if (close(pipex->infile_fd) == -1)
			err_exit("close infile");
		ft_putstr_fd(FAIL_MSG, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void	second_child_helper(t_pipex *pipex, char **av)
{
	pipex->outfile_fd = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipex->outfile_fd == -1)
	{
		if (pipex->env_path)
			ft_free(pipex->env_path);
		close(pipex->fds[0]);
		close(pipex->fds[1]);
		err_exit("open outfile");
	}
	pipex->cmd = ft_split(av[3], ' ');
	if (!pipex->cmd)
	{
		if (pipex->env_path)
			ft_free(pipex->env_path);
		close(pipex->fds[0]);
		close(pipex->fds[1]);
		if (close(pipex->outfile_fd) == -1)
			err_exit("close outfile");
		ft_putstr_fd(FAIL_MSG, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
