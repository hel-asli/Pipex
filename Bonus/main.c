/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/01/22 09:46:59 by hel-asli         ###   ########.fr       */
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

void multiple_pipes(t_pipex *pipex, int ac)
{
    int nb = ac - 3;
    pid_t fds[nb][2];
    int i= 0;
    int j = 0;
    while (i < nb)
    {
        if (pipe(fds[i]));
            err_exit("pipe");
        i++;
    }
        /*

            ---> parent
            fork() --> 1 child ->execute the first command ;
            
            ---> parent  waitpid ();
        */
        // ./pipex *** infile cmd1 ***| cmd2 | cmd3 | cmd4 outfile
        // first child  
        // other command
        // pipes_helper();
        while (j <= nb - 1)
        {
            // fork;
            // j == 0;
            // execute the first command;
            // j == nb  - 1 
            // execute last command
            // j > 0 && j < nb - 1
            // execute other cmd
            pid_t id = fork(); 
            int status; 
            if (id < 0)
                err_exit("fork"); // free and close
            if (id == 0)
            {
                ft_putstr_fd("execute cmd***", 1);
                exit(EXIT_SUCCESS);
            }
            else
                waitpid(id, &status, 0);
        }
}

int main(int ac, char **av, char **env)
{
    t_pipex pipex; 
    (void)av;
    (void)env;
    if (ac < 5)
        err_handler("hhhhhoh");
    
    if (ft_strcmp(av[1], "here_doc") == 0)
    {
        // helper function here_doc
        ft_putstr_fd("here_doc", 1);
    }
    else
    {
        multiple_pipes(&pipex, ac);
    }
        
    // printf("hellow rodl");
}