/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:01:06 by kfan              #+#    #+#             */
/*   Updated: 2025/03/26 15:47:18 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file minshell.h
 * @brief Minishell header file.
 * Contains all the tools and utilities for minishell.
 */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef DEBUG
#  define DEBUG 0
# endif // DEBUG

# include "libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/wait.h>

/**
 * @def HISTORY_MAX_SIZE
 * @brief Max size for history.
 * This macro defines the max size of the history. Default is 100 commands.
 * @author kmautner
 */
# define HISTORY_MAX_SIZE 100
/**
 * @def HISTORY_FILE
 * @brief Location and name of the history file.
 * This macro defines the location and name of the file where the history
 * is saved inbetween sessions.
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
 * @param str (Malloc'd) String input of the user
 * @param fd_in temp for gnl
 * @param fd_out for write in heredoc
 * @param fd File descriptors for stdin and stdout (may not be 0 & 1!)
 * @param exit_code exit code of the program
 * @param error Errno? (will be init by kfan)
 * @param envp Array of environment variables
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
	// pid for waiting?
}				t_data;

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
	t_data		*data;
}				t_token;

/**
 * @struct t_history
 * @brief Struct to save history data.
 * This struct saves all needed data for the history.
 * It's currently just a fancy array, but it may evolve in the future.
 *
 * @param list List of all commands in the history.
 * @param length Amount of commands in history.
 *
 * @author kmautner
 */
typedef struct s_history
{
	char		*list[HISTORY_MAX_SIZE];
	int			length;
}				t_history;

/**
 * @struct t_signal
 * @brief Struct to store a received signal.
 * @deprecated Will be replaced because of subject/norm.
 *
 * @param signal number of the signal
 * @param siginfo info about the signal sender (see siginfo_t)
 * @param context Currently not used for anything.
 * @param check See if the sgnal has been processed.
 *      Set to 0 after processing the signal.
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

void	print_array(char **temp);     // for debugging
void	print_token(t_token **token); // for debugging'

void			make_tree(t_data *data);
int				init_token(char **temp, t_token **token, t_data *data, int i);
int				make_cmd_list(char **temp, t_token *token);
int				redir(char *temp, t_token *token, int k);
char			*clean_name(char *temp, t_token *token, int count, char *file);
char			*expand_envp(char *temp, t_token *token, char *new,
					t_clean *clean);
int				check_envp_count(char *temp);

// error
void			syntax_error(char *str, t_token *token);
void			open_error(char *str, t_token *token);

// from pipex
int				pipex(t_token *token);
// void	ft_free_pipex(char **array);
// void	error_pipex(char *message, char **temp);
char			*find_bin(char **cmd, char **paths, int i);
char			*get_path(char **cmd, char **envp);
int				input(t_cmds *cmds, t_token *token);
int				last_input(t_cmds *cmds, t_token *token);
// void	execve_error(char *message, char **temp, int fd);

// all utils
char			*ft_charjoin(char *s1, char *s2);
int				inside_quote(char c, int quote);
void			close_unused_fd(t_token **token, int i);
void			restore_fd(t_data *data, int *fd);
char			*join_cmd(char *src, char *new);
char			**ft_split_cmd(char *s);
char			**ft_split_delimiter(char const *s);
char			**ft_split_pipe(char *s);
void			ft_free(char **array, int i);
void			ft_free_split(char **array);
void			ft_free_token(t_token **token);
void			small_free(t_token **token, int i);
int				is_delimiter(char c1, char c2);
int				is_sym(char c1, char c2);
int				is_quote(char c);
int				is_space(char c);
int				is_pipe(char c);
int				sym_count(char c1, char c2, char *pin);
int				delimiter_count(char c1, char c2);

// ERROR.C
int				error(char *msg);
void			warn(char *msg);
void			debug(char *msg);

// HISTORY.C
int				history_add(t_history *history, char *command);
int				init_history(t_history *history);
void			destroy_history(t_history *history);

// UTILS_HISTORY.C
int				str_equals(char *str1, char *str2);
void			shift_array(char **array, int length);

#endif