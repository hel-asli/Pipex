/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:34:18 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/01 17:11:59 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX
# define PIPEX

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

#define ERR_MSG "BAD ARGS : infile cmd1 cmd2 outfile :)"

/**/
typedef struct s_pipex
{
	int		infile_fd;
	int		outile_fd;
	char	*path_cmd1;
	char	*path_cmd2;
	char	**env_path; // split the Path With :
	char	**first_cmd;
	char	**second_cmd;
	int		fds[2];
} t_pipex;

size_t	ft_strlen(char *str);
char	**ft_split(char const *s, char c);
void	ft_putstr_fd(char *str, int fd);
char	*ft_strjoin(const char *s1, const char *s2, char c);
void	check_args(int ac, char *av[], t_pipex *pipex);
void	check_env(char **env, t_pipex *pipex);
int		check_executable(char **env_path, char **path, char *cmd_name);
void	err_exit(char *str);
void error_handle(char *str);

# endif
