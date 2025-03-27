SRCDIR = .
OBJDIR = obj

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
	utils_count.c \
	utils_history.c \
	history.c 
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

NAME = minishell
NAME_BONUS = minishell_bonus
SUPRESSION_FILE = readline.supp

CFLAGS = -Wall -Wextra -Werror -g
LIBS = $(SRCDIR)/libft/libft.a -lreadline
RM = rm -f

HOSTNAME = $(shell "hostname")

.DEFAULT_GOAL = all

.PHONY: all bonus clean fclean re vg

all: $(NAME)
bonus: $(NAME_BONUS)

$(NAME): obj $(OBJ)
	cd $(SRCDIR)/libft && make
	cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)
	export HOSTNAME=$(HOSTNAME)

$(NAME_BONUS): obj $(OBJ)
	cd $(SRCDIR)/libft && make
	cc $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME_BONUS)
	export HOSTNAME=$(HOSTNAME)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	cc $(CFLAGS) -c $< -o $@

obj:
	mkdir -p $@

vg: | $(SUPRESSION_FILE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell

$(SUPRESSION_FILE):
	printf "{\n\t<readline-stupidities>\n\tMemcheck:Leak\n\t...\n\tfun:readline\n}\n{\n\t<history-stupoidities>\n\tMemcheck:Leak\n\t...\n\tfun:add_history\n}" | cat > readline.supp

clean:
	cd src/libft && make clean
	$(RM)r obj
	$(RM) $(SUPRESSION_FILE)

fclean:
	cd src/libft && make fclean
	$(RM)r obj
	$(RM) $(SUPRESSION_FILE)
	$(RM) $(NAME) $(NAME_BONUS)

re: fclean all
