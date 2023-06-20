#include "minishell.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (!(s1[i] || s2[i]))
		return (0);
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int valid_line(char *line)
{
	int i;
	int j;
	int k;

	i = 0;
	j = 0;
	k = 0;
	while(line[i])
	{
		if(line[i] == '\"')
			j++;
		if(line[i] == '\'')
			k++;
		i++;
	}
	if(j % 2 != 0)
	{
		printf("syntax error near unexpected token `\"'\n");
		return (1) ;
	}
	if(k % 2 != 0)
	{
		printf("syntax error near unexpected token `'\''\n");
		return(1);
	}
	return (0);
}

int help_token(t_list **list)
{
	t_list *tmp;
	tmp = *list;

		if(tmp->token == WORD || tmp->token == REDIR_IN ||
		tmp->token == REDIR_OUT || tmp->token == APPEND || tmp->token == HEARDOC)
			return (1);

	return (0);
}
int isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (c);
	return (0);
}

void free_list(t_list **list)
{
	t_list *tmp;
	t_list *tmp2;

	tmp = *list;
	while(tmp)
	{
		tmp2 = tmp->next;
		free(tmp->content);
		free(tmp);
		tmp = tmp2;
	}
}

void pipe_line_syntax(t_list **list)
{
	t_list *tmp;

	tmp = *list;
	while(tmp)
	{
		if(tmp->token == PIPE_LINE)
		{
			if(tmp->prev == NULL || tmp->next == NULL)
			{
				printf("syntax error near unexpected token `|'\n");
				*list = NULL;
				return ;
			}
			if(help_token(&tmp->prev) == 0 || help_token(&tmp->next) == 0)
			{
				printf("syntax error near unexpected token `|'\n");
				*list = NULL;
				return ;
			}
		}
		tmp = tmp->next;
	}
}
void redir_syntax(t_list **list)
{
	t_list *tmp;

	tmp = *list;

	while(tmp)
	{
		if(tmp->token == REDIR_IN || tmp->token == REDIR_OUT || tmp->token == APPEND || tmp->token == HEARDOC)
		{
			if(tmp->token == REDIR_IN)
			{
				if(tmp->next != NULL && tmp->next->token == REDIR_OUT)
					tmp = tmp->next;
			}
			
			if(tmp->next == NULL || tmp->next->token != WORD)
			{
				printf("syntax error\n");
				*list = NULL;
				return ;
			}
		}
		tmp = tmp->next;
	}
}

void syntax_check(t_list **list)
{
	pipe_line_syntax(list);
	redir_syntax(list);
}
