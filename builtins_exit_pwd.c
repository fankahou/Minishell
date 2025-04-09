/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_pwd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmautner <kmautner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/09 15:55:43 by kmautner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get total digit of str
/**
 * @brief Get the number of digits in a string
 *
 * Count the number of digits in the string representation
 * of a number.
 * The string may begin with an arbitrarry amount of whitespace,
 * followed by a single '+' or '-' symbol, follwoed by digits.
 * The number may be prepended by any amount of zeroes and they
 * will not be counted towards the length of the number.
 *
 * @param nptr string to check
 * @return int
 * @retval digit_cmd Number of digits in the string
 */
static int	pre_check_long_overflow(const char *nptr)
{
	int	i;
	int	digit_cmd;

	i = 0;
	digit_cmd = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i++] == '-')
			digit_cmd++;
	}
	while (nptr[i] == '0')
		i++;
	if (!nptr[i])
		digit_cmd++;
	while (nptr[i] && ft_isdigit(nptr[i++]))
		digit_cmd++;
	return (digit_cmd);
}

/**
 * @brief Check if a long overflowed.
 *
 * Checks if a long overflowed by comparing its digits
 * to a string representation of the wanted number.
 *
 * @param nptr string to compare the number to
 * @param temp number to check for overflows
 * @param digit_cmd number of digits in the string
 * @param digit_temp number of digits in the number
 * @return int
 * @retval success 0 if it did not overflow, 1 otherwise.
 */
static int	check_long_overflow(const char *nptr, long temp, int digit_cmd,
		int digit_temp)
{
	digit_cmd = pre_check_long_overflow(nptr);
	if (temp == LONG_MIN)
		digit_temp = 20;
	else if (temp < 0)
	{
		temp = temp * -1;
		digit_temp++;
	}
	while (temp > 9)
	{
		temp = temp / 10;
		digit_temp++;
	}
	if (digit_cmd == 0)
		digit_cmd = 1;
	if (digit_cmd != digit_temp)
		return (1);
	return (0);
}

/**
 * @brief Evaluates the arguments of the exit command
 * 
 * Checks for errors in the arguments of exit.
 * If more than one argument is given it throws an error.
 * If a non-numeric argument is given it throws an error.
 * If the number is too large (over LONG_MAX) it throws an error.
 *
 * @param cmd command arguments
 * @param token token to change the exit code in
 * @param temp converted number (ignored)
 * @param i character index. Always set to 0!
 * @return int 
 * @retval success Always returns 0
 *
 * @author kfan
 */
static int	exit_arg(char **cmd, t_token *token, long temp, int i)
{
	temp = ft_atoi(cmd[0]);
	if (temp > 255)
		token->exit_code[0] = 255;
	else
		token->exit_code[0] = (int)temp;
	temp = ft_atol(cmd[0]);
	if (check_long_overflow(cmd[0], temp, 0, 1))
		return (token->exit_code[0] = 2,
			perror("minishell: exit: numeric argument required"), 0);
	if (cmd[0][i] == '+' || cmd[0][i] == '-' || is_space(cmd[0][i]))
		i++;
	if (!cmd[0][i])
		return (token->exit_code[0] = 2,
			perror("minishell: exit: numeric argument required"), 0);
	while (cmd[0][i])
	{
		if (cmd[0][i] && !is_space(cmd[0][i]) && !ft_isdigit(cmd[0][i]))
			return (token->exit_code[0] = 2,
				perror("minishell: exit: numeric argument required"), 0);
		i++;
	}
	return (0);
}

// check arg > 2 ; bash: exit: too many arguments
// exit 9223372036854775807 > ok
// exit 9223372036854775808 > fail
// exit -9223372036854775807 > ok but how to make it faster?
/**
 * @brief Builtin "exit" command
 *
 * Handles the bahaviour of the builtin exit command.
 * Quits minishell, taking one optional argument
 * for the exit code.
 *
 * @param cmd command arguments
 * @param token command token
 * @return int
 * @retval success 0 on success, 1 on error
 *
 * @author kfan
 */
int	builtins_exit(char **cmd, t_token *token)
{
	if (cmd[0] && cmd[1] && ft_isdigit(cmd[0][0]))
		return (token->exit_code[0] = 1,
			perror("minishell: exit: too many arguments"), 1);
	else if (token->nmb_of_cmd == 1)
	{
		token->error[0] = 2;
		// ft_printf("exit\n"); // no need to print?
		token->exit_code[0] = 0;
	}
	if (cmd[0])
		exit_arg(cmd, token, 0, 0);
	return (0);
}

/* When run as a built-in command in that shell it was
that shell internally keeping track of the name of
the current directory in a shell/environment variable.
When run as a built-in command in another shell,
it was the other shell failing to match the device
and i-node number of its working directory to the second directory
now named by the contents of the PWD environment variable that it inherited,
thus deciding not to trust the contents of PWD, and then failing in the getcwd()
library function because the working directory does not have any names any longer,
it having been unlinked. */

// need to check!!!!
// mkdir a a/b; cd a/b; rm -rf ../../a; unset PWD; unset OLDPWD; pwd
// why is it still working in bash???

/**
 * @brief Builtin "pwd" command
 *
 * Handles the behaviour of the pwd command,
 * printing the current working directory to
 * stdout.
 *
 * Note: ignores arg?
 *
 * @param envp environment variables
 * @return int
 * @retval success 0 on success, return 0 also when getcwd fails,
	just a bash thing
 *
 * @author kfan
 */
int	builtins_pwd(char **envp)
{
	char	path[1024];

	(void)envp;
	if (!getcwd(path, 1024))
		return (perror("minishell: pwd: getcwd: cannot access parent directories: "),
			0);
	ft_printf("%s\n", path);
	return (0);
}
