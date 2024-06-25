/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:24:00 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/25 17:57:10y hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int line_cmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2 && *s1 != '\n')
    {
        s1++;
        s2++;
    }
    if (*s1 == '\n')
        return (0);
    return (*s1 - *s2);
}

void execute_cmd1(t_pipex *pipex, int fds[2])
{
    int fd = open("HERE_DOC", O_RDONLY);
    if (fd < 0)
        err_exit("open");
    pipex->cmd = ft_split(pipex->av[3], ' ');
    if (!pipex->cmd)
        return ;
    dup2(fd, STDIN_FILENO);
    close(fd);
    dup2(fds[1], 1);
    close(fds[1]);
    close(fds[0]);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    err_exit("execve");
}

void execute_cmd2(t_pipex *pipex, int fds[2])
{
    pipex->outfile_fd = open(pipex->av[5], O_RDWR | O_CREAT | O_APPEND, 0777);
    if (pipex->outfile_fd < 0)
        err_exit("outfile");
    pipex->cmd = ft_split(pipex->av[4], ' ');
    if (!pipex->cmd)
        return ;
    dup2(pipex->outfile_fd, 1);
    close(pipex->outfile_fd);
    dup2(fds[0], 0);
    close(fds[1]);
    close(fds[0]);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    err_exit("execve");

}

void parent(t_pipex *pipex, int fds[2])
{
    pid_t id1 = fork();  
    if (id1 < 0)
        err_exit("fork");
    if (id1 == 0)
        execute_cmd2(pipex, fds);
    else
    {
        close(fds[0]);
        close(fds[1]);
        while (wait(NULL) != -1 || errno != ECHILD);
    }
    exit(EXIT_SUCCESS);
}

void heredoc_implement(t_pipex *pipex)
{
    int fds[2];
    int fd = open("HERE_DOC", O_CREAT | O_RDWR | O_TRUNC, 0766);
    if (fd == -1)
        err_exit("open heredoc");
    write(1, "heredoc> ", ft_strlen("heredoc> "));
    char *str = get_next_line(STDIN_FILENO);
    if (!str)
        return ;
    while (str && line_cmp(str, pipex->av[2]))
    {
        write(1, "heredoc> ", ft_strlen("heredoc> "));
        write(fd, str, ft_strlen(str));
        str = get_next_line(STDIN_FILENO);
        if (!str)
            break ;
    }
    close(fd);
    pid_t n = pipe(fds);
    if (n == -1 )
        err_exit("pipe");
    pid_t id = fork();
    if (id == -1)
        err_exit("fork");
    if (id == 0)
        execute_cmd1(pipex, fds);
    else
        parent(pipex, fds);
}
