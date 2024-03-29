NAME = ft_strace

HEADER = includes/ft_strace.h

SRC = src/main.c \
src/init_syscall_list.c \
src/ft_strace.c \
src/syscall_displaying.c \
src/signal_displaying.c \
src/get_args.c

OBJ = $(SRC:.c=.o)

CC = gcc -g -Wall -Werror -Wextra

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
