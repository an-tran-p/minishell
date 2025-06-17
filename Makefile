# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: atran <atran@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 17:50:04 by atran             #+#    #+#              #
#    Updated: 2025/06/17 17:55:56 by atran            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS =  main.c signal.c\
		execution/envp.c execution/exec_pipe.c execution/execution.c \
		execution/find_path.c execution/free_mem.c\
		builtins/pwd.c builtins/echo.c builtins/cd.c builtins/exit.c \
		builtins/export.c builtins/env.c builtins/unset.c builtins/builtin_exec.c \
		hdoc_n_redirection/heredoc.c hdoc_n_redirection/redirection.c \
		hdoc_n_redirection/heredoc_expand.c hdoc_n_redirection/open_n_close_hd.c\
		parser/tokenizer.c parser/tokenizer_handle.c parser/tokenizer_util.c \
		parser/parser_err.c parser/parser_synerr.c parser/token_err_chk.c \
		parser/token_flag.c \
		parser/token_env.c parser/token_get_env.c parser/token_env_space.c \
		parser/st_step.c parser/st_lstnew.c \
		parser/lst_util/tk_lst_util_1.c parser/lst_util/tk_lst_util_2.c \
		parser/lst_util/tk_lst_util_3.c

OBJS = $(SRCS:%.c=%.o)

CC = cc

FLAGS = -Wall -Wextra -Werror

RM = rm -rf

all : $(NAME)

$(OBJS): %.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJS)
		make all -C Libft
		cc $(OBJS) -LLibft -lft -lreadline -o $(NAME)

clean :
		$(RM) $(OBJS)
		make clean -C Libft

fclean : clean
		$(RM) $(NAME)
		make fclean -C Libft

re : fclean all

.PHONY: all clean fclean re
