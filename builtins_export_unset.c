/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_unset.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endermenskill <endermenskill@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/17 18:02:25 by endermenski      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if prefix == 11, will skip the prefix in envp_export
/**
 * @brief Unsets an environment variable
 *
 * Unsets (deletes) an environment variable from the given token's
 * environment variable list.
 *
 * @author kfan
 * 
 * @param token token to unset the variable in
 * @param temp variable to unset
 * @return int 
 * @retval succes Returns 0 on success, 1 otherwise.
 */
static int	unset_export(t_token *token, char *temp)
{
	int	i;
	int	j;
	int	prefix;

	i = 0;
	prefix = 11;
	while (token->envp_export && token->envp_export[i])
	{
		j = 0;
		while (token->envp_export[i][j + prefix] && token->envp_export[i][j
			+ prefix] != '=')
			j++;
		if (!ft_strncmp(&token->envp_export[i][prefix], temp, j))
		{
			token->envp_export = remove_array(token->envp_export, i, 0, 0);
			if (!token->envp_export)
				return (perror("remove_array failed"), 1);
			ft_free_split(token->data->envp_export);
			token->data->envp_export = token->envp_export;
			break ;
		}
		i++;
	}
	return (0);
}

/**
 * @brief Add a variable to envp.
 *
 * Adds a variable to envp for the export command.
 * If the valie is already present it should be overwritten.
 *
 * @author kfan
 * 
 * @param cmd name of the variable to export
 * @param token token to set environment variables with
 * @return int 
 * @retval success Returns 0 on success, 1 otherwise.
 */
static int	add_envp_export(char *cmd, t_token *token)
{
	char	*temp;
	char	*quote;

	if (unset_export(token, cmd))
		return (1);
	quote = attach_quote(cmd);
	if (!quote)
		return (perror("attach_quote failed"), 1);
	temp = ft_strjoin("declare -x ", quote);
	free(quote);
	if (!temp)
		return (perror("ft_strjoin failed"), 1);
	token->envp_export = add_array(token->envp_export, temp, 0);
	free(temp);
	if (!token->envp_export)
		return (perror("add_array failed"), 1);
	ft_free_split(token->data->envp_export);
	token->data->envp_export = token->envp_export;
	sort_array(token->envp_export);
	return (0);
}

/**
 * @brief Add entry to envp.
 *
 * Adds an environment variable to envp.
 *
 * @author kfan
 *
 * @param cmd value to add
 * @param token Token to set environment variables
 * @param i index 1 (Always set to 0!)
 * @param j index 2 (Always set to 0!)
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
int	add_envp(char *cmd, t_token *token, int i, int j)
{
	while (token->envp && token->envp[i])
	{
		j = 0;
		while (token->envp[i][j] && token->envp[i][j] != '=')
			j++;
		if (!ft_strncmp(token->envp[i], cmd, j) && token->envp[i][j] == '=')
		{
			token->envp = remove_array(token->envp, i, 0, 0);
			if (!token->envp)
				return (perror("remove_array failed"), 1);
			ft_free_split(token->data->envp);
			token->data->envp = token->envp;
			break ;
		}
		i++;
	}
	token->envp = add_array(token->envp, cmd, 0);
	if (!token->envp)
		return (perror("add_array failed"), 1);
	ft_free_split(token->data->envp);
	token->data->envp = token->envp;
	return (add_envp_export(cmd, token));
}

// check nmb of cmds!!!
// check cmd[1], eg. "USER = 123"
// check starting only with alphabats
/**
 * @brief Builtin "export" command
 *
 * Handles the behaviour of the export command.
 * Sets or changes an environment variable to a
 * given value.
 *
 * @author kfan
 *
 * @param cmd command arguments
 * @param token command token
 * @param k mystery integer
 * @param i mystery integer 2 - electric boogaloo
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
int	builtins_export(char **cmd, t_token *token, int k, int i)
{
	if (cmd[0] && cmd[0][0] == '-' && !str_equals(cmd[0], "--"))
		return (write(2, "minishell: export: invalid option\n", 34), 2);
	while (cmd[k])
	{
		i = 0;
		if (!ft_isalpha(cmd[k][i]) && cmd[k][i] != '_')
			return (write(2, "minishell: export: not a valid identifier\n", 42), 1);
		while (cmd[k][i])
		{
			if (!ft_isalpha(cmd[k][i]) && !ft_isalnum(cmd[k][i])
				&& cmd[k][i] != '_' && cmd[k][i] != '=')
				return (write(2, "minishell: export: not a valid identifier\n", 42), 1);
			if (cmd[k][i] == '=')
				break ;
			i++;
		}
		if (cmd[k][i] == '=' && token->nmb_of_cmd == 1)
			add_envp(cmd[k], token, 0, 0);
		else if (token->nmb_of_cmd == 1)
			add_envp_export(cmd[k], token);
		k++;
	}
	if (k == 0)
		print_array(token->envp_export);
	return (0);
}

/**
 * @brief Builtin "unset" command
 *
 * Handles the behaviour of the builtin unset command.
 * Deletes an environment variable from envp.
 *
 * @author kfan
 *
 * @param cmd command arguments
 * @param envp environment variables
 * @param token command token
 * @param k mystery integer, very spooky
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
int	builtins_unset(char **cmd, char **envp, t_token *token, int k)
{
	int	i;

	if (cmd[0] && cmd[0][0] == '-' && !str_equals(cmd[0], "--"))
		return (write(2, "minishell: unset: invalid option\n", 33), 2);
	while (cmd[k] && token->nmb_of_cmd == 1)
	{
		i = -1;
		while (envp && envp[++i])
		{
			if (!ft_strncmp(envp[i], cmd[k], ft_strlen(cmd[k]))
				&& envp[i][ft_strlen(cmd[k])] == '=')
			{
				envp = remove_array(envp, i, 0, 0);
				if (!envp)
					return (perror("remove_array failed"), 1);
				ft_free_split(token->data->envp);
				token->data->envp = envp;
				break ;
			}
		}
		if (envp && unset_export(token, cmd[k]))
			return (1);
		k++;
	}
	return (0);
}
