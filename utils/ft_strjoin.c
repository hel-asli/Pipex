/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 21:15:53 by hel-asli          #+#    #+#             */
/*   Updated: 2024/05/29 21:18:43 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*str1;
	char	*str2;
	char	*p;
	int		i;
	int		j;

	str1 = (char *)s1;
	str2 = (char *)s2;
	if (!str1 || !str2)
		return (NULL);
	p = malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (str1[i])
	{
		p[i] = str1[i];
		i++;
	}
	j = 0;
	while (str2[j])
		p[i++] = str2[j++];
	p[i] = '\0';
	return (p);
}