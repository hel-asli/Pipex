/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 11:53:17 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/20 19:59:35 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_lsof(void)
{
	system("leaks pipex");
}

// write a function to free resource . 


// void free_resource(t_pipex *pipex)
// {
//     if (close(pipex->infile_fd) == -1)
//         err_exit("close");
//     if (close(pipex->infile_fd) == -1)
//         err_exit("close");
//     if (close(pipex->fds[0]) == -1 || close(pipex->fds[1]) == -1)
//         err_exit("close");
// }
void child_helper(t_pipex *pipex, char **av, int flag)
{
    if (flag == 0)
    {
        pipex->infile_fd = open(av[1], O_RDONLY);
        if (pipex->infile_fd == -1)
        {
            ft_free(pipex->env_path);
            close(pipex->fds[0]);
            close(pipex->fds[1]);
            err_exit("open infile");
        }
        pipex->first_cmd = ft_split(av[2], ' ');
        if (!pipex->first_cmd || pipex->first_cmd[0] == 0)
        {
            ft_free(pipex->env_path);
            ft_free(pipex->first_cmd);
            close(pipex->fds[0]);
            close(pipex->fds[1]);
            if (close(pipex->infile_fd) == -1)
                err_exit("close infile");
            exit(EXIT_FAILURE);
        }
    }
    else if (flag == 1)
    {
        pipex->outfile_fd = open(av[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        if (pipex->outfile_fd == -1)
        {
            ft_free(pipex->env_path);
            close(pipex->fds[0]);
            close(pipex->fds[1]);
            err_exit("open outfile");
        }
        pipex->second_cmd = ft_split(av[3], ' ');
        if (!pipex->second_cmd || pipex->second_cmd[0] == 0)
        {
            ft_free(pipex->env_path);
            close(pipex->fds[0]);
            close(pipex->fds[1]);
            if (close(pipex->outfile_fd) == -1)
                err_exit("close outfile");
            ft_free(pipex->second_cmd);
            exit(EXIT_FAILURE);
        }
    }
}

void first_child(t_pipex *pipex, char *av[], char **env)
{
    child_helper(pipex, av, 0); // open infile && split cmd
    if (dup2(pipex->infile_fd, STDIN_FILENO) == -1)
        err_exit("dup infile to stdin");
    if (close(pipex->infile_fd) == -1)
        err_exit("close infile of pipe");
    if (dup2(pipex->fds[1], STDOUT_FILENO) == -1)
        err_exit("dup write-end of the pipe to stdout");
    if (close(pipex->fds[0]) == -1)
        err_exit("close the read end of the pipe");
    if (close(pipex->fds[1]) == -1)
        err_exit("close write end of pipe");
    if (check_executable(pipex->env_path, &pipex->path_cmd1, pipex->first_cmd[0]))
    {
        ft_free(pipex->env_path);
        execve(pipex->path_cmd1, pipex->first_cmd, env);
    }
    ft_free(pipex->env_path);
    ft_free(pipex->first_cmd);
    err_exit("execve");
}

void second_child(t_pipex *pipex, char **av, char **env)
{
    child_helper(pipex, av, 1);
    if (dup2(pipex->outfile_fd, STDOUT_FILENO) == -1)
        err_exit("dup the outfile to the stdout");
    if (close(pipex->outfile_fd) == -1)
        err_exit("close the outfile");
    if (dup2(pipex->fds[0], STDIN_FILENO) == -1)
        err_exit("dup the read end of the pipe to stdin");
    if (close(pipex->fds[1]) == -1)
        err_exit("close the write end of the pipe");
    if (close(pipex->fds[0]) == -1)
        err_exit("close the read end of the pipe");
    if (check_executable(pipex->env_path, &pipex->path_cmd2, pipex->second_cmd[0]))
        execve(pipex->path_cmd2, pipex->second_cmd, env);
    ft_free(pipex->env_path);
    ft_free(pipex->second_cmd);
    err_exit("execve");
}

void parent(t_pipex *pipex, pid_t pid1, char **av, char **env)
{
    pid_t pid;
    int status[2] = {0};

    pid = fork();
    if (pid == -1)
        err_exit("fork");
    if (pid == 0)
    {
        second_child(pipex, av, env);
    }
    else
    {
        close(pipex->fds[0]);
        close(pipex->fds[1]);
        ft_free(pipex->env_path);
        if (waitpid(pid1, &status[0], 0) == -1)
            err_exit("waitpid");
        if (waitpid(pid, &status[1], 0) == -1)
            err_exit("waitpid");
        exit(WEXITSTATUS(status[1]));
    }
}

int main(int ac, char *av[], char *env[])
{
    t_pipex pipex;
    pid_t pid1;

    check_args(ac, env, &pipex);
    if (pipe(pipex.fds) == -1)
    {
        ft_free(pipex.env_path);
        err_exit("pipe");
    }
    pid1 = fork();
    if (pid1 == -1)
    {
        close(pipex.fds[0]);
        close(pipex.fds[1]);
        ft_free(pipex.env_path);
        err_exit("fork");
    }
    else if (pid1 == 0)
    {
        first_child(&pipex, av, env);
    }
    else
    {
        parent(&pipex, pid1, av, env);
    }
    exit(EXIT_SUCCESS);
}
