/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:45:16 by atran             #+#    #+#             */
/*   Updated: 2025/05/11 22:01:47 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

int		ft_echo(char **argv);
int		ft_pwd(void);
int		ft_cd(char **argv, char **env);
int		export_print(char **env);
int	    ft_export(char **argv, char ***env);
int	    ft_env(char **argv, char **env);

char	*ft_getenv(char **env, char *key);
char	**copy_env(char **envp);
int		ft_setenv(char ***env, char *key, char *n_value);
char	**realloc_env(char **env, int add);

char	*ft_strnstr(const char *big, const char *little, size_t len);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_free_strarr(char **str_arr);
char	*find_path(char *cmd, char **envp);
int		ft_printf(const char *str, ...);
char	replace_space_in_cmd(char *argv);
void	put_back_space(char **cmd, char c);
void	ft_put_err(char *err_msg, char *para);

#endif
