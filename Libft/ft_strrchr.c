/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:51:28 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:47:32 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*ptr;
	int			l;
	char		ch;

	ch = (char)c;
	l = ft_strlen(s);
	ptr = s;
	if (ch == '\0')
		return ((char *)ptr + l);
	else
	{
		while (l > 0 && *(ptr + l) != ch)
			l--;
		if (*(ptr + l) == ch)
			return ((char *)ptr + l);
		else
			return (NULL);
	}
}
