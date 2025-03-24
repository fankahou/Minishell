/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_is.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kfan <kfan@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 12:05:24 by kfan              #+#    #+#             */
/*   Updated: 2025/03/21 13:49:33 by kfan             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// return value:
// 1 = $ no needed
// 2 = |
// 3 = <<
// 4 = <
// 5 = >
// 6 = >>
// 7 = ;
// 8 (||) and 9 (&&) for bonus
int is_sym(char c1, char c2)
{
/*     if (c1 == '|')
        return (2); */
    if (c1 == '<')
    {
        if (c2 == '<')
            return (3);
/*         else if (c2 == '>')
            return (1); */
        else
            return (4);
    }
    if (c1 == '>')
    {
        if (c2 == '>')
            return (6);
        else
            return (5);
    }
    return (0);
}

int is_delimiter(char c1, char c2)
{
    if (c1 == ';')
        return (1);
    if (c1 == '&' && c2 == '&')
        return (2);
    if (c1 == '|' && c2 == '|')
        return (3);
    return (0);
}

int is_quote(char c)
{
    if (c == '\'')
        return (1);
    if (c == '\"')
        return (2);
    return (0);
}

int is_space(char c)
{
    if((c >= 9 && c <= 13) || c == ' ')
        return (1);
    return (0);
}

int is_pipe(char c)
{
    if (c == '|')
        return (1);
    return (0);
}
