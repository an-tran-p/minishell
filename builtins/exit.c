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

void	define_limit(int *sign, unsigned long long *limit, const char *s)
{
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			*sign = -1;
		s++;
	}
	if (*sign == 1)
		*limit = (unsigned long long)LLONG_MAX;
	else
		*limit = (unsigned long long)LLONG_MAX + 1;
}

int	ft_is_overflow(const char *s, long long *num)
{
	int					sign;
	unsigned long long	result;
	unsigned long long	limit;
	unsigned int		digit;

	sign = 1;
	result = 0;
	define_limit(&sign, &limit, s);
	while (*s >= '+' && *s <= '-')
		s++;
	while (*s >= '0' && *s <= '9')
	{
		digit = *s - '0';
		if (result > (limit - digit) / 10)
			return (1);
		result = result * 10 + digit;
		s++;
	}
	if (sign == 1)
		*num = (long long)result;
	else
		*num = -(long long)result;
	return (0);
}

int	ft_exit(char **argv, char **env, t_step *step)
{
	int			status;
	long long	num;

	ft_printf("exit\n");
	status = exit_status(0, false);
	if (argv[1])
	{
		if (!is_number(argv[1]) || ft_is_overflow(argv[1], &num))
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
			status = num % 256;
	}
	ft_free_eve(step, env);
	rl_clear_history();
	exit(status);
}
