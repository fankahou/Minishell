/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse6b_clean_name_join_envp.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/24 17:52:07 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	join_envp_str(t_token *token, t_clean *clean, char *str)
{
	int		i;
	char	*temp;
	char	**new;

	i = 0;
	while (clean->temp[i])
		i++;
	if (i == 0 || clean->new_array == 2)
	{
		new = add_array(clean->temp, "", 0);
		if (!new)
			return (perror("add_array failed"), token->error[0] = 1, 1);
		ft_free_split(clean->temp);
		clean->temp = new;
		clean->new_array = 0;
		i++;
	}
	temp = ft_strjoin(clean->temp[i - 1], str);
	if (!temp)
		return (perror("ft_charjoin failed"), token->error[0] = 1, 1);
	free(clean->temp[i - 1]);
	clean->temp[i - 1] = temp;
	return (0);
}

int	join_envp_char(t_token *token, t_clean *clean, char *str)
{
	int		i;
	char	*temp;
	char	**new;

	i = 0;
	while (clean->temp[i])
		i++;
	if (i == 0 || clean->new_array == 2)
	{
		new = add_array(clean->temp, "", 0);
		if (!new)
			return (perror("add_array failed"), token->error[0] = 1, 1);
		ft_free_split(clean->temp);
		clean->temp = new;
		clean->new_array = 0;
		i++;
	}
	temp = ft_charjoin(clean->temp[i - 1], str);
	if (!temp)
		return (perror("ft_charjoin failed"), token->error[0] = 1, 1);
	free(clean->temp[i - 1]);
	clean->temp[i - 1] = temp;
	return (0);
}

// 3rd part of join_envp...
static void	join_envp_2(t_clean *clean, char **temp)
{
	if (clean->temp)
		free(clean->temp);
	clean->temp = copy_array(temp);
}

// 2nd part of join_envp to get clean->temp
// join_split if envp begins with space
// else join previous str for the first splited cmd
// if nth before it just copy itself
static void	join_envp_1(t_clean *clean, char **temp, int j, char *str)
{
	char	**new;

	if (j > 0)
	{
		new = copy_array(temp);
		clean->temp = join_split(&clean->temp, &new, 0, 0);
	}
	else
	{
		j = 0;
		while (clean->temp[j])
			j++;
		if (j > 0)
		{
			str = ft_strjoin(clean->temp[j - 1], temp[0]);
			if (!str)
				return ;
			free(clean->temp[j - 1]);
			clean->temp[j - 1] = str;
			new = remove_array(temp, 0, 0, 0);
			clean->temp = join_split(&clean->temp, &new, 0, 0);
		}
		else
			join_envp_2(clean, temp);
	}
}

// if (i == 0) eg. $NOT_A_VAR
// else eg. export STH="    A   B    "; ls 'HEL    LO'$STH"WORLD"
int	join_envp(t_token *token, t_clean *clean, char **temp, int i)
{
	int	j;

	if (i == 0)
		clean->temp = ft_cmd(clean->new, token, NULL);
	else
	{
		if (!clean->temp && clean->file[0] == '\0')
		{
			clean->temp = malloc(sizeof(char *));
			if (!clean->temp)
				return (perror("malloc failed"), token->error[0] = 1, 1);
			clean->temp[0] = NULL;
		}
		else if (!clean->temp)
			clean->temp = add_array(NULL, clean->file, 0);
		j = 0;
		while (clean->envp_temp[j] && is_space(clean->envp_temp[j]))
			j++;
		join_envp_1(clean, temp, j, NULL);
		if (!clean->temp)
			return (perror("join_envp1 failed"), token->error[0] = 1, 1);
	}
	return (0);
}
