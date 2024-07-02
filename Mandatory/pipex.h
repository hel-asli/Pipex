/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:34:18 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 04:59:53 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>

// #define malloc(x) NULL

# define ERR_MSG "BAD ARGS : infile cmd1 cmd2 outfile\n"

typedef struct s_pipex
{
	int		infile_fd; /** < input file descriptor */
	int		outfile_fd; /** < output file descriptor */
	char	*path_cmd1; /** < path to First Command executable*/
	char	*path_cmd2; /** < path to Second Command executable*/
	char	**env_path; /** < PATH split from envirement Variable*/
	char	**first_cmd; /** < Argv for First Command */
	char	**second_cmd; /** < Argv for Second Command */
	int		fds[2]; /** < pipe file Descriptor */
}	t_pipex;

size_t	ft_strlen(char *str);
char	**ft_split(char const *s, char c);
void	ft_putstr_fd(char *str, int fd);
char	*ft_strjoin(const char *s1, const char *s2, char c);
void	check_args(int ac, char *env[], t_pipex *pipex);
int		check_executable(char **env_path, char **path, char *cmd_name);
void	err_exit(char *str);
void	error_handle(char *str);
char	**ft_free(char **split);
void	ft_parent(t_pipex *pipex);
int		find_path(char **env);
void	free_resource(t_pipex *pipex);
void	error_handle(char *str);
void	first_child_helper(t_pipex *pipex, char **av);
void	second_child_helper(t_pipex *pipex, char **av);

#endif
