/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:25:02 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/12 22:02:18 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

static void	create_tokens(t_token **head, t_line *line)
{
	while (line->s[line->i])
	{
		if (ft_isspace(line->s[line->i]))
			tokenizer_handle_space(head, line);
		else if (line->s[line->i] == '|' || line->s[line->i] == '<'
			|| line->s[line->i] == '>')
			tokenizer_handle_op(head, line);
		else if (line->s[line->i] == '\'' || line->s[line->i] == '\"')
			tokenizer_handle_quote(head, line);
		else if (line->s[line->i] == '$')
			line->flag ++;
		if (line->exit_no)
			return ;
		line->i ++;
	}
	if (line->s[0])
		tk_word_token(head, line);
}

static void	init_t_line(t_line *line, char *s)
{
	line->i = 0;
	line->flag = 0;
	line->exit_no = 0;
	line->c = 0;
	line->quote = 0;
	line->s = s;
	line->tmp = s;
}

int	tokenizer(t_token **head, char *s)
{
	t_line	line;

	init_t_line(&line, s);
	line.s += trim_space(line.s);
	if (!line.s[0])
		return (0);
	create_tokens(head, &line);
	if (line.exit_no)
		return (line.exit_no);
	tk_lstprint(*head);
	return (0);
}

int	parser(char *s, t_step **lst, char **env)
{
	int		exit_no;
	t_token	*head;

	head = NULL;
	exit_no = tokenizer(&head, s);
	if (!head || exit_no)
		return (exit_no);
	exit_no = chk_tokens(&head, env);
	if (exit_no)
		return (exit_no);
	if (head)
	{
		tk_lstprint(head);
		st_step(&head, lst);
		st_lstprint(*lst);
	}
//	tk_lstclear(&head);
	return (exit_no);
}

/*
int	main(void)
{
	int		status;
	int		len;
	char	str[] =
//		"<$A cat >a|cat -e a";
//		"<<eofi\"\" cat a$A. $A elk | >a grep \"ads\">b | sort output.txt ";
//		"cat test.txt | grep \"apple $USER $_a\" | sort output.txt ";
//		" <a echo $USER >b |ca\"t\" 'Hello \"world\"'df|cmd arg$PWD  ";
		">a $AA e\"c\"h$A |ehcho ihid >a |kl|>bb";
//		" <Aa \"$A sklsd\" $A $A";
	char	*line;
	t_step	*lst;

	lst = NULL;
	len = ft_strlen(str);
	line = (char *)ft_calloc(len + 1, sizeof(char));
	if (!line)
		exit(1);
	ft_memcpy(line, str, len);
	status = parser(line, &lst);
	if (lst)
		st_lstprint(lst);
 	st_lstclear(&lst);
//	status = parser(line);
	if (status)
		printf("exit_no: %d\n", status);
}
*/
