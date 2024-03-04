##
## EPITECH PROJECT, 2024
## bsFTP
## File description:
## Makefile
##

SRC = $(wildcard src/*.c)

OBJ = $(SRC:.c=.o)

CFLAGS += -Wall -Wextra -I ./include


all: $(OBJ)
	gcc -o server $(OBJ)

run:
	make all
	./server 4242

.PHONY: all clean fclean re server run

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f server

re: fclean all
