/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 04:02:23 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/16 13:56:19 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void err_handler(char *msg)
{
    ft_putstr_fd(msg, 2);
    exit(EXIT_FAILURE);  
}

int main(int ac, char **av, char **env)
{
    (void)av;
    (void)env;
    if (ac < 5)
        err_handler("hhhhhoh");
    
    if (ft_strcmp(av[1], "here_doc") == 0)
        printf("here_doc yalsalam\n");
    else
        printf("randir l mlayr d mlayr d pipes");
        
    // printf("hellow rodl");
}