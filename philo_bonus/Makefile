NAME = philo_bonus

FLAG = -Wextra -Wall -Werror# -fsanitize=thread -g

BSRC = \
	ft_start_simulation_bonus.c \
	ft_utils_bonus.c \
	ft_clean_bonus.c

BOBJ = $(BSRC:.c=.o)

all: $(NAME)

$(NAME): philo_bonus.c $(BOBJ)
	cc $(FLAG) $^ -o $@

$(BOBJ): %.o: %.c philo_bonus.h
	cc $(FLAG) -c $< -o $@

clean:
	rm -f $(BOBJ)

fclean : clean
	rm -f $(NAME)

re: fclean all