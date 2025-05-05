/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:51:28 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:46:34 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	const char	*ptr;
	char		ch;

	ch = (char)c;
	ptr = s;
	while (*ptr != '\0' && *ptr != ch)
		ptr++;
	if (*ptr == ch || ch == '\0')
		return ((char *)ptr);
	else
		return (0);
}
