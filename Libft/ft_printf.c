/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atran <atran@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:11:25 by atran             #+#    #+#             */
/*   Updated: 2025/05/11 20:34:33 by atran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (s[i] != '\0')
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_specifier(va_list args, va_list args_cpy, char c, int count)
{
	if (c == 'c')
		count += ft_putchar((char)va_arg(args, int));
	if (c == 's')
		count += ft_putstr(va_arg(args, char *));
	if (c == 'i' || c == 'd')
		count += ft_putnbr(va_arg(args, int));
	if (c == '%')
		count += ft_putchar('%');
	if (c == 'u')
		count += ft_putnbr(va_arg(args, unsigned int));
	if (c == 'x' || c == 'X')
		count += ft_unsigned_hex(va_arg(args, unsigned int), c);
	if (c == 'p')
	{
		if (va_arg(args_cpy, uint64_t) != 0)
		{
			count += ft_putstr("0x");
			count += ft_unsigned_hex(va_arg(args, uint64_t), c);
		}
		else
			count += ft_putstr("(nil)");
	}
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;
	va_list	args_cpy;

	count = 0;
	va_copy(args, args_cpy);
	va_start(args, str);
	va_start(args_cpy, str);
	while (*str != '\0')
	{
		if (*str == '%')
		{
			str++;
			count = ft_specifier(args, args_cpy, *str, count);
			str++;
		}
		if (*str != '\0' && *str != '%')
		{
			count += ft_putchar(*str);
			str++;
		}
	}
	va_end(args);
	va_end(args_cpy);
	return (count);
}
