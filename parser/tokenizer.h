/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:29:22 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/12 20:22:51 by ji-hong          ###   ########.fr       */
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

int		tokenizer(t_token **head, char *s);

void	st_step(t_token **head, t_step **lst);
void	st_lstprint(t_step *lst);
void	st_lstclear(t_step **lst);

int		type_isrd(t_etype type);
int		chk_tokens(t_token **head, char **env);

int		token_quote(char **str, int *i, int heredoc, char **env);
int		token_flag(t_token **head, t_token *cur, int rd, char **env);

int		token_env(t_token **cur, int *i, int rd, char **env);

void	token_env_space(t_token **cur, int *i, t_env *env);

int		ft_isspace(int c);
int		trim_space(char *s);
void	tk_token(t_token **head, t_etype type, t_line *line);

int		token_synerr(t_token **head, char *err_s);
void	synerr(t_token **head, t_line *line, char *err_s);
void	synerr_quote(t_token **head, t_line *line);
void	m_err_exit_token(t_token **head);
void	mem_err_exit_line(t_token **head, t_line *line);

int		synerr_rd(t_token **head, char *err_s);

void	tk_word_token(t_token **head, t_line *line);
//void	tk_op_token(t_token **head, t_line *line);
void	tokenizer_handle_space(t_token **head, t_line *line);
void	tokenizer_handle_op(t_token **head, t_line *line);
void	tokenizer_handle_quote(t_token **head, t_line *line);

t_step	*st_lstnew(t_token **head);
//void	st_lstadd_back(t_step **lst, t_step *new_node);

t_token	*tk_lstnew(t_etype type, char *s);
void	tk_lstadd_front(t_token **head, t_token *new_node);
//int		tk_lstsize(t_token *head);
t_token	*tk_lstlast(t_token *head);
void	tk_lstadd_back(t_token **head, t_token *new_node);

void	tk_lstdelone(t_token **token);
void	tk_lstclear(t_token **head);
//t_token	*tk_lstnth(t_token *head, size_t n);
void	tk_appendlst(t_token **head, t_etype type, char *s);
int		tk_appendfront(t_token **cur, char *s);

char	*get_tk_name(t_etype type);
void	tk_lstprint(t_token *head);

#endif
