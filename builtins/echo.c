/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 22:48:46 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_not_n(char *s)
{
	int	i;

	i = 1;
	while (s[i])
	{
		if (s[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	ft_echo(char **argv)
{
	int	i;
	int	n;

	i = 1;
	n = 0;
	while (argv[i])
	{
		if (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0
			&& !check_not_n(argv[i]))
			n = 1;
		else
			break ;
		i++;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (n == 0)
		ft_printf("\n");
	return (0);
}
