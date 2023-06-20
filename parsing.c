#include "minishell.h"

int is_redir(t_list **list)
{
	t_list *tmp;
	tmp = *list;

		if(tmp->token == REDIR_IN ||tmp->token == WORD ||
		tmp->token == REDIR_OUT || tmp->token == APPEND || tmp->token == HEARDOC)
			return (1);

	return (0);
}
void get_list_command(t_list **list)
{
	t_list *tmp;
	t_list *tmp2;
	char *p;
	char *p2;
	int i;
	int j;

	tmp = *list;

	while(tmp)
	{
		if(tmp->token == WORD)
		{
			i = 0;
			j = 0;
			p = tmp->content;
			tmp2 = tmp->next;
			while(tmp2 && tmp2->token == WORD)
			{
				p2 = tmp2->content;
				while(p[i])
					i++;
				while(p2[j])
				{
					p[i] = p2[j];
					i++;
					j++;
				}
				p[i] = '\0';
				tmp2 = tmp2->next;
			}
			tmp->next = tmp2;
		}
		tmp = tmp->next;
	}

}
