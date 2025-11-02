NAME = train_shell 
CC = cc -Wall -Wextra -Werror -Iincludes
CCDEBUG = $(CC) -g3 -O0

SRC_DIR = src
SRC_FILES = builtin_handling.c \
			cleaning.c \
			exec.c \
			get_command_binary_path.c \
			get_paths.c \
			init_env.c \
			lexer.c \
			list_to_array.c \
			main.c \
			main_loop.c \
			parser.c \
			parsing.c \
			syntax.c \
			ts_echo.c \
			ts_exit.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:.c=.o)

OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ ts_readline.a

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

debug: fclean
	$(MAKE) CC="$(CCDEBUG)" all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all debug clean fclean re
