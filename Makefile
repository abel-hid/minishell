
NAME = minishell

SRCS =	main.c\
		expander.c\
		expander_utils.c\
		expand_value.c\
		lexing.c\
		quote.c\
		test.c\
		list.c\
		parsing.c\
		parsing_utils.c\
		parsing_error.c\
		parsing_redirction.c\
		parsing_utils2.c\
		parse_args.c\
		execute.c\
		env.c\
		libft.c\
		libft2.c\
		t.c\
		dollar.c\
		exit.c\
		create_lexer.c\
		syntax.c\
		syntax_utils.c\
		cmd.c\
		heredoc.c\
		export.c\
		export2.c\
		export_tools.c\
		ft_error.c\
		update_oradd_env.c\
		cd.c\
		cd2.c\
		echo.c\
		pwd.c\
		ft_env.c\
		unset.c\
		execute_bin.c\
		execute_bin2.c\
		check_builtins.c\
		exec_builtins.c\
		exec_utils.c\
		exec_utils2.c\
		pipe_execution.c\
		execution_utils.c\
		t2.c\
		lexing_utils.c\
		free.c\
		main_helper.c\

OBJS = $(SRCS:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra
READLINE = $(shell brew --prefix readline)

all: $(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) -L $(READLINE)/lib $(OBJS) -o $(NAME) -lreadline 
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -I $(READLINE)/include -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
