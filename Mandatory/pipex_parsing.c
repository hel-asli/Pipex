/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:21:08 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 01:55:07 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(int ac, char *env[], t_pipex *pipex)
{
	int	i;

	if (ac != 5)
	{
		ft_putstr_fd(ERR_MSG, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	i = find_path(env);
	if (!i)
	{
		ft_putstr_fd("env PATH not found\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	pipex->env_path = ft_split(&env[i][ft_strlen("PATH=")], ':');
	if (!pipex->env_path)
	{
		ft_putstr_fd("Failed to parse PATH\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
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
		if (stats_with(env[i]))
		{
			return (i);
		}
		i++;
	}
	return (0);
}

// int	check_path(char *str)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '/' && str[i + 1] == '/')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

int	check_executable(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (!pipex->cmd[0])
	{
		ft_free(pipex->env_path);
		ft_free(pipex->cmd);
		error_handle("empty command\n");
	}
	while (pipex->env_path[i] != NULL)
	{
		if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		{
			pipex->path = pipex->cmd[0];
			return (1);
		}
		else
		{
			pipex->path = ft_strjoin(pipex->env_path[i], pipex->cmd[0], '/');
			if (!pipex->path)
				return (0);
			if (access(pipex->path, F_OK | X_OK) == 0)
				return (1);
			free(pipex->path);
		}
		i++;
	}
	return (0);
}
