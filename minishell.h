/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:01:06 by kfan              #+#    #+#             */
/*   Updated: 2025/04/23 16:30:02 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minshell.h
 * @brief Minishell header file.
 *
 * Contains all the tools and utilities for minishell.
 */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef DEBUG
/**
 * @def DEBUG
 * @brief Debug status
 *
 * @author kmautner
 *
 * Indicates if minishell is in debug mode.
 * By default (if not defined) this will be set to 0.
 * Use the debug rule in the make file to compile
 * minishell with debug mode enabled.
 */
#  define DEBUG 0
# endif // DEBUG

# include "libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/wait.h>

/**
 * @def HISTORY_MAX_SIZE
 * @brief Max size for history.
 *
 * @author kmautner
 *
 * This macro defines the max size of the history. Default is 100 commands.
 */
# define HISTORY_MAX_SIZE 100
/**
 * @def HISTORY_FILE
 * @brief Location and name of the history file.
 *
 * @author kmautner
 *
 * This macro defines the location and name of the file where the history
 * is saved inbetween sessions.
 * Default is ".minishell_history"
 */
# define HISTORY_FILE ".minishell_history"

//  delimiter:
//  0 = \0 end
//  1 = ;
//  2 = &&
//  3 = ||
typedef struct s_cmds
{
	char	**cmd;
	int		redir[2];
	int		fd[2];
	char	*infile;
	char	*outfile;
	int		pid; // to wait for the last input to finish?
	int		exit_code; // new: everything pipe has it's own exit code
}			t_cmds;

// to be used in clean_name() for pasring to get through norm 25 lines;
typedef struct s_clean
{
	int		count;
	int		quote;
	int		space;
	char	*envp_temp;
	// char		*wildcards;
	char	*new;
	char	*file;
	char	**temp; // to check split_space() for envp
	int		new_array; // check if needed to open a new array for charjoin
}			t_clean;

/**
 * @struct t_data
 * @brief Input data struct.
 *
 * This struct contains all the necessary data to receive and process
 * user input.
 *
 * @author kfan
 *
 * @var t_data::str
 *		(Malloc'd) String input of the user
 * @var t_data::fd_in
 *		temp for gnl
 * @var t_data::fd_out
 *		for write in heredoc
 * @var t_data::fd
 *		File descriptors for stdin and stdout (may not be 0 & 1!)
 * @var t_data::exit_code
 *		exit code of the program
 * @var t_data::error
 *		Errno? (will be init by kfan)
 * @var t_data::envp
 *		Array of environment variables
 */
typedef struct s_data
{
	char	*str;
	int		fd_in;
	int		fd_out;
	int		fd[2];
	int		exit_code;
	int		error;
	char	**envp;
	char	**envp_export; // new!!!!!
	char	**cmd_temp; // new!! for split_space envp
	int		readline_switch;
}			t_data;

/**
 * @struct t_token
 * @brief Struct containing a token.
 *
 * This struct contains all data of a single token.
 * I need to get Ka Hou to give a more detailed explanation
 * of what a token is for this description because I
 * don't quite understand his code (yet).
 *
 * @author kfan
 *
 * @var t_token::cmds
 *		(unknown)
 * @var t_token::fd_in
 *		File descriptor for STDIN
 * @var t_token::fd_out
 *		File descriptor for STDOUT
 * @var t_token::delimiter
 *		Delimiter after this token
 * @var t_token::nmb_of_cmd
 *		(unknown)
 * @var t_token::exit_code
 *		exit code of the command in this token
 * @var t_token::error
 *		Error status (1 if an error occurred, 0 otherwise)
 * @var t_token::envp
 *		environment variables
 * @var t_token::envp_export
 *		exported envp?
 * @var t_token::wildcards
 *		no clue what this is...
 * @var t_token::data
 *		data struct
 */
typedef struct s_token
{
	t_cmds	**cmds;
	int		fd_in;
	int		fd_out;
	int		delimiter;
	int		nmb_of_cmd;
	int		parenthesis;
	int		parenthesis_flag;
	int		*exit_code;
	int		*error;
	char	**envp;
	char	**envp_export; // new!!!!!
	char	*wildcards; // new!!
	t_data	*data;
}			t_token;

/**
 * @struct t_history
 * @brief Struct to save history data.
 *
 * This struct saves all needed data for the history.
 * It's currently just a fancy array, but it may evolve in the future.
 *
 * @author kmautner
 * 
 * @var t_history::list
 *		List of all commands in the history.
 * @var t_history::length
 *		Amount of commands in history.
 * @var t_history::location
		Location of the history file.
 */
typedef struct s_history
{
	char	*list[HISTORY_MAX_SIZE];
	int		length;
	char	*location;
}			t_history;

