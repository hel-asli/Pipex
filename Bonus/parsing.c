/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 17:05:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 02:23:23 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	check_args(char *env[], t_pipex *pipex)
{
	int	i;

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
	if (!pipex->env_path[0])
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
			return (i);
		i++;
	}
	return (0);
}

void	ft_exit(t_pipex *pipex)
{
	free_res(pipex);
	free(pipex->ids);
	err_handler("empty command");
}

int	check_executable(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (!pipex->cmd[0])
		ft_exit(pipex);
	while (pipex->env_path[i] != NULL)
	{
		if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		{
			pipex->cmd_path = pipex->cmd[0];
			return (1);
		}
		else
		{
			pipex->cmd_path = ft_strjoin_del(pipex->env_path[i],
					pipex->cmd[0], '/');
			if (!pipex->cmd_path)
				return (0);
			if (access(pipex->cmd_path, F_OK | X_OK) == 0)
				return (1);
			free(pipex->cmd_path);
		}
		i++;
	}
	return (0);
}
