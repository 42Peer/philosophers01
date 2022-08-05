CC = cc

NAME = philo

CFLAGS = -Wall -Werror -Wextra

SRCS = philo.c ft_atoi.c

OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) -lpthread $(OBJS) -o $(NAME)

%.o : %.c
	$(CC) -c $< -o $@

all : $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

debug: all
	$(CC) -lpthread $(OBJS) -o $(NAME) -fsanitize=address

lldb: all
	$(CC) -lpthread $(SRCS) -o $(NAME) -g

re:
	make fclean
	make all

.PHONY: all clean fclean re