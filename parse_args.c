/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:35:14 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 21:39:05 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_args(t_lexer *tmp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tmp != NULL && tmp->token != PIPE_LINE)
	{
		if (tmp->token == WORD)
		{
			j = count_strings(tmp->content, " \t\n\v\f\r");
			i += j;
		}
		else if (tmp->token == REDIR_IN || tmp->token == REDIR_OUT
			|| tmp->token == APPEND || tmp->token == HEARDOC)
		{
			tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (i);
}


int	parse_env(t_env **g_env, char *str)
{
	t_env	*env;

	env = *g_env;
	while (env)
	{
		env->value = del_quote(env->value, '\'', '\"');
		if (ft_strcmp(env->value, str) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

char	*add_args(char *str)
{
	str = del_quote(str, '\'', '\"');
	str = ft_strdup(str);
	return (str);
}

char	**get_p(char *str)
{
	char	**p;

	str = del_quote(str, '\'', '\"');
	p = ft_split1(str, "' ' '\t' '\n' '\v' '\f' '\r'");
	return (p);
}

int	is_env(t_env **g_env, char *str)
{
	t_env	*env;

	env = *g_env;
	while (env)
	{
		if (ft_strcmp(env->value, str) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}
