/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/05/28 19:43:07 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtins(char *argv)
{
	if (ft_strncmp(argv, "echo", 5) == 0 || ft_strncmp(argv, "cd", 3) == 0
		|| ft_strncmp(argv, "pwd", 4) == 0 || ft_strncmp(argv, "export", 7) == 0
		|| ft_strncmp(argv, "unset", 6) == 0 || ft_strncmp(argv, "env", 4) == 0
		|| ft_strncmp(argv, "exit", 5) == 0)
		return (0);
	return (1);
}

int	execute_builtin(char **argv, char ***env)
{
	int	exit;

	if (ft_strncmp(argv[0], "echo", 5) == 0)
		exit = ft_echo(argv);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		exit = ft_pwd();
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		exit = ft_cd(argv, *env);
	if (ft_strncmp(argv[0], "export", 7) == 0)
		exit = ft_export(argv, env);
	if (ft_strncmp(argv[0], "env", 4) == 0)
		exit = ft_env(argv, *env);
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		exit = ft_unset(argv, env);
	return (exit);
}
