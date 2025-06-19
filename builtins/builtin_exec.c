/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/19 20:11:33 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtins(char *argv)
{
	if (ft_strncmp(argv, "echo", 5) == 0 || ft_strncmp(argv, "cd", 3) == 0
		|| ft_strncmp(argv, "pwd", 4) == 0 || ft_strncmp(argv, "export", 7) == 0
		|| ft_strncmp(argv, "unset", 6) == 0 || ft_strncmp(argv, "env", 4) == 0
		|| ft_strncmp(argv, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute_builtin_in_child(char **argv, char ***env, t_step *step,
		int pipe_fd)
{
	int	status;

	if (ft_strncmp(argv[0], "echo", 5) == 0)
		status = ft_echo(argv);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		status = ft_pwd(*env);
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		status = ft_cd(argv, *env);
	if (ft_strncmp(argv[0], "export", 7) == 0)
		status = ft_export(argv, env);
	if (ft_strncmp(argv[0], "env", 4) == 0)
		status = ft_env(argv, *env);
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		status = ft_unset(argv, env);
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		status = ft_exit(argv, *env, step);
	ft_free_eve(step, *env);
	if (pipe_fd != -1)
		close(pipe_fd);
	exit(status);
}

int	execute_builtin_in_parent(char **argv, char ***env, t_step *step,
		int pipe_fd)
{
	int	status;

	(void)step;
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		status = ft_echo(argv);
	if (ft_strncmp(argv[0], "pwd", 4) == 0)
		status = ft_pwd(*env);
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		status = ft_cd(argv, *env);
	if (ft_strncmp(argv[0], "export", 7) == 0)
		status = ft_export(argv, env);
	if (ft_strncmp(argv[0], "env", 4) == 0)
		status = ft_env(argv, *env);
	if (ft_strncmp(argv[0], "unset", 6) == 0)
		status = ft_unset(argv, env);
	if (ft_strncmp(argv[0], "exit", 5) == 0)
		status = ft_exit(argv, *env, step);
	if (pipe_fd != -1)
		close(pipe_fd);
	return (status);
}
