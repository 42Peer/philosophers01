CC = cc

NAME = philo

CFLAGS = -Wall -Werror -Wextra -lpthread

SRCS = *.c \
		phlio.c
	  
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

all : $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)


re:
	make fclean
	make all

.PHONY: all clean fclean re