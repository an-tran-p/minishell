/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:06:25 by atran             #+#    #+#             */
/*   Updated: 2025/06/25 18:53:56 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_permission(char **cmd, char **env, t_step *step)
{
	struct stat	sb;

	if (stat(cmd[0], &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			ft_put_err("Is a directory", cmd[0], NULL);
			ft_free_eve(step, env);
			exit(126);
		}
		if (access(cmd[0], X_OK) != 0)
		{
			ft_put_err("Permission denied", cmd[0], NULL);
			ft_free_eve(step, env);
			exit(126);
		}
	}
}

void	put_err_execve(char **env, t_step *step)
{
	ft_free_eve(step, env);
	perror("Execution fails");
	if (errno == ENOENT)
		exit(127);
	else if (errno == EACCES)
		exit(126);
	exit(1);
}

void	filename_has_slash(char **cmd, char **env, t_step *step)
{
	if (cmd[0][ft_strlen(cmd[0]) - 1] == '/')
	{
		cmd[0][ft_strlen(cmd[0]) - 1] = 0;
		if (!access(cmd[0], F_OK))
		{
			cmd[0][ft_strlen(cmd[0]) - 1] = '/';
			ft_put_err("Not a directory", cmd[0], NULL);
			ft_free_eve(step, env);
			exit(126);
		}
		cmd[0][ft_strlen(cmd[0]) - 1] = '/';
	}
}

void	execute(char **cmd, char **env, t_step *step)
{
	char	*path;

	if (!cmd || !cmd[0])
	{
		ft_free_eve(step, env);
		exit(0);
	}
	if (!access(cmd[0], F_OK) && ft_strchr(cmd[0], '/'))
	{
		check_permission(cmd, env, step);
		path = cmd[0];
	}
	else
		path = find_path(cmd[0], env);
	if (!path || !path[0])
	{
		filename_has_slash(cmd, env, step);
		ft_put_err("command not found", cmd[0], NULL);
		ft_free_eve(step, env);
		exit(127);
	}
	else if (execve(path, cmd, env) == -1)
		put_err_execve(env, step);
	free(path);
}
