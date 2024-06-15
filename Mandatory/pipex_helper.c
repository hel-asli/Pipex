/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 03:28:13 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/11 23:27:35 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

// void	check_env(char	**env, t_pipex	*pipex)
// {
// }

void	ft_parent(t_pipex *pipex)
{
	ft_free(pipex->first_cmd);
	ft_free(pipex->second_cmd);
	ft_free(pipex->env_path);
	if (close(pipex->infile_fd) == -1 || errno == EBADF)
		err_exit("close");
	if (close(pipex->outfile_fd) == -1 || errno == EBADF)
		err_exit("close");
	if (close(pipex->fds[1]) == -1 || errno == EBADF)
		err_exit("close the write end of the pipe");
	if (close(pipex->fds[0]) == -1 || errno == EBADF)
		err_exit("close the read end of the pipe");
}

void	error_handle(char *str)
{
	ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}
