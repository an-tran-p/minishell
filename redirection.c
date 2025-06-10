/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/10 21:18:25 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_infile(t_token *redirection)
{
	int		num;
	t_token	*rd;

	num = 0;
	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_INFILE)
			num++;
		rd = rd->next;
	}
	return (num);
}

int	infile_last(t_token *redirection)
{
	int		flag;
	t_token	*rd;

	flag = 0;
	rd = redirection;
	if (!rd)
		return (0);
	while (rd)
	{
		if (rd->type == RD_INFILE)
			flag = 1;
		if (rd->type == RD_HDQUOTE || rd->type == RD_HEREDOC)
			flag = 2;
		rd = rd->next;
	}
	return (flag);
}

void	handle_infile(t_token *redirection)
{
	int		fd_in;
	t_token	*rd;
	int		inf_num;
	int		inf_last;

	rd = redirection;
	inf_num = count_infile(redirection);
	inf_last = infile_last(redirection);
	while (rd)
	{
		if (rd->type == RD_INFILE)
		{
			inf_num--;
			fd_in = open(rd->s, O_RDONLY);
			if (fd_in == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), rd->s);
				exit(1);
			}
			else if (inf_num == 0 && inf_last == 1)
				dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		rd = rd->next;
	}
}

void	handle_outfile(t_token *redirection)
{
	int		fd_out;
	t_token	*rd;

	rd = redirection;
	if (!rd)
		return ;
	while (rd)
	{
		if (rd->type == RD_OUTFILE || rd->type == RD_APPEND)
		{
			if (rd->type == RD_OUTFILE)
				fd_out = open(rd->s, O_RDWR | O_CREAT | O_TRUNC, 0777);
			else if (rd->type == RD_APPEND)
				fd_out = open(rd->s, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (fd_out == -1)
			{
				ft_printf("%s: %s\n", strerror(errno), rd->s);
				exit(1);
			}
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		rd = rd->next;
	}
}

void	handle_rd(t_step *step)
{
	t_token	*rd;

	rd = step->rd;
	if (step->hd_fd > 0)
	{
		dup2(step->hd_fd, STDIN_FILENO);
		close(step->hd_fd);
	}
	handle_infile(step->rd);
	handle_outfile(step->rd);
}
