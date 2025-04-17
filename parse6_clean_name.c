/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse6_clean_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: endermenskill <endermenskill@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/04/17 21:52:21 by endermenski      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check if a command is valid after expanding variables.
 *
 * Check if there is valid space with split after envp is expanded.
 * Eg.: export STH="echo 123"; $STH.
 * If i == 1, just dup check[0] replace.
 *
 * @author kfan
 *
 * @param token command token
 * @param clean clean struct
 * @param i counter variable (Always set to 0!)
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
static int	check_split_envp(t_token *token, t_clean *clean, int i)
{
	char	**check;

	check = ft_split_space(clean->envp_temp);
	if (!check)
		return (perror("ft_split_space failed"), 1);
	else
	{
		while (check[i])
			i++;
		check_new_array(clean);
		if ((i == 0 && ((clean->envp_temp[0] != '\0') || clean->quote != 2)
				&& clean->new[0] == '\0') || i > 1 || clean->new_array > 0)
			join_envp(token, clean, check, i);
		else if (i == 1)
		{
			free(clean->new);
			clean->new = ft_strjoin(clean->file, check[0]);
			if (!clean->new)
				return (perror("ft_strjoin failed\n"), 1);
			if (clean->temp && ft_strncmp(token->wildcards, "found", 5))
				join_envp_str(token, clean, check[0]);
		}
		ft_free_split(check);
	}
	return (0);
}

/**
 * @brief Expand envp
 *
 * Check the character count of envp for temp to skip index.
 * Reset space counter to 0.
 * Join the previously processed string with the new, expanded envp.
 * Check envp with ft_split_space if envp has spaces in between entries.
 * If yes and i > 1 it means ther's spaces inbetween and it will
 * allocate clean->temp for later proccessing.
 * -----important: if i = 0 for later wiping out of the current array of str
 * -----important: COnditions: only applies if it's the first char that's
 * valid or not inside "" (double quotes).
 * -----and together if there's sth in the previous process str.
 * If clean->temp exists, it will later in clean_cmd being process with
 * join_cmd and wipe the clean->new created string. 
 *
 * @author kfan
 *
 * @param temp
 * @param token
 * @param clean
 * @return int
 * @retval success Returns 0 on success, -1 on error.
 */
static int	clean_name_envp(char *temp, t_token *token, t_clean *clean)
{
	clean->envp_temp = expand_envp(&temp[clean->count + 1], token, NULL,
			clean->quote);
	if (!clean->envp_temp)
		return (token->error[0] = 1, perror("expand_envp failed"),
			free(clean->file), -1);
	clean->count = clean->count + check_envp_count(&temp[clean->count + 1]);
	if (ft_strlen(clean->envp_temp) > 0)
		clean->space = 0;
	clean->new = ft_strjoin(clean->file, clean->envp_temp);
	if (!clean->new)
		return (free(clean->file), free(clean->envp_temp),
			perror("ft_strjoin failed\n"), -1);
	if (check_split_envp(token, clean, 0))
		return (free(clean->envp_temp), free(clean->file), -1);
	free(clean->envp_temp);
	free(clean->file);
	clean->file = NULL;
	return (0);
}


/**
 * @brief Clean up a command.
 *
 * Clean up a command.
 * New: WIldcards!
 * Wildcards only expand if they're not in a quote (quote == 0).
 * temp[clean->count] = current character.
 * Get rid of excess space to check character by character and only
 * join the new character if it is not an extra space.
 * Filter out the quotes and extra space by strdup.
 * Space inside quotes will not be filtered out.
 * clean->quote indicates the current status of being inside a quote or not.
 * If quote > 0 the current position is inside a quote.
 * If quote > 2 the current position is inside a quote witin a quote,
 * which the quote will be copied. Eg.: "'$USER'" or '"$USER"'.
 * if space < 2
 * wild_switch == 0; jackpot no hit, you only have one chance to verify
 * after you hit it.
 * New: if (clean->envp_temp) join the character at the end of the array.
 *
 * @author kfan
 *
 * @param temp
 * @param token command token
 * @param clean clean struct
 * @return int
 * @retval success Returns 0 on success, -1 on error.
 */
static int	clean_name_char(char *temp, t_token *token, t_clean *clean)
{
	if (is_space(temp[clean->count]))
		clean->space = clean->space
			+ get_rid_of_extra_space(&temp[clean->count]);
	else
		clean->space = 0;
	if (temp[clean->count] == '*' && clean->quote == 0 && !token->wildcards)
	{
		token->wildcards = wildcards(temp, token, clean);
		if (!token->wildcards)
			return (perror("wildcards failed"), -1);
	}
	if ((clean->space < 2 || clean->quote > 0) && (!is_quote(temp[clean->count])
			|| clean->quote > 2))
	{
		clean->new = ft_charjoin(clean->file, &temp[clean->count]);
		if (clean->temp && ft_strncmp(token->wildcards, "found", 5))
			join_envp_char(token, clean, &temp[clean->count]);
	}
	else
		clean->new = ft_strdup(clean->file);
	free(clean->file);
	clean->file = NULL;
	if (!clean->new)
		return (perror("ft_charjoin failed"), -1);
	return (0);
}

/**
 * @brief Clean and expand a string.
 *
 * Loop through the string temp to remove quotes, expand environment variables
 * and expand wildcards.
 *
 * @author kfan
 *
 * @param temp string to expand
 * @param token command token
 * @param clean clean struct
 * @return int
 * @retval success Returns 0 on success, 1 on error.
 */
static int	loop_str(char *temp, t_token *token, t_clean *clean)
{
	int	return_val;

	return_val = 0;
	while (temp[clean->count])
	{
		if (clean->new)
			clean->file = clean->new;
		clean->quote = inside_quote(temp[clean->count], clean->quote);
		if (temp[clean->count] == '$' && clean->quote != 1 && clean->quote != 3)
			return_val = clean_name_envp(temp, token, clean);
		else
			return_val = clean_name_char(temp, token, clean);
		if (return_val == -1)
			return (1);
		clean->count++;
	}
	if (!clean->new && clean->file)
		free(clean->file);
	return (0);
}

/**
 * @brief Expand input.
 *
 * Expand environment variables in temp.
 * Environment variables inside '' (single quotes) should not be expanded,
 * otherwise they should always expand.
 * Any extra space outside of quotes should be removed.
 * count == 0 for cmd, otherwise for redir.
 * Only create new cmd array if token->pipe == 0 ???
 *
 * @author kfan
 *
 * @param temp string to expand
 * @param token command token
 * @param count start index
 * @param file foo
 * @return char*
 * @retval new clean.new on success, NULL otherwise.
 */
char	*clean_name(char *temp, t_token *token, int count, char *file)
{
	t_clean	clean;

	while (is_space(temp[count]))
		count++;
	init_clean(&clean, count, file);
	if (loop_str(temp, token, &clean))
		return (NULL);
	if (clean.quote > 0)
		return (syntax_error("unexpected EOF while looking for matching quote",
				token), clean.new);
	if (clean.temp)
		token->data->cmd_temp = clean.temp;
	if (token->wildcards)
	{
		if (token->wildcards[0] != '*')
		{
			if (clean.new)
				free(clean.new);
			clean.new = ft_strdup(token->wildcards);
			if (!clean.new)
				return (perror("ft_strdup failed"), NULL);
		}
	}
	return (clean.new);
}
