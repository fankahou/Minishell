/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4a_redir_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/12 19:23:13 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// stop heredoc if heredoc readline returns NULL
// which means it receiveed a signal ctrl + d or ctrl + c
static int	stop_heredoc(t_token *token, int fd, int fd_temp)
{
	token->data->readline_switch = 1;
	signal_init1();
	close(fd_temp);
	write(fd, "\0", 1);
	if (g_sigrecv == 0)
	{
		write(2, "minishell: warning: ", 20);
		write(2, "here-document delimited by end-of-file (wanted `eof')\n", 54);
		return (0);
	}
	else
	{
		token->error[0] = 1;
		token->exit_code[0] = 130;
		dup2(fd_temp, 0);
		g_sigrecv = 0;
	}
	return (1);
}

// expand envp inside heredoc
// loop through every char in temp to find $ for expand
static char	*expand_heredoc(char *temp, t_token *token, char *temp_expanded,
		int i)
{
	char	*prefix;
	char	*suffix;

	while (temp[++i])
	{
		if (temp[i] == '$')
		{
			temp_expanded = expand_envp(&temp[i + 1], token, NULL, 0);
			prefix = ft_substr(temp, 0, i);
			i = i + check_envp_count(&temp[i + 1]);
			suffix = ft_substr(temp, i + 1, ft_strlen(&temp[i + 1]));
			free(temp);
			temp = ft_strjoin(prefix, temp_expanded);
			if (!temp)
				return (free_heredoc(suffix, prefix, temp_expanded), NULL);
			free_heredoc(NULL, prefix, temp_expanded);
			temp_expanded = ft_strjoin(temp, suffix);
			if (!temp_expanded)
				return (free_heredoc(suffix, NULL, temp), NULL);
			free_heredoc(suffix, NULL, temp);
			temp = temp_expanded;
		}
	}
	return (temp);
}

// found eof
static void	end_of_file(char *temp, int fd_temp)
{
	signal_init();
	close(fd_temp);
	if (temp)
		free(temp);
}

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
	char	*temp;
	int		fd_temp;

	while (1)
	{
		fd_temp = dup(0);
		if (fd_temp == -1)
			return (perror("dup failed"), 1);
		temp = readline("> ");
		if (!temp)
			return (stop_heredoc(token, fd, fd_temp));
		if (!ft_strncmp(eof, temp, ft_strlen(eof)))
			return (end_of_file(temp, fd_temp), 0);
		if (quote == 0)
			temp = expand_heredoc(temp, token, NULL, -1);
		if (!temp)
			return (1);
		write(fd, temp, ft_strlen(temp));
		write(fd, "\n", 1);
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
int	ft_heredoc(t_token *token, char *eof, char *file, int k)
{
	int	fd;
	int	i;

	if (access("temp", F_OK) == 0)
		unlink("temp");
	fd = open("temp", O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (open_error("here_doc", token, NULL, k), -1);
	signal_init_here_doc();
	i = 0;
	while (file[i] && is_quote(file[i]))
		i++;
	if (!file[i])
		i = 0;
	write_heredoc(token, fd, eof, i);
	close(fd);
	fd = open("temp", O_RDONLY);
	if (access("temp", F_OK) == 0)
		unlink("temp");
	if (fd < 0)
		return (open_error("here_doc", token, NULL, k), -1);
	return (fd);
}
