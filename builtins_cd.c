/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:25:47 by kfan              #+#    #+#             */
/*   Updated: 2025/04/23 14:10:50 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// works like strjoin but will update the temp after joining by dereferencing
/**
 * @brief Joins two strings to a path
 *
 * Joins two paths using strjoin and updates the string
 * inside of temp accordingly (*temp == result).
 *
 * @author kfan
 *
 * @param path path to contatenate with
 * @param prefix Prefix to concatenate the path on
 * @param temp Adress to write the result to
 * @return int
 * @retval succes Returns 0 on success, 1 otherwise.
 */
static int	make_path(char *path, char *prefix, char **temp)
{
	char	*new;

	new = ft_calloc(ft_strlen(path) + ft_strlen(prefix) + 1, 1);
	if (!new)
		return (perror("ft_strdup failed"), 1);
	ft_strlcpy(new, prefix, ft_strlen(prefix) + 1);
	ft_strlcpy(&new[ft_strlen(prefix)], path, ft_strlen(path) + 1);
	*temp = new;
	return (0);
}

/**
 * @brief Replace PWD with a new path
 *
 * Updates the PWD environmental variable with the current
 * working directory.
 * Fetches the cwd using getcwd() and copies the old working
 * directory to the OLDPWD envronment variable.
 *
 * The internal variable path is limited to 4096 characters
 * since that is the maximum length of a path in linux.
 *
 * @author kfan
 *
 * @param oldpath old working directory
 * @param token Token to export the variables with
 * @return int
 * @retval success Returns 0 on success, 1 otherwise
 */
static int	replace_pwd(char *oldpath, t_token *token)
{
	char	*pwd;
	char	*oldpwd;
	char	path[4096];

	if (!getcwd(path, 4096))
		return (perror("minishell: getcwd failed"), 1);
	if (make_path(path, "PWD=", &pwd))
		return (1);
	if (make_path(oldpath, "OLDPWD=", &oldpwd))
		return (free(pwd), 1);
	if (add_envp(oldpwd, token, 0, 0))
		return (free(pwd), free(oldpwd), 1);
	if (add_envp(pwd, token, 0, 0))
		return (free(pwd), free(oldpwd), 1);
	free(pwd);
	free(oldpwd);
	return (0);
}

/**
 * @brief Changes the current working directory.
 *
 * Changes the path of the current working directory, as long
 * as the path is a valid directory.
 * If the path is "-" it changes the current working directory to OLDPWD.
 * Caches the current working directory before changing it and calling
 * replace_pwd() to update the environment variables.
 *
 * @author kfan
 *
 * @param envp environment variables
 * @param path path to the new working directory
 * @param token Token to change the variables with
 * @return int
 * @retval succes Returns 0 on success, 1 otherwise.
 */
static int	check_and_change_path(char **envp, char *path, t_token *token)
{
	int		i;
	char	oldpath[4096];

	if (path && str_equals(path, "-"))
	{
		i = 0;
		while (envp && envp[i])
		{
			if (!ft_strncmp(envp[i], "OLDPWD=", 7))
				return (ft_printf("%s\n", &envp[i][7]),
						check_and_change_path(envp, &envp[i][7], token));
			i++;
		}
		return (write(2, "minishell: cd: OLDPWD not set\n", 30), 1);
	}
	if (!getcwd(oldpath, 4096))
		return (perror("minishell: cd: cannot access parent directories: "), 0);
	if (chdir(path))
		return (perror("minishell: cd: "), 1);
	return (replace_pwd(oldpath, token));
}

/**
 * @brief Changes the current working directory.
 *
 * Checks if path is empty. If so, changes the current working directory
 * to the home directory. Otherwise it changes the directory to
 * path using check_and_change_path().
 *
 * @author kfan
 *
 * @param path path to new working directory
 * @param envp environment variables
 * @param token token to change the variables with
 * @return int
 * @retval success Returns 0 on success, 1 otherwise.
 */
static int	change_dir(char *path, char **envp, t_token *token)
{
	int	i;

	if (!path)
	{
		i = 0;
		while (envp && envp[i])
		{
			if (!ft_strncmp(envp[i], "HOME=", 5))
				return (check_and_change_path(envp, &envp[i][5], token));
			i++;
		}
		return (write(2, "minishell: cd: HOME not set\n", 28), 1);
	}
	else
		return (check_and_change_path(envp, path, token));
	return (0);
}

// check nmb of cmds >> if nmb > 1 it just skip! like in exit()!
// opendir function to check for errors? error after a pipe??
/**
 * @brief Builtin "cd" command.
 *
 * Handles the behaviour of the builtin cd command.
 * Changes the current working directory to a given
 * relative or absolute path.
 * If getcwd fails, it wil lreturn with 0. We don't know why,
 * but bash does it so we do it too. :P
 *
 * @author kfan
 *
 * @param cmd command arguments
 * @param envp environment variables
 * @param token command token
 * @return int
 * @retval success Returns 0 on success, 1 otherwise
 */
int	builtins_cd(char **cmd, char **envp, t_token *token)
{
	DIR	*dir;

	if (cmd[0] && cmd[1])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	if (cmd[0] && !str_equals(cmd[0], "-") && !str_equals(cmd[0], "--"))
	{
		if (cmd[0][0] == '-')
			return (write(2, "minishell: cd: invalid option\n", 30), 125);
		if (cmd[0][0] == '\0')
			return (0);
		dir = opendir(cmd[0]);
		if (!dir)
			return (write(2, "minishell: No such file or directory\n", 37), 1);
		closedir(dir);
	}
	if ((!cmd[0] || str_equals(cmd[0], "--")) && token->nmb_of_cmd == 1)
		return (change_dir(NULL, envp, token));
	else if (token->nmb_of_cmd == 1)
		return (change_dir(cmd[0], envp, token));
	return (0);
}
