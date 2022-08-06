CC = cc

NAME = philo

NAME_MICRO = philo_micro

CFLAGS = -Wall -Werror -Wextra

SRCS = philo.c ft_atoi.c

SRCS_MICRO = philo_micro.c ft_atoi.c

OBJS = $(SRCS:.c=.o)

OBJS_MICRO = $(SRCS_MICRO:.c=.o)

$(NAME): $(OBJS)
	$(CC) -lpthread $(OBJS) -o $(NAME)

$(NAME_MICRO): $(OBJS_MICRO)
	$(CC) -lpthread $(OBJS_MICRO) -o $(NANAME_MICROME)

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

micro: $(SRCS_MICRO)
	$(CC) -lpthread $^ -o $(NAME_MICRO)

re:
	make fclean
	make all

.PHONY: all clean fclean re