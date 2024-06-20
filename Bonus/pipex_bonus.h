/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:34:18 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/20 10:12:52 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>

# define ERR_MSG "BAD ARGS : infile cmd1 cmd2 outfile ;)\n"

typedef struct s_pipex
{
	int		infile_fd;
	int		outfile_fd;
	char	*cmd_path;
	char	**env;
	char	**av;
	int		ac;
	char	**cmd;
	char	**env_path;
}	t_pipex;

size_t	ft_strlen(char *str);
char	**ft_split(char const *s, char c);
void	ft_putstr_fd(char *str, int fd);
char	*ft_strjoin(const char *s1, const char *s2, char c);
int		ft_strcmp(char *s1, char *s2);
void	check_args(char *env[], t_pipex *pipex);
int		stats_with(char *str);
int		find_path(char **env);
char	**ft_free(char **split);
int		check_executable(char **env_path, char **path, char *cmd_name);
// void	error_handler(char *str);

#endif