/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/20 20:18:24 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void free_res(t_pipex *pipex)
{
    if (!pipex)
        return ;
    if (pipex->cmd_path)
    {
        free(pipex->cmd_path);
        pipex->cmd_path = NULL;
    }
    if (pipex->cmd)
    {
        ft_free(pipex->cmd);
        pipex->cmd_path = NULL;
    }
    if (pipex->env_path)
    {
        ft_free(pipex->env_path);
        pipex->cmd_path = NULL;
    }
}

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
        if (close(fds[i][0]) == -1)
            err_exit("close");
        if (close(fds[i][1]) == -1)
            err_exit("close");
        i++;
    }
}

void first_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->infile_fd = open(pipex->av[1], O_RDONLY);
    if (pipex->infile_fd < 0)
    {
        free_res(pipex);
        close_pipes(fds, j);  
        err_exit("open infile");
    }
    pipex->cmd = ft_split(pipex->av[2], ' ');
    if (!pipex->cmd || !pipex->cmd[0])
    {
        close_pipes(fds, pipex->ac - 4);  
        close(pipex->infile_fd);
        free_res(pipex);
        err_handler("ft_split");
    }
    if (dup2(pipex->infile_fd, STDIN_FILENO) == -1)
        err_exit("dup");
    if (close(pipex->infile_fd) == -1)
        err_exit("close");
    if (dup2(fds[j][1], STDOUT_FILENO) == -1)
        err_exit("dup");
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        ft_free(pipex->env_path);
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    free_res(pipex);
    err_exit("execve first_cmd");
}

void last_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->outfile_fd = open(pipex->av[pipex->ac - 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (pipex->outfile_fd == -1)
    {
        free_res(pipex);
        close_pipes(fds, pipex->ac - 4);
        err_exit("open outfile");
    }
    pipex->cmd = ft_split(pipex->av[j + 2], ' ');
    if (!pipex->cmd || !pipex->cmd[0])
    {
        close_pipes(fds, pipex->ac - 4);
        close(pipex->outfile_fd);
        free_res(pipex);
        err_handler("ft_split");
    }
    if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
        err_exit("dup");
    if (close(pipex->outfile_fd) == -1)
        err_exit("close");
    if (dup2(fds[j - 1][0], STDIN_FILENO) == -1)
        err_exit("dup2");
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
         execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    free_res(pipex);
    err_exit("execve last_cmd");
}

void other_cmd(t_pipex *pipex, pid_t fds[][2], int j)
{
    pipex->cmd = ft_split(pipex->av[j + 2], ' ');
    if (!pipex->cmd)
    {
        free_res(pipex);
        close_pipes(fds, pipex->ac - 4);
        err_handler("split\n");
    }
    dup2(fds[j - 1][0], STDIN_FILENO);
    dup2(fds[j][1], STDOUT_FILENO);
    close_pipes(fds, pipex->ac - 4);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    free_res(pipex);
    err_exit("execve other_cmd");
}

void multiple_pipes(t_pipex *pipex, int ac)
{
    pipex->cmd = NULL;
    pipex->cmd_path = NULL;
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
    pid_t ids[nb + 1];

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

    free_res(pipex);
    close_pipes(fds, nb);

    int a = 0;
    // Parent waits for all child processes to finish

    while(a <= nb)
    {
        if (waitpid(ids[a], &status, 0) == -1 || errno == ECHILD)
            err_exit("waitpid");
        a++;
    }

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

// 0x401ABE: