/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3_make_cmd_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/11 15:07:38 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Splits the command and separates them with spaces.
 *
 * Split the temp cmd with a modified split to seperate the cmd and arg
 * with space.
 * Space inside a quote will not be skipped!
 * Quotes and $ are still inside yet and will be cleaned and expanded later.
 * Join the new split with previous split.
 *
 * Example:
 * "<infile echo      >out 'hel   lo' world$USER" will become:
 * - echo
 * - 'hel   lo'
 * - world$USER
 *
 * @param temp command to split
 * @param token token
 * @param old old split
 * @return char**
 * @retval cmd old or NULL on error
 *
 * @author kfan
 */
char	**ft_cmd(char *temp, t_token *token, char **old)
{
	char	**cmd;

	cmd = ft_split_space(temp);
	if (!cmd)
		return (perror("ft_split failed"), token->error[0] = 1, NULL);
	if (old)
		old = join_split(&old, &cmd, 0, 0);
	else
		old = cmd;
	if (!old)
		return (token->error[0] = 1, NULL);
	return (old);
}

// temp is the new splited cmd list with redir syntax in between
// scan temp and parse info into struct
// redir if the array starts with a redir syntax
// ft_cmd if it is something else
// if it's only a valid syntax inside a pipe eg. ">out | echo 123",
//	just return 0
// free and replace token->cmds[k]->cmd with the parsed **cmd
/**
 * @brief Scans the command for pipes
 *
 * Scans the command array in temp for pipes. If the array
 * starts with a redirection syntax, a redirection is done.
 * Otherwise it will go through ft_cmd(). If the syntax is only
 * valid with the context of a pipe it just returns 0.
 * Frees and replaces token->cmds[k]->cmd with the parsed **cmd.
 *
 * @param temp cmd list with redir syntax
 * @param token struct to parse command into
 * @param k mystery integer
 * @param cmd parsed command
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
static int	scan_pipe(char **temp, t_token *token, int k, char **cmd)
{
	int	i;
	int	j;

	i = -1;
	while (temp[++i])
	{
		j = 0;
		while (is_space(temp[i][j]))
			j++;
		if (temp[i][j] && is_sym(temp[i][j], temp[i][j + 1]) > 2)
			j = redir(&temp[i][j], token, k);
		else if (temp[i][j])
		{
			cmd = ft_cmd(&temp[i][j], token, cmd);
			if (!cmd)
				return (token->error[0] = 1, 1);
		}
		if (j == -1)
			return (token->error[0] = 1, 1);
	}
	ft_free_split(token->cmds[k]->cmd);
	token->cmds[k]->cmd = cmd;
	return (0);
}

/**
 * @brief Make a command list of all split pipes.
 *
 * This functions makes a command list of all split pipes.
 * It will skip an array if the array is a pipe '|'.
 * The commands are also split by redirection operators
 * (<, <<, >>, >).
 * Also takes one array.
 *
 * @param token token to split the commands of
 * @param temp mystery array
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
static int	split_cmd(t_token *token, char **temp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (temp[i])
	{
		if (!is_pipe(temp[i][0]))
		{
			token->cmds[j]->cmd = ft_split_cmd(temp[i]);
			if (!token->cmds[j]->cmd)
				return (token->error[0] = 1, perror("ft_split_cmd failed"), 1);
			j++;
		}
		i++;
	}
	return (0);
}

// make cmd list of all splited pipes
// split cmds by redir syntax < << >> >
// the syntax also takes one array
// scan all pipes
/**
 * @brief Makes a command list of all split pipes.
 *
 * Makes a command list of all split pipes.
 * It splits the commands by redirection operators
 * (<, <<, >>, >) and scans all pipes.
 * Also takes one array.
 *
 * @param temp mystery array
 * @param token token to split the commands of
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
int	make_cmd_list(char **temp, t_token *token)
{
	int	k;

	k = 0;
	if (split_cmd(token, temp))
		return (1);
	while (token->cmds[k])
	{
		if (scan_pipe(token->cmds[k]->cmd, token, k, NULL))
			return (1);
		k++;
	}
	return (0);
}
