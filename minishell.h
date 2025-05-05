/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:45:16 by atran             #+#    #+#             */
/*   Updated: 2025/05/05 20:23:42 by atran            ###   ########.fr       */
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
