CC = gcc

FLAGS = -Wall -Wextra -Werror

SRC =	file1.c \
	get_next_line.c \

OBJ	= $(SRC:.c=.o)

LIB 	= libft/libft.a

LIBX    = -L/usr/X11/lib -lX11 -lmlx -lXext

NAME = fdf

all:		$(NAME)

$(NAME):	$(OBJ)
	$(MAKE) -C ./libft
	$(CC) -o $(NAME) $(FLAGS) $(OBJ) $(LIB) $(LIBX)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re