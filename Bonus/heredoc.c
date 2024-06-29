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
    if (*s1 == '\n' && *s2 == '\0')
        return (0);
    return (*s1 - *s2);
}

void execute_cmd1(t_pipex *pipex, int fds[2])
{
    pipex->infile_fd = open(FILE_NAME, O_RDONLY);
    if (pipex->infile_fd < 0)
    {
        ft_free(pipex->env_path);
        err_exit("open");
    }
    pipex->cmd = ft_split(pipex->av[3], ' ');
    if (!pipex->cmd)
    {
        ft_free(pipex->env_path);
        err_handler("split");
    }
    dup2(pipex->infile_fd, STDIN_FILENO);
    close(pipex->infile_fd);
    dup2(fds[1], 1);
    close(fds[1]);
    close(fds[0]);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        ft_free(pipex->env_path);
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    ft_free(pipex->cmd);
    ft_free(pipex->env_path);
    err_exit("execve");
}

void execute_cmd2(t_pipex *pipex, int fds[2])
{
    pipex->outfile_fd = open(pipex->av[5], O_RDWR | O_CREAT | O_APPEND, 0777);
    if (pipex->outfile_fd < 0)
    {
        ft_free(pipex->env_path);
        err_exit("outfile");
    }
    pipex->cmd = ft_split(pipex->av[4], ' ');
    if (!pipex->cmd)
    {
        ft_free(pipex->env_path);
        err_handler("split");
    }
    dup2(pipex->outfile_fd, STDOUT_FILENO);
    close(pipex->outfile_fd);
    dup2(fds[0], 0);
    close(fds[1]);
    close(fds[0]);
    if (check_executable(pipex->env_path, &pipex->cmd_path, pipex->cmd[0]))
    {
        ft_free(pipex->env_path);
        execve(pipex->cmd_path, pipex->cmd, pipex->env);
    }
    ft_free(pipex->env_path);
    ft_free(pipex->cmd);
    err_exit("execve");

}


void parent(t_pipex *pipex, pid_t id, int fds[2])
{
    pid_t id1 = fork();  
    int status[2];;
    if (id1 < 0)
        err_exit("fork");
    if (id1 == 0)
        execute_cmd2(pipex, fds);
    else
    {
        ft_free(pipex->env_path);
        close(fds[0]);
        close(fds[1]);
        status[0] = 0;
        if (waitpid(id, &status[0], 0) < 0)
            err_exit("waitpid");
        status[1] = 0;
        if (waitpid(id1, &status[1], 0) < 0)
            err_exit("waitpid");
    }
    exit(WEXITSTATUS(status[1]));
}

void heredoc_file (t_pipex *pipex)
{
    int fd = open(FILE_NAME, O_CREAT | O_RDWR | O_TRUNC, 0766);
    if (fd == -1)
        err_exit("open heredoc");
   
    char *str = ft_strdup("");
    char *ptr = str;
    while (line_cmp(str, pipex->av[2]))
    {
        write(1, "heredoc> ", ft_strlen("heredoc> "));
        str = get_next_line(STDIN_FILENO);
        write(fd, str, ft_strlen(str));
        if (!str)
            return ;
        free(str);
    }
    free(ptr);
    close(fd);
}

void heredoc_implement(t_pipex *pipex)
{
    heredoc_file(pipex);
    
    int fds[2];
    pid_t n = pipe(fds);
    if (n == -1 )
        err_exit("pipe");
    pid_t id = fork();
    if (id == -1)
        err_exit("fork");
    if (id == 0)
        execute_cmd1(pipex, fds);
    else
        parent(pipex, id, fds);
}
