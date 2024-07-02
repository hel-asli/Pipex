/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 03:30:31 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/30 03:31:06 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

static unsigned int	count_numbers(int n)
{
	unsigned int	count;
	long			tmp;

	count = 0;
	tmp = n;
	if (tmp < 0)
	{
		count++;
		tmp = tmp * -1;
	}
	while (tmp / 10)
	{
		count++;
		tmp = tmp / 10;
	}
	count++;
	return (count);
}

char	*ft_itoa(int n)
{
	char			*s;
	unsigned int	len;
	unsigned int	i;
	long			tmp;

	tmp = n;
	len = count_numbers(n);
	s = (char *)malloc(len + 1);
	if (!s)
		return (NULL);
	i = len;
	s[i] = '\0';
	i -= 1;
	if (tmp < 0)
	{
		tmp = tmp * -1;
		s[0] = '-';
	}
	while (tmp / 10)
	{
		s[i--] = tmp % 10 + '0';
		tmp = tmp / 10;
	}
	s[i] = tmp + '0';
	return (s);
}
