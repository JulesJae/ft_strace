NAME = ft_strace

PATH_SRC = ./srcs/
PATH_OBJ = ./objs/
PATH_INC = ./includes/

INCLUDES = -I $(PATH_INC)

GCC = gcc
FLAGS = -Wall -Wextra -Werror

HEADER = $(shell find $(PATH_INC) -type f -follow -print | grep ".*\.h")
SRC = $(shell find $(PATH_SRC) -type f -follow -print | grep ".*\.c")
OBJ = $(SRC:$(PATH_SRC)%.c=$(PATH_OBJ)%.o)

.PHONY: clean fclean re tester

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
		$(GCC) $(FLAGS) -o $(NAME) $(OBJ) $(LIB)

$(PATH_OBJ)%.o: $(PATH_SRC)%.c 
		mkdir -p $(PATH_OBJ)
		$(GCC) $(FLAGS) -o $@ -c $<

$(PATH_OBJ_D)%.o: $(PATH_SRC_D)%.c 
		mkdir -p $(PATH_OBJ)/discovery
		$(GCC) $(FLAGS) -o $@ -c $<

tester:
	@gcc -o test1 test/test1.c
	@gcc -o test2 test/test2.c
	@gcc -o test3 test/test3.c
	@gcc -o test4 test/test4.c
	@gcc -o test5 test/test5.c -lpthread

clean: 
		rm -rf $(PATH_OBJ) test1 test2 test3 test4 test5

fclean: clean
		rm -f $(NAME)
re: fclean all        