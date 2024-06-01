/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 16:26:29 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/01 16:26:51 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE); // the exit make all the available resource available to the kernel for reallocation
	// the parent ps can recive the EXIT_STATUS using the wait fun
}

void error_handle(char *str)
{
	ft_putstr_fd(str, 2);
	exit(EXIT_FAILURE);
}