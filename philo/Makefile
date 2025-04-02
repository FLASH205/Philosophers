NAME = philo

FLAG = -Wextra -Wall -Werror -fsanitize=thread -g

SRC = \
	ft_atoi.c \
	ft_current_time.c \
	ft_start_simulation.c \
	ft_clean.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): philo.c $(OBJ)
	cc $(FLAG) $^ -o $@

$(OBJ): %.o: %.c philo.h
	cc $(FLAG) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re: fclean all