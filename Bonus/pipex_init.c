/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 02:49:39 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 02:53:02 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	pipex_init(t_pipex *pipex, char **av, char **env, int ac)
{
	pipex->av = av;
	pipex->env = env;
	pipex->ac = ac;
	pipex->cmd = NULL;
	pipex->cmd_path = NULL;
	pipex->here_doc = NULL;
	pipex->fds = NULL;
	pipex->ids = NULL;
}

void	data_alloc(t_pipex *pipex, int nb)
{
	pipex->fds = fds_allocation(nb);
	if (!pipex->fds)
	{
		free_res(pipex);
		exit(EXIT_FAILURE);
	}
	pipex->ids = malloc(sizeof(pid_t) * (nb + 1));
	if (!pipex->ids)
	{
		free_res(pipex);
		exit(EXIT_FAILURE);
	}
}
