/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:45:16 by atran             #+#    #+#             */
/*   Updated: 2025/06/10 20:38:43 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/readline.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef enum e_token_type
{
	RD_INFILE,
	RD_HDQUOTE,
	RD_HEREDOC,
	RD_OUTFILE,
	RD_APPEND,
}					t_etype;

typedef struct s_token
{
	t_etype			type;
	char			*s;
	struct s_token	*next;
}					t_token;

typedef struct s_step
{
	int				pipe;
	t_token			*rd;
	int				hd_fd;
	char			**cmd;
	struct s_step	*next;
}					t_step;

int					is_builtins(char *argv);
int					execute_builtin(char **argv, char ***env);
int					ft_echo(char **argv);
int					ft_pwd(void);
int					ft_cd(char **argv, char **env);
int					ft_export(char **argv, char ***env);
int					ft_env(char **argv, char **env);
int					check_valid_id(char *entry, char c);
int					ft_unset(char **argv, char ***env);
int					export_print(char **env);

char				*ft_getenv(char **env, char *key);
char				**copy_env(char **envp);
int					ft_setenv(char ***env, char *key, char *n_value);
char				**realloc_env(char **env, int add);

void				handle_heredoc(t_step *step);
void				handle_rd(t_step *step);
char				*find_path(char *cmd, char **envp);
void				ft_put_err(char *err_msg, char *para);

int					ft_printf(const char *str, ...);

#endif
