/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 04:23:50 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_pipes(int **fds, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (close(fds[i][0]) == -1)
			err_exit("close");
		if (close(fds[i][1]) == -1)
			err_exit("close");
		i++;
	}
}

void	ft_pipe(t_pipex *pipex, pid_t *fds[], int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		if (pipe(fds[i]) == -1)
		{
			ft_free(pipex->env_path);
			close_pipes(fds, i);
			err_exit("pipe");
		}
		i++;
	}
}

void	multiple_pipe_helper(t_pipex *pipex, pid_t *fds[2], pid_t *ids, int nb)
{
	int	j;

	j = 0;
	while (j <= nb)
	{
		ids[j] = fork();
		if (ids[j] < 0)
		{
			ft_free(pipex->env_path);
			close_pipes(fds, nb);
			err_exit("fork");
		}
		if (ids[j] == 0)
		{
			if (j == 0)
				first_cmd(pipex, fds, j);
			else if (j == nb)
				last_cmd(pipex, fds, j);
			else
				other_cmd(pipex, fds, j);
		}
		j++;
	}
}

void	multiple_pipes(t_pipex *pipex, int ac)
{
	int		i;
	int		nb;
	int		status;

	nb = ac - 4;
	data_alloc(pipex, nb);
	ft_pipe(pipex, pipex->fds, nb);
	multiple_pipe_helper(pipex, pipex->fds, pipex->ids, nb);
	close_pipes(pipex->fds, nb);
	free_res(pipex);
	status = 0;
	i = 0;
	while (i <= nb)
	{
		if (waitpid(pipex->ids[i], &status, 0) == -1 || errno == ECHILD)
			err_exit("waitpid");
		i++;
	}
	free(pipex->ids);
	pipex->ids = NULL;
	exit(WEXITSTATUS(status));
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	pipex_init(&pipex, av, env, ac);
	if (ac == 6 && ft_strcmp(av[1], "here_doc") == 0)
	{
		check_args(env, &pipex);
		heredoc_implement(&pipex);
	}
	else if (ac >= 5 && ft_strcmp(av[1], "here_doc"))
	{
		check_args(env, &pipex);
		multiple_pipes(&pipex, ac);
	}
	else
		err_handler(ERR_MSG);
	return (0);
}
