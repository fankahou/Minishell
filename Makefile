SRC = main.c \
	parse1_make_tree.c \
	parse2_init.c \
	parse3_make_cmd_list.c \
	parse4_redir.c \
	parse5_clean_name.c \
	parse6_expand_envp.c \
	pipex.c \
	pipex_input.c \
	pipex_utils.c \
	error.c \
	utils.c \
	utils_free.c \
	utils_is.c \
	utils_ft_split_cmd.c \
	utils_ft_split_pipe.c \
	utils_ft_split_delimiter.c \
	utils_count.c

OBJ = $(SRC:.c=.o)

NAME = minishell

LIBFT = libft/libft.a

CC = cc

CFLAGS = -Wall -Werror -Wextra -g

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -c

$(LIBFT):
	$(MAKE) -C ./libft

clean:
	rm -f $(OBJ)
	$(MAKE) -C ./libft clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re