void	print_token(t_token **token); // for debugging'

void	make_tree(t_data *data);
int		init_token(char **temp, t_token **token, t_data *data, int i);
void	init_parenthesis(char **temp, t_token **token, int i, int k);
int		copy_bracket(char *temp, t_clean *clean);
int		check_parenthesis(t_token **token, int i);
int		make_cmd_list(char **temp, t_token *token);
char	**ft_cmd(char *temp, t_token *token, char **old);
int		redir(char *temp, t_token *token, int k);
int		ft_heredoc(t_token *token, char *eof, char *file, int k);
int		clean_and_expand(t_token *token);
char	*clean_name(char *temp, t_token *token, int count, char *file);
int		get_rid_of_extra_space(char *temp);
void	init_clean(t_clean *clean, int count, char *file);
char	*clean_name_no_expand(char *temp, t_token *token, int count,
			char *file);
int		join_envp_char(t_token *token, t_clean *clean, char *str);
int		join_envp_str(t_token *token, t_clean *clean, char *str);
int		join_envp(t_token *token, t_clean *clean, char **temp, int i);
void	check_new_array(t_clean *clean);
char	*expand_envp(char *temp, t_token *token, char *new, int quote);
int		check_envp_count(char *temp);
char	*wildcards(char *temp, t_token *token, t_clean *clean);

// error
void	syntax_error(char *str, t_token *token);
void	open_error(char *str, t_token *token, int *fd, int k);
void	mini_error(char *str, char *arg, t_token *token, int *fd);
void	execve_error(char *cmd, char *path, t_token *token, DIR *dir);

// from pipex
int		pipex(t_token *token);
int		outfile(t_token *token, int k);
char	*get_path(char **cmd, char **envp);
int		input(t_cmds *cmds, t_token *token);
int		last_input(t_cmds *cmds, t_token *token);
int		empty_pipe(int *fd);
void	wait_pipes(t_token *token, int i, int status); // new: for execve()

// all utils
char	*ft_charjoin(char *s1, char *s2);
int		inside_quote(char c, int quote);
void	close_unused_fd(t_token **token, int i);
void	restore_fd(t_data *data, int *fd);
char	*join_cmd(char *src, char *new);
int		join_cmd_1(t_token *token, int i, int j, char *file);
int		join_cmd_2(t_token *token, int i, int j, char *file);
void	sort_array(char **input);
void	sort_array_wildcards(char **input); // for bonus
char	*attach_quote(char *temp);
char	**copy_array_prefix(char **input, int y, int i, int j);
char	**copy_array(char **input);
char	**remove_array(char **input, int index, int y, int j);
char	**add_array(char **input, char *entry, int y);
void	print_array(char **temp);
char	**ft_split_space(char *s);
char	**ft_split_cmd(char *s);
char	**ft_split_delimiter(char const *s);
char	**ft_split_pipe(char *s);
void	ft_free(char **array, int i);
void	ft_free_split(char **array);
void	ft_free_token(t_token **token);
void	free_heredoc(char *suffix, char *prefix, char *temp);
void	small_free(t_token **token, int i);
int		is_delimiter(char c1, char c2);
int		is_sym(char c1, char c2);
int		is_quote(char c);
int		is_space(char c);
int		is_pipe(char c);
int		sym_count(char c1, char c2, char *pin);
int		delimiter_count(char c1, char c2);
char	**join_split(char ***old, char ***cmd, int i, int j);

// builtins
int		builtins_echo(char **cmd);
int		builtins_cd(char **cmd, char **envp, t_token *token);
int		builtins_pwd(char **cmd);
int		builtins_export(char **cmd, t_token *token, int k, int i);
int		builtins_unset(char **cmd, char **envp, t_token *token, int k);
int		builtins_env(char **envp, char **cmd);
int		builtins_exit(char **cmd, t_token *token);
int		add_envp(char *cmd, t_token *token, int i, int j);
int		builtins_pipe_fd_out(t_cmds *cmds, int *fd);
int		builtins_pipe_fd_in(t_cmds *cmds, int *fd);

// ERROR.C
int		error(char *msg);
int		error2(char *msg, char *msg2);
void	warn(char *msg);
void	debug(char *msg);

// SIGNALS.C
extern int	g_sigrecv;
int		signal_init(void);
int		signal_init1(void);
int		signal_init_here_doc(void);
int		signal_init_execve(void);
int		signal_init_post_execve(void);

// HISTORY.C
int		history_add(t_history *history, char *command);
int		init_history(t_history *history);
void	destroy_history(t_history *history);

// UTILS_HISTORY.C
int		str_equals(char *str1, char *str2);
void	shift_array(char **array, int length);
char	*get_history_path(void);

#endif