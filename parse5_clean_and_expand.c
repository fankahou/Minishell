/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse5_clean_and_expand.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/08 13:22:17 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// clean and expand cmd list in a pipe
// join_cmd_1 for envp split > 1 (or wildcards)
// join_cmd_2 for envp i = 0 for wiping out the current array of str and skip
static int	clean_cmd(t_token *token, char *file, int i, int j)
{
	while (token->cmds[i]->cmd && token->cmds[i]->cmd[j])
	{
		file = ft_calloc(1, 1);
		if (!file)
			return (ft_free_split(token->cmds[i]->cmd),
				perror("ft_calloc failed"), token->error[0] = 1, 1);
		file = clean_name(token->cmds[i]->cmd[j], token, 0, file);
		if (!file)
			return (ft_free_split(token->cmds[i]->cmd), token->error[0] = 1, 1);
		free(token->cmds[i]->cmd[j]);
		token->cmds[i]->cmd[j] = NULL;
		if (token->data->cmd_temp && token->data->cmd_temp[0]
			&& token->data->cmd_temp[0][0])
			j = j + join_cmd_1(token, i, j, file);
		else if (token->data->cmd_temp)
			j = j + join_cmd_2(token, i, j, file);
		else
			token->cmds[i]->cmd[j] = file;
		j++;
		if (token->error[0] != 0)
			return (1);
		if (token->wildcards)
			free(token->wildcards);
		token->wildcards = NULL;
	}
	return (0);
}

// clean and expand all cmd list in all the pipes
static int	clean_cmd_list(t_token *token)
{
	int	i;

	i = 0;
	while (token->cmds && token->cmds[i])
	{
		if (clean_cmd(token, NULL, i, 0))
			return (1);
		i++;
	}
	return (0);
}

static int	clean_outfile(t_token *token)
{
	char	*file;
	int		i;

	i = 0;
	while (token->cmds && token->cmds[i])
	{
		if (token->cmds[i]->outfile)
		{
			file = ft_calloc(1, 1);
			if (!file)
				return (ft_printf("ft_calloc failed\n"), 1);
			file = clean_name(token->cmds[i]->outfile, token, 0, file);
			if (!file)
				return (syntax_error(token->cmds[i]->outfile, token), 1);
			if (file[0] == '\0')
				return (free(file), mini_error("ambiguous redirect",
						token->cmds[i]->outfile, token, token->cmds[i]->fd), 0);
			free(token->cmds[i]->outfile);
			token->cmds[i]->outfile = file;
			if (token->wildcards)
				free(token->wildcards);
			token->wildcards = NULL;
		}
		i++;
	}
	return (0);
}

static int	clean_infile(t_token *token)
{
	char	*file;
	int		i;

	i = 0;
	while (token->cmds && token->cmds[i])
	{
		if (token->cmds[i]->infile)
		{
			file = ft_calloc(1, 1);
			if (!file)
				return (ft_printf("ft_calloc failed\n"), 1);
			file = clean_name(token->cmds[i]->infile, token, 0, file);
			if (!file)
				return (syntax_error(token->cmds[i]->infile, token), 1);
			if (file[0] == '\0')
				return (free(file), mini_error("ambiguous redirect",
						token->cmds[i]->infile, token, token->cmds[i]->fd), 0);
			free(token->cmds[i]->infile);
			token->cmds[i]->infile = file;
			if (token->wildcards)
				free(token->wildcards);
			token->wildcards = NULL;
		}
		i++;
	}
	return (0);
}

int	clean_and_expand(t_token *token)
{
	if (clean_outfile(token))
		return (1);
	if (clean_infile(token))
		return (1);
	if (clean_cmd_list(token))
		return (1);
	return (0);
}
