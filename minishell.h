/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:01:06 by kfan              #+#    #+#             */
/*   Updated: 2025/03/29 12:31:35 by kfan             ###   ########.fr       */
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
 * Indicates if minishell is in debug mode.
 * By default (if not defined) this will be set to 0.
 * Use the debug rule in the make file to compile
 * minishell with debug mode enabled.
 *
 * @author kmautner 
 */
#  define DEBUG 0
# endif // DEBUG

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>
# include <dirent.h>

/**
 * @def HISTORY_MAX_SIZE
 * @brief Max size for history.
 * 
 * This macro defines the max size of the history. Default is 100 commands.
 * 
 * @author kmautner
 */
# define HISTORY_MAX_SIZE 100
/**
 * @def HISTORY_FILE
 * @brief Location and name of the history file.
 * 
 * This macro defines the location and name of the file where the history
 * is saved inbetween sessions.
 * 
 * @author kmautner
 */
# define HISTORY_FILE ".minishell_history"

//  delimiter:
//  0 = \0 end
//  1 = ;
//  2 = &&
//  3 = ||
typedef struct s_cmds
{
	char		**cmd;
	int			redir[2];
	int			fd[2];
	char		*infile;
	char		*outfile;
	// int pid; // to wait for the last input to finish?
}				t_cmds;

// to be used in clean_name() for pasring to get through norm 25 lines;
typedef struct s_clean
{
	int			count;
	int			quote;
	int			space;
	char		*envp_temp;
	char		*new;
	char		*file;
}				t_clean;

/**
 * @struct t_data
 * @brief Input data struct.
 *
 * This struct contains all the necessary data to receive and process
 * user input.
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
 *
 * @author kfan
 */
typedef struct s_data
{
	char		*str;
	int			fd_in;
	int			fd_out;
	int			fd[2];
	int			exit_code;
	int			error;
	char		**envp;
	char    **envp_export; // new!!!!!
	// pid for waiting?
}				t_data;

/**
 * @struct t_token
 * @brief Struct containing a token.
 *
 * This struct contains all data of a single token.
 * I need to get Ka Hou to give a more detailed explanation
 * of what a token is for this description because I
 * don't quite understand his code (yet).
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
 * @var t_token::data
 *		data struct
 *
 * @ref t_data
 *
 * @author kfan
 */
typedef struct s_token
{
	t_cmds		**cmds;
	int			fd_in;
	int			fd_out;
	int			delimiter;
	int			nmb_of_cmd;
	int			*exit_code;
	int			*error;
	char		**envp;
	char    **envp_export; // new!!!!!
	t_data		*data;
}				t_token;

/**
 * @struct t_history
 * @brief Struct to save history data.
 * This struct saves all needed data for the history.
 * It's currently just a fancy array, but it may evolve in the future.
 *
 * @var t_history::list
 *		List of all commands in the history.
 * @var t_history::length
 *		Amount of commands in history.
 * @author kmautner
 */
typedef struct s_history
{
	char		*list[HISTORY_MAX_SIZE];
	int			length;
}				t_history;

/**
 * @deprecated
 * @struct t_signal
 * @brief Struct to store a received signal.
 * @deprecated Will be replaced because of subject/norm.
 *
 * @var t_signal::signal
 *		number of the signal
 * @var t_signal::siginfo
 *		info about the signal sender (see siginfo_t)
 * @var t_signal::context
 *		Not used for anything.
 * @var t_signal::check
 *		See if the sgnal has been processed.
 *		Set to 0 after processing the signal.
 *
 * @author kmautner
 */
typedef struct s_signal
{
	int			signal;
	siginfo_t	*siginfo;
	void		*context;
	char		check;
}				t_signal;

void print_token(t_token **token);  // for debugging'

void make_tree(t_data *data);
int init_token(char **temp, t_token **token, t_data *data, int i);
int make_cmd_list(char **temp, t_token *token);
int redir(char *temp, t_token *token, int k);
int clean_and_expand(t_token *token);
char *clean_name(char *temp, t_token *token, int count, char *file);
char *expand_envp(char *temp, t_token *token, char *new, t_clean *clean);
int check_envp_count(char *temp);

//error
void syntax_error(char *str, t_token *token);
void open_error(char *str, t_token *token);

// from pipex
int     pipex(t_token *token);
//void	ft_free_pipex(char **array);
//void	error_pipex(char *message, char **temp);
char	*find_bin(char **cmd, char **paths, int i);
char	*get_path(char **cmd, char **envp);
int	input(t_cmds *cmds, t_token *token);
int	last_input(t_cmds *cmds, t_token *token);
//void	execve_error(char *message, char **temp, int fd);

//all utils
char	*ft_charjoin(char *s1, char *s2);
int inside_quote(char c, int quote);
void close_unused_fd(t_token **token, int i);
void restore_fd(t_data *data, int *fd);
char *join_cmd(char *src, char *new);
void sort_array(char **input);
char *attach_quote(char *temp);
char	**copy_array_prefix(char **input, int y, int i, int j);
char	**copy_array(char **input);
char	**remove_array(char **input, int index, int y, int j);
char	**add_array(char **input, char *entry, int y);
void print_array(char **temp);
char	**ft_split_space(char *s);
char	**ft_split_cmd(char *s);
char	**ft_split_delimiter(char const *s);
char	**ft_split_pipe(char *s);
void	ft_free(char **array, int i);
void	ft_free_split(char **array);
void	ft_free_token(t_token **token);
void    small_free(t_token **token, int i);
int is_delimiter(char c1, char c2);
int is_sym(char c1, char c2);
int is_quote(char c);
int is_space(char c);
int is_pipe(char c);
int sym_count(char c1, char c2, char *pin);
int delimiter_count(char c1, char c2);
char **join_split(char ***old, char ***cmd, int i, int j);

//builtins
int builtins_echo(char **cmd);
int builtins_cd(char **cmd, char **envp, t_token *token);
int builtins_pwd(char **envp);
int builtins_export(char **cmd, t_token *token, int k, int i);
int builtins_unset(char **cmd, char **envp, t_token *token, int k);
int builtins_env(char **envp);
int builtins_exit(char **cmd, t_token *token);
int add_envp(char *cmd, t_token *token, int i, int j);
int builtins_pipe_fd_out(t_cmds *cmds, int *fd);
int builtins_pipe_fd_in(t_cmds *cmds, int *fd);

// ERROR.C
int				error(char *msg);
void			warn(char *msg);
void			debug(char *msg);

// HISTORY.C
extern int		sigrecv;
int				history_add(t_history *history, char *command);
int				init_history(t_history *history);
void			destroy_history(t_history *history);

// UTILS_HISTORY.C
int				str_equals(char *str1, char *str2);
void			shift_array(char **array, int length);

#endif