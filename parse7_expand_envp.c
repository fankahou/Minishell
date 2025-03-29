/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse7_expand_envp.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 10:02:39 by kfan              #+#    #+#             */
/*   Updated: 2025/03/29 13:13:32 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


//expand_envp() and handle " and ';
int check_envp_count(char *temp)
{
    int i;

    i = 0;
    while (temp[i] && temp[i] != '$' && !is_space(temp[i]) && !is_sym(temp[i], 0) && !is_quote(temp[i]) && !is_delimiter(temp[i], '&') && !is_delimiter(temp[i], '|'))
        i++;
    if (i == 0)
        return (0);
    return (i);
}

static int check_sys_envp_count(char *temp)
{
    int i;

    i = 0;
    while (temp[i])
    {
        if (temp[i] == '=')
            return (i);
        i++;
    }
    return (0);
}

static char *expand_exit_code(t_token *token, char *new)
{
    if (new)
        free(new);
    new = ft_itoa(*token->exit_code);
    if (!new)
        return (ft_printf("ft_itoa failed\n"), NULL);
    return (new);
}

char *expand_envp(char *temp, t_token *token, char *new, t_clean *clean)
{
    int i;
    int j;
    
    new = ft_calloc(2, 1);
    if (!new)
        return (perror("ft_calloc failed\n"), NULL);
    i = check_envp_count(temp);
    if (i == 0 && is_quote(temp[i]) && clean->quote == 0)
        return (new);
    else if (i == 0)
        return (new[0] = '$', new);
    else if (temp[0] == '?')
        return (expand_exit_code(token, new));
    else if (temp[0] && !ft_isalpha(temp[0]) && temp[0] != '_' && temp[1])
        return(free(new), ft_strdup(&temp[1]));
    j = 0;
    while (token->envp && token->envp[j])
    {
        if (!ft_strncmp(temp, token->envp[j], check_sys_envp_count(token->envp[j])) && token->envp[j][i] == '=')
            return (free(new), ft_strdup(&token->envp[j][i + 1]));
        j++;
    }
    return (new);
}
