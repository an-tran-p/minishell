/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/04 21:44:18 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char **argv)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	if (argv[1] && ft_strncmp(argv[1], "-n", 3) == 0)
	{
		n = 1;
		i = 1;
	}
	while (argv[++i])
	{
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));
		if (argv[i + 1])
			ft_printf(" ");
	}
	if (n == 0)
		write(STDOUT_FILENO, "\n", strlen("\n"));
	return (0);
}
