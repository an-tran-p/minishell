/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 13:29:09 by atran             #+#    #+#             */
/*   Updated: 2025/02/17 14:53:24 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_fill_line(char *src)
{
	char	*dest;
	size_t	size;
	size_t	i;

	i = 0;
	size = (ft_strchr(src, '\n') - src) + 1;
	dest = malloc((size + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	while (i < size)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_remain(char *s)
{
	char	*dest;
	char	*new_line;

	new_line = ft_strchr(s, '\n') + 1;
	if (*new_line == '\0')
	{
		ft_free_str(&s);
		return (NULL);
	}
	dest = ft_strdup(new_line);
	if (!dest)
	{
		ft_free_str(&s);
		return (NULL);
	}
	ft_free_str(&s);
	return (dest);
}

char	*ft_strjoin_m(char *s1, char *s2)
{
	size_t	s1_l;
	size_t	s2_l;
	char	*dest;

	s1_l = ft_strlen(s1);
	s2_l = ft_strlen(s2);
	dest = malloc((s1_l + s2_l + 1) * sizeof(char));
	if (!dest)
	{
		ft_free_str(&s1);
		return (NULL);
	}
	ft_memcpy(&dest[0], s1, s1_l);
	ft_memcpy(&dest[s1_l], s2, s2_l);
	dest[s1_l + s2_l] = '\0';
	ft_free_str(&s1);
	return (dest);
}

char	*find_line(char **store, char *buffer, char *line, int fd)
{
	int	bytes;

	while (*store)
	{
		if (ft_strchr(*store, '\n') != NULL)
		{
			line = ft_fill_line(*store);
			*store = ft_remain(*store);
			return (ft_free_str(&buffer), line);
		}
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes <= 0 && **store && ft_strchr(*store, '\n') == NULL)
		{
			line = ft_strdup(*store);
			return (ft_free_str(&buffer), ft_free_str(store), line);
		}
		if (bytes > 0)
		{
			buffer[bytes] = '\0';
			*store = ft_strjoin_m(*store, buffer);
		}
		else
			return (ft_free_str(&buffer), ft_free_str(store), NULL);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*store;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc((unsigned int)(BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	if (!store)
		store = ft_strdup("");
	line = NULL;
	line = find_line(&store, buffer, line, fd);
	return (line);
}
