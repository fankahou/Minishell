SRCDIR = .
OBJDIR = obj

SRC = main.c \
	parse1_make_tree.c \
	parse2_init.c \
	parse2a_parenthesis.c \
	parse3_make_cmd_list.c \
	parse4_redir.c \
	parse4a_redir_heredoc.c \
	parse5_clean_and_expand.c \
	parse6_clean_name.c \
	parse6a_clean_name_no_expand.c \
	parse6b_clean_name_join_envp.c \
	parse7_expand_envp.c \
	parse8_wildcards.c \
	pipex.c \
	pipex_input.c \
	pipex_utils.c \
	builtins_cd.c \
	builtins_echo_env.c \
	builtins_exit_pwd.c \
	builtins_export_unset.c \
	builtins_pipes.c \
	error.c \
	error_mini.c \
	utils.c \
	utils_array1.c \
	utils_array2.c \
	utils_free.c \
	utils_is.c \
	utils_join.c \
	utils_ft_split_space.c \
	utils_ft_split_cmd.c \
	utils_ft_split_pipe.c \
	utils_ft_split_delimiter.c \
	utils_count.c \
	utils_history.c \
	history.c \
	signals1.c \
	signals2.c 
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

NAME = minishell
NAME_BONUS = minishell_bonus
SUPRESSION_FILE = readline.supp

CFLAGS = -Wall -Wextra -Werror -g
LIBS = $(SRCDIR)/libft/libft.a -lreadline
RM = rm -f

HOSTNAME = $(shell "hostname")
DEBUG = 0

.DEFAULT_GOAL = all

.PHONY: all bonus clean fclean re vg

all: $(NAME)
bonus: $(NAME_BONUS)

ifeq ($(DEBUG),1)
$(NAME): obj $(OBJ)
	cd $(SRCDIR)/libft && make
	cc $(CFLAGS) $(OBJ) $(LIBS) -D DEBUG=1 -o $(NAME)
	export HOSTNAME=$(HOSTNAME)

$(NAME_BONUS): obj $(OBJ)
	cd $(SRCDIR)/libft && make
	cc $(CFLAGS) $(OBJ) $(LIBS) -D DEBUG=1 -o $(NAME_BONUS)
	export HOSTNAME=$(HOSTNAME)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	cc $(CFLAGS) -c $< -D DEBUG=1 -o $@
else
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
endif

obj:
	mkdir -p $@

vg: | $(SUPRESSION_FILE)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=all --suppressions=readline.supp ./minishell

$(SUPRESSION_FILE):
	printf "{\n\t<readline-stupidities>\n\tMemcheck:Leak\n\t...\n\tfun:readline\n}\n{\n\t<history-stupoidities>\n\tMemcheck:Leak\n\t...\n\tfun:add_history\n}" | cat > readline.supp

clean:
	cd libft && make clean
	$(RM)r obj
	$(RM) $(SUPRESSION_FILE)

fclean:
	cd libft && make fclean
	$(RM)r obj
	$(RM) $(SUPRESSION_FILE)
	$(RM) .minishell_history
	$(RM) $(NAME) $(NAME_BONUS)

re: fclean all
