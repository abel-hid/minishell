
NAME = minishell

SRCS =	main.c\
		expander.c\
		lexing.c\
		test.c\
		list.c\
		parsing.c\
		execute.c\
		env.c\
		libft.c\
		get_next_line.c\


OBJS = $(SRCS:.c=.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra
READLINE = $(shell brew --prefix readline)

all: $(NAME)

$(NAME):$(OBJS)
	$(CC) $(CFLAGS) -L $(READLINE)/lib $(OBJS) -o $(NAME) -lreadline
%.o: %.c
	$(CC) $(CFLAGS) -I $(READLINE)/include -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
