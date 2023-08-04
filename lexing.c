/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:11:23 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 20:13:13 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *src, size_t n)
{
	char	*dst;
	size_t	i;

	i = 0;
	dst = malloc(sizeof(char) * (n + 1));
	if (!dst)
		return (NULL);
	while (src[i] && i < n)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int	get_word(char *line, int i, t_lexer **list)
{
	int		j;
	char	*str;
	char	quote_type;
	int		quote_length;

	quote_type = 0;
	quote_length = 0;
	j = 0;
	while (line[i + j] && !is_token(line[i + j]) && !is_space(line[i + j]))
	{
		if (is_quote(line[i + j]))
		{
			quote_type = line[i + j];
			quote_length = get_quote_length(line, i + j, quote_type);
			j += quote_length + 1;
		}
		else
			j++;
	}
	str = ft_strndup(line + i, j);
	add_token(list, str, WORD);
	return (j);
}

int	add_redir_out(char *line, int i, t_lexer **list)
{
	if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(list, ">>", APPEND);
			return (2);
		}
		else
		{
			add_token(list, ">", REDIR_OUT);
			return (1);
		}
	}
	return (0);
}

int	syntax_check(t_lexer **list)
{
	if (pipe_line_syntax(list))
		return (1);
	if (quote_syntax(list))
		return (1);
	if (redir_syntax(list))
		return (1);
	return (0);
}

int	lexing(t_lexer **list, char *line)
{
	create_lexer(line, list);
	if (syntax_check(list))
		return (1);
	return (0);
}
