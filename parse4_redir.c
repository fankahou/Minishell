/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/17 14:12:33 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 2nd part of ft_redir_in()
// check if it is here_doc or normal infile
// clean the name, no expand for here_doc
// check if file can be opened
static int	ft_redir_in1(t_token *token, int type, char *new, int k)
{
	if (type == 3)
	{
		new = clean_name_no_expand(token->cmds[k]->infile, token, 0, new);
		if (!new)
			return (1);
		token->cmds[k]->redir[0] = 3;
		token->cmds[k]->fd[0] = ft_heredoc(token, new, token->cmds[k]->infile,
				k);
		if (token->cmds[k]->fd[0] == -1)
			token->cmds[k]->fd[1] = -1;
	}
	else
	{
		new = clean_name(token->cmds[k]->infile, token, 0, new);
		if (!new)
			return (1);
		token->cmds[k]->redir[0] = 4;
		token->cmds[k]->fd[0] = open(new, O_RDONLY);
		if (token->cmds[k]->fd[0] == -1)
			open_error(new, token, token->cmds[k]->fd, k);
		else
			close(token->cmds[k]->fd[0]);
	}
	free(new);
	return (0);
}

// free previous filename if a new input file exists withn the same pipe
// and close fd if the previous syntax is here_doc
// calloc and clean the name later, no expand for here_doc
// no expand of $USER for eof, only quotes and space at the end
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
	if (ft_redir_in1(token, type, new, k))
		return (-1);
	return (0);
}

// free previous filename if a new output file exists withn the same pipe
// parsing the type to struct
// calloc and clean the name, then create and wipe out file clean if neccessary
// put the outfile name back to the raw name
// for later processing before pipex (because of split and wildcards!)
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

// check what kind of syntax it is
// check if it is valid (file name followed by redir syntax)
// strdup the raw file name
// put them to process
int	redir(char *temp, t_token *token, int k)
{
	char	*file;
	int		return_val;
	int		type;
	int		count;

	return_val = 0;
	type = is_sym(temp[0], temp[1]);
	count = sym_count(temp[0], temp[1], NULL);
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
