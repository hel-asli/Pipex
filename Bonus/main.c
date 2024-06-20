/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/20 20:03:51 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void err_handler(char *msg)
{
    ft_putstr_fd(msg, 2);
    exit(EXIT_FAILURE);
}

void err_exit(char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}

void close_pipes(pid_t fds[][2], int size)
{
    int i = 0;
    while (i < size)
    {
        close(fds[i][0]);
        close(fds[i][1]);
        i++;
    }
}

void first_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->infile_fd = open(pipex->av[1], O_RDONLY);
    if (pipex->infile_fd < 0)
    {
        ft_free(pipex->env_path);
        close_pipes(fds, j);  
        err_exit("open infile");
    }
    pipex->cmd = ft_split(pipex->av[2], ' ');
    if (!pipex->cmd)
    {
        close_pipes(fds, j);  
        ft_free(pipex->cmd);
        ft_free(pipex->env_path);
        err_exit("ft_split");
    }
    dup2(pipex->infile_fd, STDIN_FILENO);
    close(pipex->infile_fd);
    dup2(fds[j][1], STDOUT_FILENO);
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        ft_free(pipex->env_path);   
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    ft_free(pipex->env_path);
    close_pipes(fds, j);
    ft_free(pipex->cmd);
    err_exit("execve first_cmd");
}

void last_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->outfile_fd = open(pipex->av[pipex->ac - 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (pipex->outfile_fd < 0)
    {
        ft_free(pipex->env_path);
        close_pipes(fds, j);
        err_exit("open outfile");
    }
    pipex->cmd = ft_split(pipex->av[j + 2], ' ');
    if (!pipex->cmd)
        err_exit("ft_split");
    dup2(pipex->outfile_fd, STDOUT_FILENO);
    close(pipex->outfile_fd);
    dup2(fds[j - 1][0], STDIN_FILENO);
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    ft_free(pipex->env_path);
    // close_pipes(fds, j);
    ft_free(pipex->cmd);
    err_exit("execve last_cmd");
}

void other_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->cmd = ft_split(pipex->av[j + 2], ' ');
    if (!pipex->cmd)
        err_handler("split\n");
    dup2(fds[j - 1][0], STDIN_FILENO);
    dup2(fds[j][1], STDOUT_FILENO);
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        ft_free(pipex->env_path);   
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    ft_free(pipex->env_path);
    ft_free(pipex->cmd);
    err_exit("execve other_cmd");
}

void multiple_pipes(t_pipex *pipex, int ac)
{
    int nb = ac - 4;
    int i = 0;
    int j = 0;
    pid_t fds[nb][2];
    int status = 0;

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
    pid_t ids[nb];

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
            {
                first_cmd(pipex, fds, j);
            }
            else if (j == nb)
            {
                last_cmd(pipex, fds, j);
            }
            else
                other_cmd(pipex, fds, j);
        }
        j++;
    }

    ft_free(pipex->env_path);
    close_pipes(fds, nb);

    int a = 0;
    // Parent waits for all child processes to finish
    while(waitpid(ids[a++], &status, 0) != -1 || errno == ECHILD)
    exit(WEXITSTATUS(status));
}

int main(int ac, char **av, char **env) {
    t_pipex pipex;
    pipex.av = av;
    pipex.env = env;
    pipex.ac = ac;

    if (ac < 5)
        err_handler("Insufficient arguments\n");

    if (ft_strcmp(av[1], "here_doc") == 0)
        ft_putstr_fd("here_doc\n", 1);
    else
    {
        check_args(env, &pipex);
        multiple_pipes(&pipex, ac);
    }

    return 0;
}