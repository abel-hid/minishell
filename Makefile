
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
