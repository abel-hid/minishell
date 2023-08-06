/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heddahbi <heddahbi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/06 11:44:04 by heddahbi          #+#    #+#             */
/*   Updated: 2023/08/06 11:44:05 by heddahbi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_lstlast(t_lexer *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_lexer **lst, t_lexer *new)
{
	t_lexer	*last_add;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_add = *lst;
	while (last_add->next != NULL)
		last_add = last_add->next;
	last_add->next = new;
	new->prev = last_add;
	new->next = NULL;
}

int	ft_strncmp( char *s1, char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((s1[i] && s2[i]) && (s1[i] == s2[i]) && (i < n - 1))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
