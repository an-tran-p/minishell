/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/05 20:33:57 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(char *argv)
{
	if (ft_strncmp(argv, "echo", 5) == 0 || ft_strncmp(argv, "cd", 3) == 0
		|| ft_strncmp(argv, "pwd", 4) == 0 || ft_strncmp(argv, "export", 7) == 0
		|| ft_strncmp(argv, "unset", 6) == 0 || ft_strncmp(argv, "env", 4) == 0
		|| ft_strncmp(argv, "exit", 5) == 0)
		return (0);
	return (1);
}

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
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
	}
	if (n == 0)
		ft_printf("\n");
	return (0);
}

int	ft_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, 4096))
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
		return (1);
}

int	execute_builtin(char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		ft_echo(argv);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		ft_pwd();
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc > 0)
		if (is_builtins(argv[1]) == 0)
			execute_builtin(&argv[1]);
	return (0);
}
