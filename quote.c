/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 19:58:06 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 11:45:17 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote_end(char c, char quote_type)
{
	if (quote_type == '\'')
		return (c == '\'');
	if (quote_type == '\"')
		return (c == '\"');
	return (0);
}

int	get_quote_length(char *line, int i, char quote_type)
{
	int	j;

	j = 1;
	while (line[i + j] && !is_quote_end(line[i + j], quote_type))
		j++;
	return (j);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	skip_spaces(char *line, int i)
{
	int	j;

	j = 0;
	while (line[i + j] && is_space(line[i + j]))
		j++;
	return (j);
}
