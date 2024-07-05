/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 20:06:33 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/05 20:07:21 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static pid_t	mixed_pids(pid_t id1, pid_t id2)
{
	pid_t	result;

	result = (((id1 >> 4) | (id1 << 4)) ^ ((id2 >> 4) | (id2 << 4)));
	return (result);
}

static void	get_pid_child(t_pipex *pipex, pid_t ids[2])
{
	ids[1] = fork();
	if (ids[1] < 0)
		err_exit("fork");
	if (ids[1] == 0)
	{
		free_res(pipex);
		exit(EXIT_SUCCESS);
	}
	if (ids[1] > 0)
	{
		free_res(pipex);
		if (waitpid(ids[1], NULL, 0) < 0)
			err_exit("waitpid");
		exit(EXIT_SUCCESS);
	}
}

pid_t	get_pid(t_pipex *pipex)
{
	pid_t	ids[2];

	ids[0] = 0;
	ids[1] = 0;
	ids[0] = fork();
	if (ids[0] < 0)
		err_exit("fork");
	if (ids[0] == 0)
		get_pid_child(pipex, ids);
	if (ids[0] > 0)
	{
		if (waitpid(ids[0], NULL, 0) < 0)
			err_exit("waitpid");
	}
	return (mixed_pids(ids[0], ids[1]));
}
