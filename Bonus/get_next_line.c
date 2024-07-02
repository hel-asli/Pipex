/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 16:32:09 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 02:06:22 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*read_file(char *buffer, int fd)
{
	char	*str;
	int		n;

	if (!buffer)
		return (NULL);
	str = malloc(sizeof(char) * ((size_t)BUFFER_SIZE + 1));
	if (!str)
		return (free(buffer), buffer = NULL, NULL);
	n = 1;
	while (!ft_strchr(buffer) && n != 0)
	{
		n = read(fd, str, BUFFER_SIZE);
		if (n < 0)
			return (free(buffer), buffer = NULL, free(str), NULL);
		str[n] = 0;
		buffer = ft_strjoin(buffer, str);
		if (!buffer)
			return (free(buffer), buffer = NULL, NULL);
	}
	return (free(str), str = NULL, buffer);
}

static char	*read_line(char *buffer)
{
	char	*line;
	int		len;
	int		i;

	if (!buffer)
		return (NULL);
	len = 0;
	while (buffer[len] && buffer[len] != '\n')
		len++;
	if (buffer[len] == '\n')
		len++;
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = 0;
	return (line);
}

char	*read_rest(char *buffer)
{
	int		i;
	char	*rest;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	rest = ft_strdup(&buffer[i]);
	free(buffer);
	buffer = NULL;
	return (rest);
}

char	*get_next_line(int fd, char *stop_line)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buffer == NULL)
		buffer = ft_strdup("");
	buffer = read_file(buffer, fd);
	if (!buffer)
		return (NULL);
	if (!*buffer)
		return (free(buffer), buffer = NULL, NULL);
	line = read_line(buffer);
	if (!line)
		return (free(buffer), buffer = NULL, NULL);
	buffer = read_rest(buffer);
	if (!buffer)
		return (free(line), line = NULL, NULL);
	if (!ft_strcmp(line, stop_line))
	{
		free(buffer);
		buffer = NULL;
	}
	return (line);
}
