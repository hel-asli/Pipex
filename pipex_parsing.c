/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:21:08 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/01 17:39:30 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void check_args(int ac, char *av[], t_pipex *pipex)
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
	pipex->second_cmd = ft_split(av[3], ' ');
}

int stats_with(char *str)
{
	size_t i = 0;
	char *path = "PATH=";
	while (str[i] && path[i] && str[i] == path[i])
		i++;
	if (i == ft_strlen(path))
		return (1);
	return (0);
}

int find_path(char **env)
{
	int i = 0;
	while (env[i])
	{
		if (stats_with(env[i]))
			return (i);
		i++;
	}

	return (0);
}

int check_executable(char **env_path, char **path, char *cmd_name)
{
	int i;

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
			if (access(*path, F_OK | X_OK) == 0)
				return (1);
		}
		i++;
	}
	return (0);
	
}

void check_env(char **env, t_pipex *pipex)
{
	int i = find_path(env);
	if (!i)
		error_handle("env not exits");
	else
	{
		pipex->env_path = ft_split(&env[i][ft_strlen("PATH=")], ':');
	}
}