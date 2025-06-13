/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:45:16 by atran             #+#    #+#             */
/*   Updated: 2025/06/13 20:30:31 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "Libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_env
{
	int				ex_free;
	int				j;
	int				count;
	int				m_err;
	int				len_expand;
	int				len_tmp;
	char			*var;
	char			*expand;
	char			*s;
	char			*tmp;
}					t_env;

typedef enum e_token_type
{
	WORD_F,
	WORD,
	DEL,
	RD_INFILE,
	RD_HDQUOTE,
	RD_HEREDOC,
	RD_OUTFILE,
	RD_APPEND,
	RD_ERR,
	PIPE
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

typedef struct s_del
{
	struct s_step	**head;
	char			***env;
}					t_del;

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

void				handle_heredoc(t_step *step, char **env);
void				handle_rd(t_step *step);
char				*find_path(char *cmd, char **envp);
void				ft_put_err(char *err_msg, char *para);
int					create_processes(t_step *step, char **env);
int					execute_single_cmd(t_step *step, char ***env);

int					ft_printf(const char *str, ...);
/* void				del_all(t_del *del, int i); */

int					exit_status(int new_s, bool add);

void				token_get_env(char **str, int *i, t_env *data, char **env);
int					token_expand_str(char **str, int *i, t_env *data);
int					token_quote_env(char **str, int *i, char **env);
int					heredoc_expand(char **s, char **env);

int					parser(char *s, t_step **lst, char **env);
void				st_step(t_token **head, t_step **lst);
void				st_lstprint(t_step *lst);
void				st_lstclear(t_step **lst);

#endif
