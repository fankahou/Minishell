/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/11 14:51:22 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// echo -nnnnn -nnnnn -nnnna 32
// echo $NOT_A_VAR $NOT_A_VAR $NOT_A_VAR $USER
// echo $USER after export??? export USER=xxx ;
// echo $USER <<already expanded in parsing...
/**
 * @brief echo command flag check
 *
 * Checks if the echo command was called with a flag.
 *
 * @param cmd command arguments
 * @return int
 * @retval i Position of the first non-flag argument?
 *
 * @author kfan
 */
static int	echo_flag(char **cmd)
{
	int	i;
	int	j;

	if (cmd[0] && ft_strlen(&cmd[0][0]) > 1 && cmd[0][0] == '-'
		&& cmd[0][1] == 'n')
	{
		i = 0;
		j = 0;
		while ((ft_strlen(&cmd[i][j]) > 1 && cmd[i][j] == '-' && cmd[i][j
				+ 1] == 'n'))
		{
			j++;
			while (cmd[i][j] == 'n' && cmd[i][j] != '\0')
				j++;
			if (cmd[i][j] != '\0')
				return (i);
			i++;
			j = 0;
		}
		return (i);
	}
	return (0);
}

/**
 * @brief Builtin "echo" command
 *
 * This function handles the behaviour of the echo command.
 * Writes a string to stdout that can be piped to other
 * commands.
 *
 * @param cmd command arguments and flags of echo
 * @return int
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
int	builtins_echo(char **cmd)
{
	int	i;
	int	flag;

	i = echo_flag(cmd);
	flag = 0;
	if (i > 0)
		flag = 1;
	while (cmd[i])
	{
		write(1, cmd[i], ft_strlen(cmd[i]));
		i++;
		if (!cmd[i])
			break ;
		write(1, " ", 1);
	}
	if (flag == 0)
		return (write(1, "\n", 1), 0);
	return (0);
}

/**
 * @brief Builtin env command
 *
 * Prints out the list of environment variables.
 *
 * Note: arg?
 *
 * @param envp environment variables
 * @return int
 * @retval return always returns 0
 *
 * @author kfan
 */
int	builtins_env(char **envp)
{
	print_array(envp);
	return (0);
}
