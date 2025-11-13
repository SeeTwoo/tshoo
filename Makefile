NAME = tshoo 
CC = cc -Wall -Wextra -Werror -Iincludes
CCDEBUG = $(CC) -g3 -O0

SRC_DIR = src
SRC_FILES = aliasing.c \
			assign_variable.c \
			build_prompt.c \
			cleaning.c \
			environment_manipulation.c \
			exec.c \
			exec_config_file.c \
			expansion.c \
			get_bin_path.c \
			heredoc.c \
			init_env.c \
			is_token_type.c \
			lexer.c \
			logging_and_debugging.c \
			loops.c \
			main.c \
			parsing.c \
			recursive_descent.c \
			set_options.c \
			setup_redirections.c \
			trim_command.c \
			tshoo_alias.c \
			tshoo_cd.c \
			tshoo_echo.c \
			tshoo_env.c \
			tshoo_exit.c \
			tshoo_style.c \
			tshoo_unalias.c \
			tshoo_unset.c

SRC = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = obj
OBJ_FILES = $(SRC_FILES:.c=.o)

OBJ = $(addprefix $(OBJ_DIR)/, $(OBJ_FILES))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ libs/key_value.a libs/tshoo_line.a

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

debug:
	$(MAKE) CC="$(CCDEBUG)" all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test: $(NAME)
	@cd tests && python3 tester.py ../$(NAME)

.PHONY: all debug clean fclean re test
