# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atran <atran@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 17:50:04 by atran             #+#    #+#              #
#    Updated: 2025/05/11 21:54:16 by atran            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = minishell.c envp.c builtins/pwd.c builtins/echo.c builtins/cd.c \
		builtins/export.c builtins/export_print.c

OBJS = $(SRCS:%.c=%.o)

CC = cc

FLAGS = -Wall -Wextra -Werror

RM = rm -rf

all : $(NAME)

$(OBJS): %.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJS)
		make all -C Libft
		cc $(OBJS) -LLibft -lft -o $(NAME)

clean :
		$(RM) $(OBJS)
		make clean -C Libft

fclean : clean
		$(RM) $(NAME)
		make fclean -C Libft

re : fclean all

.PHONY: all clean fclean re
