/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/05/11 21:28:24 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(char **argv, char **env)
{
	int		i;
	char	*eq;

	if (!argv[1])
	{
		i = 0;
		while (env[i])
		{
			eq = ft_strchr(env[i], '=');
			if (eq)
				ft_printf("%s\n", env[i]);
			i++;
		}
	}
	return (0);
}
