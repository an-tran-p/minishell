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

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **argv, char **env, t_step *step)
{
	int	status;

	ft_printf("exit\n");
	status = exit_status(0, false);
	if (argv[1])
	{
		if (!is_number(argv[1]))
		{
			ft_put_err("numeric argument required", "exit", argv[1]);
			status = 2;
		}
		else if (is_number(argv[1]) && argv[2])
		{
			ft_put_err("too many arguments", "exit", NULL);
			return (1);
		}
		else
			status = ft_atoi(argv[1]) % 256;
	}
	ft_free_eve(step, env);
	rl_clear_history();
	exit(status);
}
