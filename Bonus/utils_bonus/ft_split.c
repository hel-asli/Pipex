/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 14:37:36 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/03 05:09:56 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex_bonus.h"

int	count_words(char const *s, char c)
{
	int		count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s != c && *s)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_free(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	return (NULL);
}

char	*split_word(char *str, char c)
{
	int		i;
	int		j;
	char	*word;

	i = 0;
	j = 0;
	while (str[i] && str[i] != c)
		i++;
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	while (j < i)
	{
		word[j] = str[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**split;

	i = 0;
	split = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!split || !s)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			split[i] = split_word((char *)s, c);
			if (!split[i])
				return (ft_free(split));
			i++;
			s++;
		}
		while (*s && *s != c)
			s++;
	}
	split[i] = NULL;
	return (split);
}
