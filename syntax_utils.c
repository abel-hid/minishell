/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:04:15 by abel-hid          #+#    #+#             */
/*   Updated: 2023/08/06 15:11:18 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quote_syntax(t_lexer **list)
{
	t_lexer	*tmp;

	tmp = *list;
	while (tmp != NULL)
	{
		if (tmp->token == WORD)
		{
			if (quote_error(tmp->content))
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	help_token(t_lexer **list)
{
	t_lexer	*tmp;

	tmp = *list;
	if (tmp->token == WORD || tmp->token == REDIR_IN
		|| tmp->token == REDIR_OUT
		|| tmp->token == APPEND || tmp->token == HEARDOC)
		return (1);
	return (0);
}
