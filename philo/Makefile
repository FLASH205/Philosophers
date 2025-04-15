NAME = philo

FLAG = -Wextra -Wall -Werror

SRC = \
	ft_utils.c \
	ft_start_simulation.c \
	ft_philo_routine.c \
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