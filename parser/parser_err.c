/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-hong <ji-hong@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 14:22:19 by ji-hong           #+#    #+#             */
/*   Updated: 2025/06/18 15:26:52 by ji-hong          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenizer.h"

void	m_err_exit_token(t_token **head)
{
	tk_lstclear(head);
	ft_putstr_fd("Memory allocation failed.\n", 2);
	exit(EXIT_FAILURE);
}

void	mem_err_exit_line(t_token **head, t_line *line)
{
	free(line->tmp);
	m_err_exit_token(head);
}
