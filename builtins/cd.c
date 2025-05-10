/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/05/10 13:48:12 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_cd(char **argv, char **env)
{
	char	*path;
	char	old_pwd[PATH_MAX];
	char	cur_pwd[PATH_MAX];

	if (!argv[1])
		path = ft_getenv(env, "HOME");
	else if (ft_strncmp(argv[1], "-", 2) == 0)
		path = ft_getenv(env, "OLDPWD");
	else
		path = argv[1];
	getcwd(old_pwd, PATH_MAX);
	if (chdir(path) != 0)
	{
		ft_printf("cd: %s: ", argv[1]),
		perror("");
		return (1);
	}
	ft_setenv(env, "OLDPWD=", old_pwd);
	getcwd(cur_pwd, PATH_MAX);
	ft_setenv(env, "PWD=", cur_pwd);
	return (0);
}