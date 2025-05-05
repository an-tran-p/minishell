/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:15:55 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:46:07 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	void	*str;

	str = s;
	while (n > 0)
	{
		*(unsigned char *)s = (unsigned char)c;
		s++;
		n--;
	}
	return (str);
}
