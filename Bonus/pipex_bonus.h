/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 02:34:18 by hel-asli          #+#    #+#             */
/*   Updated: 2024/07/02 02:45:11 by hel-asli         ###   ########.fr       */
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
# include <limits.h>
# include <signal.h>

# define ERR_MSG "BAD ARGS : infile cmd1 cmd2 outfile ;)\n"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

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
	char	*here_doc;
	pid_t	**fds;
	pid_t	*ids;
}	t_pipex;

size_t	ft_strlen(char *str);
void	free_res(t_pipex *pipex);
char	**ft_split(char const *s, char c);
void	ft_putstr_fd(char *str, int fd);
char	*ft_strjoin_del(const char *s1, const char *s2, char c);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strcmp(char *s1, char *s2);
void	check_args(char *env[], t_pipex *pipex);
int		stats_with(char *str);
int		find_path(char **env);
char	**ft_free(char **split);
int		check_executable(char **env_path, char **path, char *cmd_name);
char	*get_next_line(int fd, char *stop_line);
int		ft_strchr(char *str);
char	*ft_strdup(char *str);
void	heredoc_implement(t_pipex *pipex);
void	err_exit(char *str);
void	err_handler(char *msg);
char	*ft_itoa(int n);
void	free_res(t_pipex *pipex);
void	err_handler(char *msg);
void	err_exit(char *str);
void	first_cmd_helper(t_pipex *pipex, pid_t **fds, int j);
void	last_cmd_helper(t_pipex *pipex, pid_t **fds, int j);
void	first_cmd(t_pipex *pipex, pid_t **fds, int j);
void	last_cmd(t_pipex *pipex, pid_t **fds, int j);
void	other_cmd(t_pipex *pipex, pid_t **fds, int j);
void	close_pipes(pid_t **fds, int size);
void	cmd1_helper(t_pipex *pipex, int fds[2]);
void	cmd2_helper(t_pipex *pipex, int fds[2]);
void	heredoc_file(t_pipex *pipex);
char	*get_file_name(void);
char	*get_line(const char *s1, const char *s2);
void	free_fds(pid_t **fds, int nb);
pid_t	**fds_allocation(int nb);

#endif
