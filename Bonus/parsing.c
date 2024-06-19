/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:05:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/19 11:39:59 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void check_args(char *env[], t_pipex *pipex)
{
    int i;

    i = find_path(env);
    if (!i)
    {
        ft_putstr_fd("env PATH not found\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
    pipex->env_path = ft_split(&env[i][ft_strlen("PATH=")], ':');
    if (!pipex->env_path || pipex->env_path[0] == 0)
    {
        ft_free(pipex->env_path);
        ft_putstr_fd("Failed to parse PATH\n", STDERR_FILENO);
        exit(EXIT_FAILURE);
    }
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
		// puts(env[i]);
		if (stats_with(env[i]))
		{
			return (i);
		}
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
				exit(EXIT_FAILURE);
				/*
			if (check_path(*path))
			{
				return (0);
			}
				*/
			if (access(*path, F_OK | X_OK) == 0)
				return (1);
			free(*path);
		}
		i++;
	}

	return (0);
}
