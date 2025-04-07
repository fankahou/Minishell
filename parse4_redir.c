/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/07 14:58:33 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc(t_token *token, char *temp, int fd, char *eof)
{
	int	n;

	n = ft_strlen(eof);
	while (1)
	{
		write(token->fd_in, "heredoc> ", 9);
		temp = get_next_line(token->fd_in);
        if (!temp)
            break ;
		if (!ft_strncmp(eof, temp, n))
		{
			if (temp)
				free(temp);
			get_next_line(-1);
			break ;
		}
		write(fd, temp, ft_strlen(temp));
		if (temp)
			free(temp);
	}
}

static int	ft_heredoc(t_token *token, char *eof)
{
	int		fd;
	char	*temp;

	temp = NULL;
	if (access("temp", F_OK) == 0)
		unlink("temp");
	fd = open("temp", O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (-1);
	write_heredoc(token, temp, fd, eof);
	close(fd);
	fd = open("temp", O_RDONLY);
	if (fd < 0)
		return (-1);
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
    new = clean_name(file, token, 0, new);
	if (type == 3)
	{
		token->cmds[k]->redir[0] = 3;
		token->cmds[k]->fd[0] = ft_heredoc(token, new);
		if (token->cmds[k]->fd[0] == -1)
			return (perror("heredoc failed"), -1);
	}
	else
	{
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
