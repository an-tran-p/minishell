/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:43:35 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:46:51 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_l;
	size_t	s2_l;
	char	*dest;

	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	dest = malloc((s1_l + s2_l + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	else
	{
		ft_memcpy(&dest[0], s1, s1_l);
		ft_memcpy(&dest[s1_l], s2, s2_l);
		dest[s1_l + s2_l] = '\0';
		return (dest);
	}
}
