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

void valid_line(char *line)
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
		exit(1);
	}
	if(k % 2 != 0)
	{
		printf("syntax error near unexpected token `'\''\n");
		exit(1);
	}
}


// int help(char c)
// {
// 	if (c == '|' || c == '>' || c == '<' || c == '\n'
// 		|| c == '\'' || c == '\"' || c == ' ' || c == '\t' || c == '\0')
// 		return (1);
// 	return (0);
// }
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
void check_all(t_list **list )
{

	t_list *tmp;
	tmp = *list;

	while (tmp)
	{
		if(!help_token(&tmp) && (tmp->next == NULL ||tmp->prev ==NULL))
		{
			printf("syntax error");
			exit(1);
		}
		tmp = tmp->next;
	}
}
// void check_redirection_syntax(t_list **list)

void syntax_check(t_list **list)
{
	// check_pipe(list);
	check_all(list);
	// check_redirection_syntax(list);
}
