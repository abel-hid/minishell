/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 13:43:49 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 13:46:11 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lexer_list(t_lexer **lst)
{
	t_lexer	*tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		if ((*lst)->token == WORD)
		{
			free((*lst)->content);
		}
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	free_args(char **s)
{
	int	i;

	i = 0;
	while (s[i] != NULL)
	{
		free(s[i]);
		i++;
	}
	free (s);
}

void	free_cmd_list(t_command **lst)
{
	t_command	*tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free_args((*lst)->args);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	free_env(t_env **lst)
{
	t_env	*tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		if ((*lst)->key != NULL)
			free((*lst)->key);
		if ((*lst)->value != NULL)
			free((*lst)->value);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
