/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abel-hid <abel-hid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:34:34 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/04 20:36:16 by abel-hid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_expand(char *str, t_env **g_env)
{
	int		i;
	char	*p;

	i = 0;
	while (str[i])
	{
		i = skip_squote(str, i);
		if (str[i] == '$' && str[i + 1] == '?')
		{
			p = ft_itoa(exit_st.status);
			str = handler_value(str, &i, 1, p);
			free(p);
			continue ;
		}
		if (str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\''
			&& str[i + 1] != '$' && str[i + 1] != '\0' && !is_spaces(str[i + 1])
			&& str[i + 1] != '\"' )
		{
			str = handler(str, &i, g_env);
			continue ;
		}
		i++;
	}
	return (str);
}

char	*expand_variables(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;

	tmp = *list;
	while (tmp)
	{
		if ((detect_dollar(tmp->content) || check_dollar(tmp->content))
			&& tmp->token == WORD)
			tmp->content = ft_expand(tmp->content, g_env);
		if (tmp->token == APPEND || tmp->token == REDIR_IN
			|| tmp->token == REDIR_OUT || tmp->token == HEARDOC)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (NULL);
}

void	function(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;
	int		i;

	tmp = *list;
	expand_variables(list, g_env);
	while (tmp)
	{
		i = 0;
		while (tmp->content[i])
		{
			if (tmp->content[i] == '$' && tmp->content[i + 1] == '\''
				&& tmp->content[i - 1] != '\'' && tmp->content[i + 2] != '\0' )
			{
				tmp->content = delete_quote(tmp->content, '\'');
				delete_dollar(tmp->content);
			}
			i++;
		}
		tmp = tmp->next;
	}
}

int	quote_check(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		{
			return (1);
		}
		if (line[i] == '\"')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	expand(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;
	int		i;
	int		j;

	tmp = *list;
	j = 0;
	i = 0;
	function(list, g_env);
}
