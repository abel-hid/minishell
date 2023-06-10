
#include "minishell.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last_add;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_add = ft_lstlast(*lst);
	last_add->next = new;
}

// t_list	*ft_lstnew(char *content)
// {
// 	t_list	*p;

// 	p = malloc(sizeof(t_list));
// 	if (!p)
// 		return (NULL);
// 	p->content = content;
// 	p->next = NULL;
// 	return (p);
// }

t_list	*ft_lstlast(t_list *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

