/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/03/24 13:23:45 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void syntax_error(char *str, t_token *token)
{
    //ft_printf("minishell: syntax error near unexpected token '%s'\n", str);
    (void)str;
    perror("minishell: syntax error");
    if (token)
    {
        token->exit_code[0] = 2;
        token->error[0] = 1;
    }
}

void open_error(char *str, t_token *token)
{
    //ft_printf("minishell: %s: open failed", str);
    (void)str;
    perror("minishell: open failed");
    if (token)
    {
        token->exit_code[0] = 1;
        token->error[0] = 1;
    }
}
