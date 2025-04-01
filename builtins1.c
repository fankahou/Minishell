/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/01 21:38:50 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//echo -nnnnn -nnnnn -nnnna 32
// echo $NOT_A_VAR $NOT_A_VAR $NOT_A_VAR $USER
// echo $USER after export??? export USER=xxx ; echo $USER <<already expanded in parsing...
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
static int echo_flag(char **cmd)
{
	int i;
	int j;
	
	if (cmd[0] && ft_strlen(&cmd[0][0]) > 1 && cmd[0][0] == '-' && cmd[0][1] == 'n')
	{
		i = 0;
		j = 0;
		while ((ft_strlen(&cmd[i][j]) > 1 && cmd[i][j] == '-' && cmd[i][j + 1] == 'n'))
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
	int i;
	int flag;
	
	i = echo_flag(cmd);
	flag = 0;
	if (i > 0)
		flag = 1;
	while (cmd[i])
	{
		if (cmd[i])
			write(1, cmd[i], ft_strlen(cmd[i]));
		i++;
		if (!cmd[i])
			break ;
		// to fix "echo $NOT_A_VAR $NOT_A_VAR $NOT_A_VAR $USER" :???
		//if (cmd[i + 1] && cmd[i][0] != '\0' && cmd[i + 1][0] != '\0')
		//if (cmd[i][0] != '\0')
		write(1, " ", 1);
	}
	if (flag == 0)
		return (write(1, "\n", 1), 0);
	return (0);
}

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
 * @retval success 0 on success, 1 otherwise.
 *
 * @author kfan
 */
int builtins_pwd(char **envp)
{
	char path[1024];
	
	(void)envp;
	if (!getcwd(path, 1024))
		return(perror("minishell: getcwd failed"), 1);
	ft_printf("%s\n", path);
	return (0);
}


int	check_long_overflow(const char *nptr, long temp, int digit_cmd, int	digit_temp)
{
	int		i;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i++] == '-')
			digit_cmd++;
	}
	while (nptr[i] == '0')
		i++;
	while (nptr[i] && ft_isdigit(nptr[i++]))
		digit_cmd++;
	if (temp < 0)
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
int builtins_exit(char **cmd, t_token *token)
{
	int i;
	long temp;

	if (cmd[0] && cmd[1] && ft_isdigit(cmd[0][0]))
		return (token->exit_code[0] = 1, perror("minishell: exit: too many arguments"), 1);
	else if (token->nmb_of_cmd == 1)
	{
		token->error[0] = 2;
		//ft_printf("exit\n"); // no need to print?
		token->exit_code[0] = 0; 
	}
	i = 0;
	if (cmd[0])
	{
		temp = ft_atoi(cmd[0]);
		if (temp > 255)
			token->exit_code[0] = 255;
		else
			token->exit_code[0] = (int)temp;
		temp = ft_atol(cmd[0]);
		if (check_long_overflow(cmd[0], temp, 0, 1))
			return (token->exit_code[0] = 2, perror("minishell: exit: numeric argument required"), 0);
		if (cmd[0][i] == '+' || cmd[0][i] == '-' || is_space(cmd[0][i]))
			i++;
		if (!cmd[0][i])
			return (token->exit_code[0] = 2, perror("minishell: exit: numeric argument required"), 0);
		while(cmd[0][i])
		{
			if (cmd[0][i] && !is_space(cmd[0][i]) && !ft_isdigit(cmd[0][i]) && token->error[0] == 2)
				return (token->exit_code[0] = 2, perror("minishell: exit: numeric argument required"), 0);
			else if (cmd[0][i] && !is_space(cmd[0][i]) && !ft_isdigit(cmd[0][i]))
				return (perror("minishell: exit: numeric argument required"), 0);
			i++;
		}
	}
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
int builtins_env(char **envp)
{
	//"_=./minishell" > "_=env" or "/usr/bin/env"?
	print_array(envp);
	
	return (0); 
}
