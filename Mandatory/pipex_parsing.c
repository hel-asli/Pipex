/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:21:08 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/05 00:00:58 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_args(int ac, char *env[], t_pipex *pipex)
{
	int	i;

	if (ac != 5)
		error_handle(ERR_MSG);
	i = find_path(env);
	if (i != -1)
	{
		pipex->env_path = ft_split(&env[i][ft_strlen("PATH=")], ':');
		if (!pipex->env_path)
			error_handle(FAIL_MSG);
		if (!pipex->env_path[0] || empty_string(pipex->env_path[0]))
		{
			ft_free(pipex->env_path);
			error_handle(EMPTY_PATH);
		}
	}
	else
		pipex->env_path = NULL;
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
	return (-1);
}

void	ft_exit(t_pipex *pipex)
{
	if (pipex->env_path)
		ft_free(pipex->env_path);
	ft_free(pipex->cmd);
	error_handle(EMPTY_CMD);
}

int	check_executable(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (!pipex->cmd[0])
		ft_exit(pipex);
	if (access(pipex->cmd[0], F_OK | X_OK) == 0)
	{
		pipex->path = pipex->cmd[0];
		return (1);
	}
	while (pipex->env_path && pipex->env_path[i] != NULL)
	{
		pipex->path = ft_strjoin(pipex->env_path[i], pipex->cmd[0], '/');
		if (!pipex->path)
			return (0);
		if (access(pipex->path, F_OK | X_OK) == 0)
			return (1);
		free(pipex->path);
		pipex->path = NULL;
		i++;
	}
	return (0);
}
