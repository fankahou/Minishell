/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse6a_clean_name_no_expand.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/24 19:00:23 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check for ambiguous redirect error in parse4
// only put it here because of space issue
int	check_amb(t_token *token)
{
	int	i;

	if (token->data->cmd_temp)
	{
		i = 0;
		while (token->data->cmd_temp[i])
			i++;
		ft_free_split(token->data->cmd_temp);
		token->data->cmd_temp = NULL;
		if (i > 1)
			return (1);
	}
	return (0);
}

// if the next one is also space it will check if it's the last space
// and return 2 to stop copying that space
// otherwise return 1 for copy the space
int	get_rid_of_extra_space(char *temp)
{
	int	i;

	if (is_space(temp[1]) || temp[1] == '\0')
	{
		i = 0;
		while (is_space(temp[i]))
			i++;
		if (temp[i] == '\0')
			return (2);
	}
	return (1);
}

static int	clean_name_char(char *temp, t_clean *clean)
{
	if (is_space(temp[clean->count]))
		clean->space = clean->space
			+ get_rid_of_extra_space(&temp[clean->count]);
	else
		clean->space = 0;
	if ((clean->space < 2 || clean->quote > 0) && (!is_quote(temp[clean->count])
			|| clean->quote > 2))
		clean->new = ft_charjoin(clean->file, &temp[clean->count]);
	else
		clean->new = ft_strdup(clean->file);
	free(clean->file);
	clean->file = NULL;
	if (!clean->new)
		return (perror("ft_charjoin failed"), -1);
	return (0);
}

void	init_clean(t_clean *clean, int count, char *file)
{
	clean->quote = 0;
	clean->space = 0;
	clean->count = count;
	clean->envp_temp = NULL;
	clean->new = NULL;
	clean->file = file;
	clean->temp = NULL;
	clean->new_array = 0;
}

// clean only quotes and not expand * and $
// only used for eof for here_doc
char	*clean_name_no_expand(char *temp, t_token *token, int count, char *file)
{
	t_clean	clean;
	int		return_val;

	while (is_space(temp[count]))
		count++;
	init_clean(&clean, count, file);
	return_val = 0;
	while (temp[clean.count])
	{
		if (clean.new)
			clean.file = clean.new;
		clean.quote = inside_quote(temp[clean.count], clean.quote);
		return_val = clean_name_char(temp, &clean);
		if (return_val == -1)
			return (NULL);
		clean.count++;
	}
	if (!clean.new && clean.file)
		free(clean.file);
	if (clean.quote > 0)
		return (syntax_error("unexpected EOF while looking for matching quote",
				token), clean.new);
	return (clean.new);
}
