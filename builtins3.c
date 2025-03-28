/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/03/28 12:44:30 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int make_path(char *path, char *prefix, char **temp)
{
	char *new;

	new = ft_calloc(ft_strlen(path) + ft_strlen(prefix) + 1, 1);
	if (!new)
		return (perror("ft_strdup failed"), 1);
	ft_strlcpy(new, prefix, ft_strlen(prefix) + 1);
	ft_strlcpy(&new[ft_strlen(prefix)], path, ft_strlen(path) + 1);
	*temp = new;
	return (0);
}


static int replace_pwd(char *oldpath, t_token *token)
{
	char *pwd;
	char *oldpwd;
	char path[1024];
	
	if (!getcwd(path, 1024))
		return(perror("minishell: getcwd failed"), 1);
	if (make_path(path, "PWD=", &pwd))  // use getcwd(char *buf, size_t size) to get pwd!
		return (1);
	if (make_path(oldpath, "OLDPWD=", &oldpwd)) // use getcwd(char *buf, size_t size) to get oldpwd from parent function!
		return (free(pwd), 1);
	if (add_envp(oldpwd, token, 0, 0))
		return(free(pwd), free(oldpwd), 1);
	if (add_envp(pwd, token, 0, 0))
		return(free(pwd), free(oldpwd), 1);
	free(pwd);
	free(oldpwd);
	return (0);
}

static int check_and_change_path(char **envp, char *path, t_token *token)
{
	int i;
	char oldpath[1024];

	if (path && path[0] == '-' && path[1] == '\0')
	{
		i = 0;
		while (envp[i])
		{
			if (!ft_strncmp(envp[i], "OLDPWD=", 7))
				return (check_and_change_path(envp, &envp[i][7], token));
			i++;
		}
		if (ft_strncmp(envp[i], "OLDPWD=", 7))
			return (perror("minishell: cd: OLDPWD not set"), 1);
	}
	if (!getcwd(oldpath, 1024))
		 return(perror("minishell: getcwd failed"), 1);
	if (chdir(path))
		return (perror("minishell: cd: "), 1);
	return(replace_pwd(oldpath, token));
}

static int change_dir(char *path, char **envp, t_token *token)
{
	int		i;
	
	if (!path)
	{
		i = 0;
		while (envp[i])
		{
			if (!ft_strncmp(envp[i], "HOME=", 5))
				return (check_and_change_path(envp, &envp[i][5], token));
			i++;
		}
		if (ft_strncmp(envp[i], "HOME=", 5))
			return (perror("minishell: cd: HOME not set"), 1);
	}
	else
		return (check_and_change_path(envp, path, token));
	return (0);
}

// check nmb of cmds >> if nmb > 1 it just skip! like in exit()!
// opendir function to check for errors? error after a pipe??
int builtins_cd(char **cmd, char **envp, t_token *token)
{
	if (!envp)
		return(perror("envp not found"), 1);
	if (cmd[0] && cmd[1])
		return(perror("bash: cd: too many arguments"), 1);
	if (cmd[0])
	{
		//if (!opendir())
		//perror("No such file or directory");
		//closedir?
	}
	if (!cmd[0] && token->nmb_of_cmd == 1)
		return (change_dir(NULL, envp, token));
	else if (token->nmb_of_cmd == 1)
		return (change_dir(cmd[0], envp, token));
	return (0);
}
