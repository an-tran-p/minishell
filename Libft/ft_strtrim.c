/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:26:22 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:47:38 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_trim_count(char const *s1, char const *set)
{
	size_t	s1_l;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	s1_l = (int)ft_strlen(s1);
	while (ft_strchr(set, s1[i]) != 0 && s1[i] != '\0')
	{
		count++;
		i++;
	}
	while (ft_strchr(set, s1[s1_l]) != 0 && s1[s1_l - 1] != '\0')
	{
		count++;
		s1_l--;
	}
	return (count - 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		s1_l;
	int		t_count;
	int		i;
	char	*dest;

	i = 0;
	s1_l = (int)ft_strlen(s1);
	t_count = ft_trim_count(s1, set);
	if ((s1_l - t_count) <= 0)
		return (ft_strdup(""));
	while (ft_strchr(set, s1[i]) != 0 && s1[i] != '\0')
		i++;
	dest = malloc((s1_l - t_count + 1) * sizeof(char));
	if (!dest)
		return (0);
	else
		dest = ft_memcpy(dest, &s1[i], (s1_l - t_count));
	dest[s1_l - t_count] = '\0';
	return (dest);
}
