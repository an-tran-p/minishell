/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:27:33 by atran             #+#    #+#             */
/*   Updated: 2024/11/08 15:20:15 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, char *src, size_t size)
{
	size_t	dst_l;
	size_t	src_l;
	size_t	i;

	i = 0;
	if (!dest && !size)
		return (0);
	dst_l = ft_strlen(dest);
	src_l = ft_strlen(src);
	if (size <= dst_l)
		return (src_l + size);
	else
	{
		while (i < (size - dst_l - 1) && src[i] != '\0')
		{
			dest[dst_l + i] = src[i];
			i++;
		}
		dest[dst_l + i] = '\0';
		return (dst_l + src_l);
	}
}
