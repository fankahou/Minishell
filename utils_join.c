/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/04/10 16:16:01 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Concatenating logic of join_split()
 * 
 * Concatenates to arrays of strings.
 *
 * @param dest Array to concantenate on
 * @param src Array to concatenate with
 * @param temp Destination for resulting array
 * @param strlen Expected length of result
 *
 * @ref join_split
 *
 * @author kfan
 */
static void	join_split2(char **dest, char **src, char **temp, int strlen)
{
	int	k;
	int	i;

	if (!temp)
		return ;
	k = 0;
	while (dest[k])
	{
		temp[k] = dest[k];
		k++;
	}
	i = 0;
	while (k < strlen)
	{
		temp[k] = src[i];
		k++;
		i++;
	}
}

/**
 * @brief Joins to arrays of strings together.
 *
 * Concatenates two string arrays and frees the originals.
 * The result will be allocated with malloc().
 * The main logic for the concatenation is in join_split2().
 * 
 * @param old Array to concatenate on
 * @param cmd Array to concatenate with
 * @param i Counter variable, ALWAYS SET TO 0!
 * @param j Counter variable, ALWAYS SET TO 0!
 * @return char** 
 * @retval new Concatenated array
 *
 * @ref join_split2
 *
 * @author kfan
 */
char	**join_split(char ***old, char ***cmd, int i, int j)
{
	char	**src;
	char	**dest;
	char	**temp;

	dest = *old;
	src = *cmd;
	while (src[i])
		i++;
	while (dest[j])
		j++;
	temp = malloc(sizeof(char *) * (i + j + 1));
	if (!temp)
		perror("malloc failed");
	else
		temp[i + j] = NULL;
	join_split2(dest, src, temp, i + j);
	free(src);
	free(dest);
	*old = NULL;
	*cmd = NULL;
	return (temp);
}

int	join_cmd_1(t_token *token, int i, int j, char *file)
{
	int		k;
	char	**temp;

	// print_array(token->data->cmd_temp);
	// printf("file = %s\n", file);
	free(file);
	temp = NULL;
	k = 0;
	while (token->data->cmd_temp[k])
		k++;
	temp = copy_array(&token->cmds[i]->cmd[j + 1]);
	while (token->cmds[i]->cmd[++j])
		free(token->cmds[i]->cmd[j]);
	if (!temp)
		return (perror("copy_array failed"), token->error[0] = 1, 0);
	token->data->cmd_temp = join_split(&token->data->cmd_temp, &temp, 0, 0);
	token->cmds[i]->cmd = join_split(&token->cmds[i]->cmd,
			&token->data->cmd_temp, 0, 0); // protection??
	return (k - 1);
}

// skip a cmd if !file or copy it
int	join_cmd_2(t_token *token, int i, int j, char *file)
{
	char	**temp;

	ft_free_split(token->data->cmd_temp);
	if (file && file[0] != '\0')
	{
		token->cmds[i]->cmd[j] = file;
		return (0);
	}
	free(file);
	temp = copy_array(&token->cmds[i]->cmd[j + 1]);
	while (token->cmds[i]->cmd[++j])
		free(token->cmds[i]->cmd[j]);
	token->data->cmd_temp = NULL;
	if (!temp)
		return (perror("copy_array failed"), token->error[0] = 1, 0);
	token->cmds[i]->cmd = join_split(&token->cmds[i]->cmd, &temp, 0, 0);
		// protection??
	return (-1);
}
