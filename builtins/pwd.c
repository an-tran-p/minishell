/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:17:21 by atran             #+#    #+#             */
/*   Updated: 2025/06/19 20:10:23 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(char **env)
{
	char	cwd[PATH_MAX];
	char	*path;

	if (getcwd(cwd, PATH_MAX))
		ft_printf("%s\n", cwd);
	else
	{
		path = ft_getenv(env, "PWD");
		ft_printf("%s\n", path);
	}
	return (0);
}
