/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:29:22 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/26 11:13:20 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "../minishell.h"
# include "../Libft/libft.h"

typedef struct s_line
{
	int		i;
	int		flag;
	int		exit_no;
	char	c;
	char	quote;
	char	*s;
	char	*tmp;
}	t_line;

void	tk_word_token(t_token **head, t_line *line);
void	tokenizer_handle_space(t_token **head, t_line *line);
void	tokenizer_handle_op(t_token **head, t_line *line);
void	tokenizer_handle_quote(t_token **head, t_line *line);

int		type_isrd(t_etype type);
int		ft_isspace(int c);
int		trim_space(char *s);
void	tk_token(t_token **head, t_etype type, t_line *line);

t_token	*tk_lstnew(t_etype type, char *s);
t_token	*tk_lstlast(t_token *head);
void	tk_lstadd_back(t_token **head, t_token *new_node);

void	tk_lstclear(t_token **head);
void	tk_appendlst(t_token **head, t_etype type, char *s);
int		tk_appendfront(t_token **cur, char *s);

t_token	*chk_tokens_loop(t_token **head, int *exit_no, char **env);

int		token_quote(char **str, int *i, int heredoc, char **env);
int		token_flag(t_token **head, t_token *cur, int rd, char **env);

int		token_env(t_token **cur, int *i, int rd, char **env);

int		token_env_expansion(t_token **cur, int *i, t_env *data, int rd);

void	token_env_space(t_token **cur, int *i, t_env *env);

int		token_synerr(t_token **head, char *err_s);
void	synerr(t_token **head, t_line *line, char *err_s);
void	synerr_quote(t_token **head, t_line *line);
int		synerr_rd(t_token **head, char *err_s);

void	m_err_exit_token(t_token **head);
void	mem_err_exit_line(t_token **head, t_line *line);

t_step	*st_lstnew(t_token **head);

#endif
