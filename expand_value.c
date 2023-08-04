/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_value.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:30:52 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 20:32:25 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_value(char *value, char *key)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (value[i] && key[j])
	{
		if (value[i] == key[j])
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	if (key[j] == '\0')
		return (1);
	return (0);
}

char	*handler_value(char *str, int *i, int j, char *value)
{
	char	*new;
	char	*s;

	if (value)
	{
		new = ft_substr(str, 0, *i);
		new = my_strjoin(new, value);
		s = ft_substr(str, *i + j + 1, ft_strlen(str));
		new = my_strjoin(new, s);
		free(str);
		str = new;
		new = NULL;
	}
	else
	{
		new = ft_substr(str, 0, *i);
		new = my_strjoin(new, "");
		s = ft_substr(str, *i + j + 1, ft_strlen(str));
		new = my_strjoin(new, s);
		free(str);
		str = new;
		new = NULL;
	}
	return (free(s), str);
}

char	*handler(char *str, int *i, t_env **g_env)
{
	char	*key;
	char	*value;
	int		j;
	int		a;

	a = *i + 1;
	j = 0;
	while ((str[a] && ft_isalnum(str[a])) || str[a] == '_')
	{
		j++;
		a++;
	}
	key = ft_substr(str, *i + 1, j);
	value = get_env_value(*g_env, key);
	str = handler_value(str, i, j, value);
	free(key);
	return (str);
}

int	check_dollar(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '\"')
			return (1);
		if (str[i] == '$' && (str[i + 1] == '$' || str[i - 1] == '$'))
			j++;
		i++;
	}
	if (j % 2 == 0)
		return (0);
	return (1);
}

int	is_spaces(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}
