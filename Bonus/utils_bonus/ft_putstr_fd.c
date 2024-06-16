/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 14:35:31 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/16 04:31:51 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

size_t	ft_strlen(char *str)
{
	size_t	n;

	n = 0;
	while (*str++)
		n++;
	return (n);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (fd < 0)
		return ;
	while (*str)
		write(fd, str++, 1);
}
