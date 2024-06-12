/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:21:08 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/11 23:28:13 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(int ac, char *av[], t_pipex *pipex)
{
	(void)av;
	if (ac != 5)
	{
		ft_putstr_fd(ERR_MSG, 2);
		exit(EXIT_FAILURE);
	}
	pipex->infile_fd = open(av[1], O_RDONLY);
	if (pipex->infile_fd == -1)
		err_exit("open infile");
	pipex->outile_fd = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (pipex->outile_fd == -1)
		err_exit("open outfile");
	pipex->first_cmd = ft_split(av[2], ' ');
	if (!pipex->first_cmd)
		error_handle("malloc :)\n");
	pipex->second_cmd = ft_split(av[3], ' ');
	if (!pipex->second_cmd)
		error_handle("malloc :)\n");
}

int	stats_with(char *str)
{
	size_t	i;
	char	*path;

	i = 0;
	path = "PATH=";
	while (str[i] && path[i] && str[i] == path[i])
		i++;
	if (i == ft_strlen(path))
		return (1);
	return (0);
}

int	find_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (stats_with(env[i]))
			return (i);
		i++;
	}
	return (0);
}

int	check_path(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/' && str[i + 1] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	check_executable(char **env_path, char **path, char *cmd_name)
{
	int	i;

	i = 0;
	while (env_path[i] != NULL)
	{
		if (access(cmd_name, F_OK | X_OK) == 0)
		{
			*path = cmd_name;
			return (1);
		}
		else
		{
			*path = ft_strjoin(env_path[i], cmd_name, '/');
			if (!*path)
				error_handle("malloc :)\n");
			if (check_path(*path))
				return (0);
			if (access(*path, F_OK | X_OK) == 0)
				return (1);
			free(*path);
		}
		i++;
	}
	return (0);
}
