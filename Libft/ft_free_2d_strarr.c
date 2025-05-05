/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_2D_strarr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:25:19 by atran             #+#    #+#             */
/*   Updated: 2025/02/20 16:36:53 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_2d_strarr(char ***str_arr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str_arr)
	{
		while (str_arr[i])
		{
			j = 0;
			while (str_arr[i][j])
			{
				ft_free_str(&str_arr[i][j]);
				str_arr[i][j] = NULL;
				j++;
			}
			ft_free_strarr(str_arr[i]);
			i++;
		}
		free(str_arr);
		str_arr = NULL;
	}
}
