/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:44:19 by atran             #+#    #+#             */
/*   Updated: 2024/11/08 14:11:18 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	digit_count(int n)
{
	int	i;

	i = 1;
	if (n < 0)
		i++;
	while (n / 10 != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char	*str_fill(char *dest, long int num, int size)
{
	dest[size] = '\0';
	if (num == 0)
		dest[0] = '0';
	if (num < 0)
	{
		num = -num;
		dest[0] = '-';
	}
	while (num > 0 && size >= 0)
	{
		dest[size - 1] = ((num % 10) + '0');
		num = num / 10;
		size--;
	}
	return (dest);
}

char	*ft_itoa(int n)
{
	char		*dest;
	long int	num;
	int			digit;

	digit = digit_count(n);
	dest = malloc((digit + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	else
	{
		num = (long)n;
		dest = str_fill(dest, num, digit);
		return (dest);
	}
}
