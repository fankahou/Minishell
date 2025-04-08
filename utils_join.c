/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:54:40 by kfan              #+#    #+#             */
/*   Updated: 2025/04/08 18:00:34 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// just copying the char for join_split()
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

// can be used as static?
// joining 2 splited array together
// will free the pointers of both sources and put the str contents
//      in a new malloc container
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
