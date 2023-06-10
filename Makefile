
NAME = minishell

SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

CC = cc
RM = rm -f
CFLAGS =

all: $(NAME)

$(NAME):$(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
