/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:02:37 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 11:45:28 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_line_syntax(t_lexer **list)
{
	t_lexer	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp->token == PIPE_LINE)
		{
			if (tmp->prev == NULL || tmp->next == NULL)
			{
				printf("syntax error near unexpected token `|'\n");
				return (1);
			}
			if (help_token(&tmp->prev) == 0 || help_token(&tmp->next) == 0)
			{
				printf("syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	redir_syntax(t_lexer **list)
{
	t_lexer	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (tmp->token == REDIR_IN || tmp->token == REDIR_OUT
			|| tmp->token == APPEND || tmp->token == HEARDOC)
		{
			if (tmp->token == REDIR_IN)
			{
				if (tmp->next != NULL && tmp->next->token == REDIR_OUT)
					tmp = tmp->next;
			}
			if (tmp->next == NULL || tmp->next->token != WORD)
			{
				printf("syntax error near unexpected token `newline'\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

int	quote_error(char *str)
{
	int		i;
	char	c;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i];
			i++;
			while (str[i] != '\0' && str[i] != c)
				i++;
			if (str[i] == '\0')
			{
				printf("syntax error near unexpected token `%c'\n", str[i - 1]);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	is_qoute(char *line, int i, char c)
{
	int	j;

	j = 0;
	if (line[i] == c)
	{
		j++;
		while (line[i + j] && line[i + j] != c)
			j++;
		if (line[i + j] == c)
			j++;
	}
	return (j);
}
