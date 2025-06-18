/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:49:00 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/18 13:36:00 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

int	type_isrd(t_etype type)
{
	if (type == RD_INFILE || type == RD_HEREDOC || type == RD_OUTFILE
		|| type == RD_APPEND || type == RD_HDQUOTE || type == RD_ERR)
		return (1);
	else
		return (0);
}

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n')
		return (1);
	else
		return (0);
}

int	trim_space(char *s)
{
	int	i;
	int	len;

	i = 0;
	while (ft_isspace(s[i]))
		i ++;
	len = ft_strlen(s);
	if (len > i)
	{
		while (ft_isspace(s[len - 1]))
		{
			s[len - 1] = 0;
			len --;
		}
	}
	return (i);
}

void	tk_token(t_token **head, t_etype type, t_line *line)
{
	char	*str;

	str = (char *)ft_calloc(line->i + 1, sizeof(char));
	if (!str)
		mem_err_exit_line(head, line);
	ft_memcpy(str, line->s, line->i);
	tk_appendlst(head, type, str);
	if (!(*head))
		mem_err_exit_line(head, line);
	line->s = line->s + line->i;
	line->flag = 0;
}
