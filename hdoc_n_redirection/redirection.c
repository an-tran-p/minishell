/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/15 00:17:32 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	handle_infile(t_token *redirection, t_step *step, char **env)
{
	int		fd_in;
	t_token	*rd;
	int		inf_num;

	rd = redirection;
	inf_num = count_infile(redirection);
	while (rd)
	{
		if (rd->type == RD_INFILE)
		{
			inf_num--;
			fd_in = open(rd->s, O_RDONLY);
			if (fd_in == -1)
			{
				ft_printf("minishell: %s: %s\n", strerror(errno), rd->s);
				ft_free_eve(step, env);
				exit(1);
			}
			else if (inf_num == 0 && infile_last(redirection) == 1)
				dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		rd = rd->next;
	}
}

void	handle_outfile(t_token *redirection, t_step *step, char **env)
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
				ft_free_eve(step, env);
				exit(1);
			}
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		rd = rd->next;
	}
}

void	handle_rd(t_step *st, t_step *step, char **env)
{
	t_token	*rd;
	t_step	*temp;

	rd = st->rd;
	if (!rd)
		return ;
	if (st->hd_fd != -1 && st->hd_fd != -2)
	{
		dup2(st->hd_fd, STDIN_FILENO);
		close(st->hd_fd);
	}
	handle_infile(st->rd, step, env);
	handle_outfile(st->rd, step, env);
	temp = step;
	while (temp)
	{
		if (temp->hd_fd != -1 && temp->hd_fd != -2)
			close(temp->hd_fd);
		temp = temp->next;
	}
}
