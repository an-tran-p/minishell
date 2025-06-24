/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/24 21:11:34 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_hdoc(t_token *redirection)
{
	int		num;
	t_token	*rd;

	num = 0;
	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
			num++;
		rd = rd->next;
	}
	return (num);
}

void	ft_put_warning_eof(char *s)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file ", 2);
	ft_putstr_fd("(wanted '", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("')\n", 2);
}

void	sigint_heredoc_clean(char **env, t_step *step, char **line)
{
	ft_free_str(line);
	close_hd(step);
	ft_free_eve(step, env);
	exit(130);
}
