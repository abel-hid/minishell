/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:24:37 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 11:42:05 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_digit(t_lexer *list)
{
	int	i;

	i = 0;
	while (list->content[i])
	{
		if (list->content[i] == '$')
		{
			if (is_digit(list->content[i + 1]))
			{
				return (1);
			}
		}
	}
	return (0);
}

char	*delete_quote(char *str, char c)
{
	int	i;
	int	j;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			j = 0;
			while (str[i + j] == c)
				j++;
			ft_strncpy(&str[i], &str[i + j], ft_strlen(str) - i - j + 1);
		}
		i++;
	}
	return (str);
}

int	check_quote(char *line, char c)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (line[j] == c)
			return (1);
		j++;
	}
	return (0);
}

int	detect_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '$' && str[i + 1] != '$' && str[i + 1] != '\0'
			&& str[i - 1] != '$')
			return (1);
		i++;
	}
	return (0);
}

int	skip_squote(char *str, int i)
{
	int	j;

	j = 0;
	if (str[i] == '\'')
	{
		while (str[i])
		{
			if (str[i] == '\'')
				j++;
			i++;
			if (j % 2 == 0)
				return (i);
		}
	}
	return (i);
}
