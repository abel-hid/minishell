/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:02:37 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/05 21:09:50 by abel-hid         ###   ########.fr       */
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
				ft_putendl_fd(" syntax error near unexpected token `|'", 2);
				return (1);
			}
			if (help_token(&tmp->prev) == 0 || help_token(&tmp->next) == 0)
			{
				ft_putendl_fd(" syntax error near unexpected token `|'", 2);
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
				if (tmp->next != NULL && tmp->next->token == REDIR_OUT)
					tmp = tmp->next;
			if (tmp->next == NULL || tmp->next->token != WORD)
			{
				ft_putstr_fd(" syntax error near unexpected token `", 2);
				if (tmp->next != NULL)
					ft_putstr_fd(tmp->next->content, 2);
				else
					ft_putstr_fd("newline", 2);
				ft_putendl_fd("'", 2);
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
				ft_putstr_fd("syntax error near unexpected token `", 2);
				ft_putstr_fd(&str[i - 1], 2);
				ft_putstr_fd("'\n", 2);
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
