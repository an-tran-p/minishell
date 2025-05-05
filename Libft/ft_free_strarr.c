/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_strarr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:25:19 by atran             #+#    #+#             */
/*   Updated: 2025/02/18 14:49:12 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_strarr(char **str_arr)
{
	int	i;

	i = 0;
	if (str_arr)
	{
		while (str_arr[i])
		{
			ft_free_str(&str_arr[i]);
			i++;
		}
		free(str_arr);
		str_arr = NULL;
	}
}
