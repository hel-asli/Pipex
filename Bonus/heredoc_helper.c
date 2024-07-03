/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 18:39:49 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 04:27:39 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	cmd1_helper(t_pipex *pipex, int fds[2])
{
	pipex->infile_fd = open(pipex->here_doc, O_RDONLY);
	if (pipex->infile_fd < 0)
	{
		free_res(pipex);
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		err_exit("open");
	}
	pipex->cmd = ft_split(pipex->av[3], ' ');
	if (!pipex->cmd)
	{
		free_res(pipex);
		if (close(fds[0]) < 0 || close(fds[1]) < 0)
			err_exit("close");
		err_handler(FAIL_MSG);
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
		err_handler(FAIL_MSG);
	}
}

int	heredoc_file_opener(t_pipex *pipex)
{
	int	fd;

	fd = open(pipex->here_doc, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		free_res(pipex);
		err_exit("open heredoc");
	}
	return (fd);
}

char	*heredoc_prompt(char *line)
{
	char	*str;

	write(1, "> ", 2);
	str = get_next_line(STDIN_FILENO, line);
	return (str);
}

void	heredoc_file(t_pipex *pipex)
{
	int		fd;
	char	*str;
	char	*line;

	fd = heredoc_file_opener(pipex);
	str = ft_strdup("");
	if (!str)
		return ;
	line = get_line(pipex->av[2], "\n");
	if (!line)
		return ;
	while (ft_strcmp(str, line))
	{
		free(str);
		str = heredoc_prompt(line);
		if (!str)
			break ;
		if (!ft_strcmp(str, line))
			break ;
		write(fd, str, ft_strlen(str));
	}
	if (str)
		free(str);
	free(line);
	close(fd);
}
