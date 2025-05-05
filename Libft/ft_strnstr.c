/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:45:49 by atran             #+#    #+#             */
/*   Updated: 2024/11/08 17:12:16 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int	n;
	int	cmp;

	if (*little == '\0')
		return ((char *)big);
	n = (int)ft_strlen(little);
	if (((int)len > 0 && (int)(len - n) < 0) || len == 0)
		return (NULL);
	while ((len - n + 1) > 0 && *big != '\0')
	{
		if (*big == *little)
		{
			cmp = ft_strncmp(big, little, n);
			if (cmp == 0)
				return ((char *)big);
		}
		big++;
		len--;
	}
	return (NULL);
}
