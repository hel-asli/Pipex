/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:39:49 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 06:50:40 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	cmd1_helper(t_pipex *pipex, int fds[2])
{
	pipex->infile_fd = open(pipex->here_doc, O_RDONLY);
	if (pipex->infile_fd < 0)
	{
		free_res(pipex);
		if (close(fds[0]) < 0  || close(fds[1]) < 0)
			err_exit("close");
		err_exit("open");
	}
	pipex->cmd = ft_split(pipex->av[3], ' ');

	if (!pipex->cmd)
	{
		free_res(pipex);
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		err_handler("split");
	}
	if (!pipex->cmd[0])
	{
		free_res(pipex);
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		err_handler("split");
	}
}

void	cmd2_helper(t_pipex *pipex, int fds[2])
{
	pipex->outfile_fd = open(pipex->av[5], O_RDWR | O_CREAT | O_APPEND, 0777);
	if (pipex->outfile_fd < 0)
	{
		free_res(pipex);
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		err_exit("outfile");
	}
	pipex->cmd = ft_split(pipex->av[4], ' ');
	if (!pipex->cmd)
	{
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		free_res(pipex);
		err_handler("split");
	}
	if (!pipex->cmd[0])
	{
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		free_res(pipex);
		err_handler("split");
	}
}

void	heredoc_file(t_pipex *pipex)
{
	int		fd;
	char	*str;
	char	*line;

	fd = open(pipex->here_doc, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
		err_exit("open heredoc");
	str = ft_strdup("");
	if (!str)
		return ;
	line = get_line(pipex->av[2], "\n");
	if (!line)
		return ;
	while (ft_strcmp(str, line))
	{
		free(str);
		write(1, "> ", 2);
		str = get_next_line(STDIN_FILENO, line);
		if (!str)
			break ;
		if (!ft_strcmp(str, line))
		{
			free(str);
			break ;
		}
		write(fd, str, ft_strlen(str));
	}
	free(line);
	if (close(fd) < 0)
		err_exit("close");
}

char	*get_file_name(void)
{
	char	*ptr;
	char	*str;

	ptr = ft_itoa(getpid());
	if (!ptr)
		return (NULL);
	str = ft_strjoin(ft_strdup("/tmp/.here_doc-XXX"), ptr);
	if (!str)
		return (free(ptr), NULL);
	free(ptr);
	return (str);
}
