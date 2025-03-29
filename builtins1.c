/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/03/29 15:35:34 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//echo -nnnnn -nnnnn -nnnna 32
// echo $NOT_A_VAR $NOT_A_VAR $NOT_A_VAR $USER
// echo $USER after export??? export USER=xxx ; echo $USER <<already expanded in parsing...
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

//ignores arg?
int builtins_pwd(char **envp)
{
	char path[1024];
	
	(void)envp;
	if (!getcwd(path, 1024))
		return(perror("minishell: getcwd failed"), 1);
	ft_printf("%s\n", path);
	return (0);
}


// check arg > 2 ; bash: exit: too many arguments
// exit 9223372036854775807 > ok
// exit 9223372036854775808 > fail
// exit -9223372036854775807 > ok but how to make it faster?
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
		temp = ft_atol(cmd[0]);
		//check long overflow! return (token->exit_code[0] = 2, perror("minishell: exit: numeric argument required"), 0);
/* 		while (temp < 0)
			temp += 256; */
		if (temp > 255)
			token->exit_code[0] = 255;
		else
			token->exit_code[0] = (int)temp;
		if (cmd[0][i] == '+' || cmd[0][i] == '-')
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

// arg?
int builtins_env(char **envp)
{
	//"_=./minishell" > "_=env" or "/usr/bin/env"?
	print_array(envp);
	
	return (0); 
}
