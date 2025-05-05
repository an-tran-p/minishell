/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 14:10:35 by atran             #+#    #+#             */
/*   Updated: 2024/11/07 14:46:29 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_word_count(char const *s, char c)
{
	int	count;
	int	word;

	count = 0;
	word = 0;
	while (*s != '\0')
	{
		if (*s != c && word == 0)
		{
			count++;
			word = 1;
		}
		if (*s == c && word == 1)
			word = 0;
		s++;
	}
	return (count);
}

char	**ft_free(char **ar, int count)
{
	int	i;

	i = 0;
	while (i <= count)
	{
		free(ar[i]);
		i ++;
	}
	free (ar);
	return (0);
}

int	ft_substr_len(char const *s, char c)
{
	int	i;

	i = 0;
	if (*s == c)
		s++;
	while (s[i] != '\0' && s[i] != c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	int		str_num;
	int		i;
	int		substr_len;

	i = 0;
	str_num = ft_word_count(s, c);
	dest = malloc((str_num + 1) * sizeof(char *));
	if (!dest)
		return (0);
	while (i < str_num)
	{
		while (*s == c)
			s++;
		substr_len = ft_substr_len(s, c);
		dest[i] = malloc((substr_len + 1) * sizeof(char));
		if (!dest[i])
			return (ft_free(dest, str_num));
		else
			ft_strlcpy(dest[i], (char *)s, substr_len + 1);
		s = s + substr_len + 1;
		i++;
	}
	dest[str_num] = NULL;
	return (dest);
}
