/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 19:58:47 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 20:16:45 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_lstnew(char *content, t_tokens type)
{
	t_lexer	*list;

	list = malloc(sizeof(t_lexer));
	if (!list)
		return (NULL);
	list->content = content;
	list->token = type;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}

void	add_token(t_lexer **list, char *line, t_tokens type)
{
	t_lexer	*new;

	new = ft_lstnew(line, type);
	if (new != NULL)
		ft_lstadd_back(list, new);
}

int	is_token(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	create_lexer(char *line, t_lexer **list)
{
	int	i;
	int	token_length;

	i = 0;
	token_length = 0;
	while (line[i])
	{
		if (is_space(line[i]))
		{
			i += skip_spaces(line, i);
		}
		else if (is_token(line[i]))
		{
			token_length = get_token(line, i, list);
			i += token_length;
		}
		else
		{
			token_length = get_word(line, i, list);
			i += token_length;
		}
	}
}

int	get_token(char *line, int i, t_lexer **list)
{
	if (line[i] == '|')
	{
		add_token(list, "|", PIPE_LINE);
		return (1);
	}
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(list, "<<", HEARDOC);
			return (2);
		}
		else
		{
			add_token(list, "<", REDIR_IN);
			return (1);
		}
	}
	else
		return (add_redir_out(line, i, list));
	return (0);
}
