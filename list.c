
#include "minishell.h"

void ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *last_add;

	if (*lst == NULL)
	{
		*lst = new;
		return;
	}
	last_add = *lst;
	while (last_add->next != NULL)
		last_add = last_add->next;
	last_add->next = new;
	new->prev = last_add;
	new->next = NULL;
}

t_list	*ft_lstlast(t_list *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

