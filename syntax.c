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

void check_pipe(t_list **list)
{
	 t_list *tmp;
	 tmp = *list;

	 printf("%s\n", tmp->next->content);

	 while (tmp != NULL)
	 {
		  if(ft_strncmp(tmp->content, "|", 1) == 0 && tmp->next == NULL)
		  {
				printf("syntax error near unexpected token `|'\n");
				exit(1);
		  }
		  if(ft_strncmp(tmp->content, "|", 1) == 0 && ft_strncmp(tmp->next->content, "|", 1) == 0)
		  {
				printf("syntax error near unexpected token `|'\n");
				exit(1);
		  }

		  if(ft_strncmp(tmp->content, "|", 1) == 1 && ft_strncmp(tmp->next->content, "|",1) == 0)
		  {
				printf("syntax error near unexpected token `|'\n");
				exit(1);
		  }
		  tmp = tmp->next;
	 }
}


void syntax_check(t_list **list)
{
	check_pipe(list);

}
