/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cases.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/30 21:44:20 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// exmaple: echo Test >> test.txt | cat Test.tct | grep Te | wc-l
t_pipe_step	*test_1(void)
{
	t_pipe_step	*n1;
	t_pipe_step	*n2;
	t_pipe_step	*n3;
	t_pipe_step	*n4;

	n1 = malloc(sizeof(t_pipe_step));
	n2 = malloc(sizeof(t_pipe_step));
	n3 = malloc(sizeof(t_pipe_step));
	n4 = malloc(sizeof(t_pipe_step));
	// Node 1: echo Test >> test.txt
	n1->cmd = (char *[]){"echo", "Test", NULL};
	n1->infile = NULL;
	n1->heredoc_deli = NULL;
	n1->outfile = strdup("test.txt");
	n1->append = 1;
	n1->pipe = 1;
	n1->next = n2;
	// Node 2: cat test.txt
	n2->cmd = (char *[]){"cat", "test.txt", NULL};
	n2->infile = NULL;
	n2->heredoc_deli = NULL;
	n2->outfile = NULL;
	n2->append = 0;
	n2->pipe = 1;
	n2->next = n3;
	// Node 3: grep Te
	n3->cmd = (char *[]){"grep", "Te", NULL};
	n3->infile = NULL;
	n3->heredoc_deli = NULL;
	n3->outfile = NULL;
	n3->append = 0;
	n3->pipe = 1;
	n3->next = n4;
	// Node 4: wc -l
	n4->cmd = (char *[]){"wc", "-l", NULL};
	n4->infile = NULL;
	n4->heredoc_deli = NULL;
	n4->outfile = NULL;
	n4->append = 0;
	n4->pipe = 0;
	n4->next = NULL;
	return (n1);
}
