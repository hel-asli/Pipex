/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/30 18:38:15 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void close_pipes(pid_t fds[][2], int size)
{
	int i = 0;
	while (i < size)
	{
		if (close(fds[i][0]) == -1)
			err_exit("close");
		if (close(fds[i][1]) == -1)
			err_exit("close");
		i++;
	}
}


void multiple_pipe_helper(t_pipex *pipex, pid_t ids[], pid_t fds[][2], int nb)
{
	int j = 0;

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

void ft_pipe(t_pipex *pipex, int fds[][2], int nb)
{
	int i = 0;

	while(i < nb)
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

void multiple_pipes(t_pipex *pipex, int ac)
{
	int		i;
	int nb ;
	pid_t fds[ac - 4][2];
	pid_t ids[ac - 3];
	int status;

	nb = ac - 4;
	ft_pipe(pipex, fds, nb);
	multiple_pipe_helper(pipex, ids, fds, nb);
	free_res(pipex);
	close_pipes(fds, nb);
	status = 0;
	i = 0;
	while(i <= nb)
	{
		if (waitpid(ids[i], &status, 0) == -1 || errno == ECHILD)
			err_exit("waitpid");
		i++;
	}

	exit(WEXITSTATUS(status));
}


int main(int ac, char **av, char **env)
{
	t_pipex pipex;
	pipex.av = av;
	pipex.env = env;
	pipex.ac = ac;
	pipex.cmd = NULL;
	pipex.cmd_path = NULL;
	pipex.here_doc = NULL;


	if (ft_strcmp(av[1], "here_doc") == 0 && ac == 6)
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
		err_handler("Insufficient arguments\n");

	return 0;
}

// 0x401ABE:
