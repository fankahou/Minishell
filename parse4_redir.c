/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/09 18:43:43 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Main heredoc functionality
 * 
 * Handles the bulk of the heredoc funcitonality, allowing
 * the user to write into a temporary file to get more complex
 * input for a command.
 *
 * @param token command token
 * @param temp buffer to read into (unused as argument)
 * @param fd file descriptor of the heredoc file
 * @param eof end of file character(s)
 * @return int 
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
static int	write_heredoc(t_token *token, int fd, char *eof, int quote)
{
	char *temp;
	int	n;
	int	fd_temp;
	int i;
	char *temp_expanded;
	char *prefix;
	char *suffix;

	temp = NULL;
	n = ft_strlen(eof);
	while (1)
	{
		fd_temp = dup(0); // for SIGINT to close(0)
		temp = readline("> ");
		if (!temp)
		{
			token->data->readline_switch = 1;
			signal_init1();
			close(fd_temp);
			write(fd, "\0", 1);
			if (g_sigrecv == 0)
			{
				write(2,
					"minishell: warning: here-document delimited by end-of-file (wanted `eof')\n",
					74);
				return (0);
			}
			else
			{
				token->error[0] = 1; // whole cmd is gone
				token->exit_code[0] = 130;
				dup2(fd_temp, 0); // to restore the STDIN after close in signal file
				g_sigrecv = 0;
			}
			return (1);
		}
		if (!ft_strncmp(eof, temp, n))
		{
			signal_init();
			close(fd_temp);
			if (temp)
				free(temp);
			break ;
		}
		if (quote == 0)
		{
			i = 0;
			while(temp[i])
			{
				if (temp[i] == '$')
				{
					//printf("temp[i + 1] = %s\n", &temp[i + 1]);
					temp_expanded = expand_envp(&temp[i + 1], token, NULL, 0);
					prefix = ft_substr(temp, 0, i); // protection
					i = i + check_envp_count(&temp[i + 1]);
					i++; // for '$'
					suffix = ft_substr(temp, i, ft_strlen(&temp[i]));  // protection
					//printf("prefix = %s\n", prefix);
					//printf("temp_expanded = %s\n", temp_expanded);
					//printf("suffix = %s\n", suffix);
					free(temp);
					temp = ft_strjoin(prefix, temp_expanded);  // protection
					free(prefix);
					free(temp_expanded);
					temp_expanded = ft_strjoin(temp, suffix); // protection
					free(temp);
					free(suffix);
					temp = temp_expanded;
					//printf("temp_expanded = %s---\n", temp_expanded);
				}
				else
					i++;
			}
		}
		write(fd, temp, ft_strlen(temp));
		write(fd, "\n", 1);
		if (temp)
			free(temp);
		close(fd_temp);
	}
	return (0);
}

/**
 * @brief Handles the functionality of a heredoc.
 *
 * Creates a temporary file for the user to write to
 * when using a heredoc and deletes it afterwards.
 * 
 * @param token Command token
 * @param eof End of file indicator
 * @return int 
 * @retval fd file descriptor of the heredoc
 *
 * @author kfan
 */
static int	ft_heredoc(t_token *token, char *eof, char *file)
{
	int	fd;
	int	i;
	
	if (access("temp", F_OK) == 0)
		unlink("temp");
	fd = open("temp", O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (open_error("here_doc", token, NULL), -1);
	signal_init_here_doc(); // new!
	i = 0;
	while (file[i] && is_quote(file[i]))
		i++;
	if (!file[i])
		i = 0;
	if (write_heredoc(token, fd, eof, i))
		return (-1);
	close(fd);
	fd = open("temp", O_RDONLY);
	if (fd < 0)
		return (open_error("here_doc", token, NULL), -1);
	if (access("temp", F_OK) == 0)
		unlink("temp");
	return (fd);
}

static int	ft_redir_in(t_token *token, int type, char *file, int k)
{
	char	*new;

	if (token->cmds[k]->redir[0] > 0 && token->cmds[k]->infile)
	{
		if (token->cmds[k]->redir[0] == 3
			&& token->cmds[k]->fd[0] != token->data->fd[0])
			close(token->cmds[k]->fd[0]);
		free(token->cmds[k]->infile);
	}
	token->cmds[k]->infile = file;
	new = ft_calloc(1, 1);
	if (!new)
		return (perror("ft_calloc failed"), -1);
	if (type == 3)
	{
		new = clean_name_no_expand(file, token, 0, new);
		// new >> name is cleaned differently! no expand of $USER for eof, only quotes and space at the end!
		token->cmds[k]->redir[0] = 3;
		token->cmds[k]->fd[0] = ft_heredoc(token, new, file);
		if (token->cmds[k]->fd[0] == -1)
			token->cmds[k]->fd[1] = -1;//return (perror("heredoc failed"), -1);
	}
	else
	{
		new = clean_name(file, token, 0, new);
		token->cmds[k]->redir[0] = 4;
		token->cmds[k]->fd[0] = open(new, O_RDONLY);
		if (token->cmds[k]->fd[0] == -1)
			open_error(new, token, token->cmds[k]->fd);
		else
			close(token->cmds[k]->fd[0]);
	}
	free(new);
	return (0);
}

// new: create and wipe out file clean??
static void	ft_redir_out(t_token *token, int type, char *file, int k)
{
	int		fd;
	char	*new;

	if (token->cmds[k]->redir[1] > 0 && token->cmds[k]->outfile)
		free(token->cmds[k]->outfile);
	token->cmds[k]->redir[1] = 5;
	if (type == 6)
		token->cmds[k]->redir[1] = 6;
	new = ft_calloc(1, 1);
	if (!new)
		perror("ft_calloc failed");
	else
	{
		new = clean_name(file, token, 0, new);
		token->cmds[k]->outfile = new;
		fd = outfile(token, k);
		if (fd >= 0)
			close(fd);
		free(new);
	}
	token->cmds[k]->outfile = file;
}

int	redir(char *temp, t_token *token, int k)
{
	char	*file;
	int		return_val;
	int		type;
	int		count;

	return_val = 0;
	type = is_sym(temp[0], temp[1]);
	count = sym_count(temp[0], temp[1], NULL);
	// check if empty > syntax error?
	while (temp[count] && is_space(temp[count]))
		count++;
	if (temp[count] == '\0')
		return (syntax_error(temp, token), -1);
	if (token->cmds[k]->fd[0] == -1)
		return (0);
	file = ft_strdup(&temp[count]);
	if (type == 3 || type == 4)
		return_val = ft_redir_in(token, type, file, k);
	else if (type == 5 || type == 6)
		ft_redir_out(token, type, file, k);
	return (return_val);
}
