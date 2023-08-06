/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:35:20 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 20:36:09 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_dollar(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '$')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
}

int	handle_dollar(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == c && str[i - 1] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*del_quote(char *str, char c, char c2)
{
	int		i;
	int		k;
	char	c3;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == c || str[i] == c2)
		{
			c3 = str[i++];
			while (str[i])
			{
				if (str[i] == c3)
				{
					i++;
					break ;
				}
				str[k++] = str[i++];
			}
		}
		else
			str[k++] = str[i++];
	}
	return (str[k] = '\0', str);
}
