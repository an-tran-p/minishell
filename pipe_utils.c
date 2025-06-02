/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:25:59 by atran             #+#    #+#             */
/*   Updated: 2025/06/02 23:00:35 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_put_err(char *err_msg, char *para)
{
	write(STDERR_FILENO, err_msg, strlen(err_msg));
	write(STDERR_FILENO, para, strlen(para));
	write(STDERR_FILENO, "\n", 1);
}

void	ft_free_strarr(char **str_arr)
{
	int	i;

	i = 0;
	if (str_arr)
	{
		while (str_arr[i])
		{
			free(str_arr[i]);
			str_arr[i] = NULL;
			i++;
		}
	}
	free(str_arr);
	str_arr = NULL;
}

char	*form_path(char **paths, char *cmd)
{
	int		i;
	char	*top_path;
	char	*path;

	i = 0;
	if (!paths || !cmd)
		return (NULL);
	while (paths[i])
	{
		top_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(top_path, cmd);
		free(top_path);
		if (access(path, F_OK) == 0)
		{
			ft_free_strarr(paths);
			return (path);
		}
		else
			free(path);
		i++;
	}
	ft_free_strarr(paths);
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	path = form_path(paths, cmd);
	return (path);
}
