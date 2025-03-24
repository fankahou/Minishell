/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:01:06 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 16:05:06 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <signal.h>

//  delimiter:
//  0 = \0 end
//  1 = ;
//  2 = &&
//  3 = ||
typedef struct s_cmds
{
    char **cmd;
    int redir[2];
    int fd[2];
    char *infile;
    char *outfile;
    //int pid; // to wait for the last input to finish?
}   t_cmds;

// to be used in clean_name() for pasring to get through norm 25 lines;
typedef struct s_clean
{
    int     count;
    int		quote;
    int     space;
    char *envp_temp;
    char *new;
    char *file;
}   t_clean;

//data to be passed by Koloman
typedef struct s_data
{
    char *str;
    int fd_in; // temp for gnl
    int fd_out; // for write in heredoc
    int fd[2];
    int exit_code;
    int error; // will be init by Ka Hou
    char    **envp;
    //pid for waiting?
}   t_data;

typedef struct s_token
{
    t_cmds **cmds;
    int fd_in;
    int fd_out;
    int delimiter;
    int nmb_of_cmd;
    int *exit_code;
    int *error;
    char    **envp;
    t_data *data;
}   t_token;

void print_array(char **temp); // for debugging
void print_token(t_token **token);  // for debugging'

void make_tree(t_data *data);
int init_token(char **temp, t_token **token, t_data *data, int i);
int make_cmd_list(char **temp, t_token *token);
int redir(char *temp, t_token *token, int k);
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

#endif