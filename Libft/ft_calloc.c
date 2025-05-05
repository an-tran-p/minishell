/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:50:08 by atran             #+#    #+#             */
/*   Updated: 2024/11/08 14:11:14 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	void	*dest;
	size_t	t_size;

	if (size != 0 && num > (SIZE_MAX / size))
		return (NULL);
	else
	{
		t_size = num * size;
		dest = malloc(t_size);
		if (!dest)
			return (NULL);
		else
		{
			ft_bzero(dest, t_size);
			return (dest);
		}
	}
}
