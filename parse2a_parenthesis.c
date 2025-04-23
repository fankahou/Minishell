/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2a_parenthesis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/23 17:47:27 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_parenthesis(char *temp)
{
	int	val;
	int	i;
	int	quote;

	quote = 0;
	i = 0;
	val = 0;
	while (temp[i])
	{
		quote = inside_quote(temp[i], quote);
		if (temp[i] == '(' && quote == 0)
			val++;
		if (temp[i] == ')' && quote == 0)
			val--;
		i++;
	}
	return (val);
}

static void	check_parenthesis_error(char **temp, t_token **token, int i, int k)
{
	int	parenthesis;

	parenthesis = update_parenthesis(temp[i]);
	if (token[k]->parenthesis + parenthesis < 0)
	{
		token[k]->parenthesis = parenthesis;
		syntax_error(")", token[k]);
	}
	if (!token[k + 1] && token[k]->parenthesis + parenthesis > 0)
	{
		token[k]->parenthesis = -1;
		syntax_error("unexpected end of file", token[k]);
	}
}

void	init_parenthesis(char **temp, t_token **token, int i, int k)
{
	int	j;
	int	parenthesis;

	parenthesis = 0;
	if (i > 1)
		parenthesis = update_parenthesis(temp[i - 2]);
	else
		parenthesis = update_parenthesis(temp[i]);
	if (k > 0 && token[k - 1]->parenthesis_flag == 1)
		token[k]->parenthesis = token[k - 1]->parenthesis + parenthesis - 1;
	else if (k > 0)
		token[k]->parenthesis = token[k - 1]->parenthesis + parenthesis;
	check_parenthesis_error(temp, token, i, k);
	j = 0;
	while (is_space(temp[i][j]))
		j++;
	if (temp[i][j] == '(')
		token[k]->parenthesis++;
	if (temp[i][j] == '(')
		token[k]->parenthesis_flag = 1;
}

int	copy_bracket(char *temp, t_clean *clean)
{
	if ((temp[clean->count] == '(' || temp[clean->count] == ')')
		&& clean->quote == 0)
		return (0);
	return (1);
}

int	check_parenthesis(t_token **token, int i)
{
	int	flag;
	int	parenthesis;

	flag = i;
	while (token[i] && token[i]->delimiter == 2 && token[i]->exit_code[0] != 0)
		i++;
	while (token[i] && token[i]->delimiter == 3 && token[i]->exit_code[0] == 0)
		i++;
	parenthesis = token[i]->parenthesis;
	if (token[i] && flag != i && token[i]->parenthesis != 0
		&& token[i]->parenthesis_flag != 0)
	{
		while (token[i + 1] && token[i + 1]->parenthesis == parenthesis
			&& token[i + 1]->parenthesis_flag == 0)
			i++;
	}
	if (flag != i)
		i = check_parenthesis(token, i);
	return (i);
}
