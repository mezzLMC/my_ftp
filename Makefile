##
## EPITECH PROJECT, 2024
## bsFTP
## File description:
## Makefile
##

SRC = $(wildcard src/*.c src/**/*.c)

NAME = myftp

OBJ = $(SRC:.c=.o)

CFLAGS += -Wall -Wextra -I ./include -g


all: $(OBJ)
	gcc -o $(NAME) $(OBJ)

run:
	make all
	$(NAME) 4242

.PHONY: all clean fclean re server run

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
