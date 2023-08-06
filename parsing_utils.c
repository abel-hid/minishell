/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 21:15:31 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 21:21:41 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_space(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (is_space(line[i]))
			return (1);
		i++;
	}
	return (0);
}

int	more_than_one(char *str)
{
	char	**p;
	int		i;

	p = ft_split1(str, "' ' '\t' '\n' '\v' '\f' '\r'");
	i = 0;
	while (p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
	if (i > 1)
		return (1);
	return (0);
}

int	is_dquote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	norm(char *str, int i)
{
	int	k;

	i++;
	k = 0;
	while (str[i] && is_spaces(str[i]))
	{
		i++;
		k++;
	}
	if (str[i] == '\"')
		i++;
	else
		i -= k + 1;
	return (i);
}

char	*ft_delete(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\"' && str[i + 1] == '\"')
			i += 2;
		else if (str[i] == '\"' && is_spaces(str[i + 1]))
			i = norm(str, i);
		str[j++] = str[i++];
	}
	str[j] = '\0';
	return (str);
}
