#include "minishell.h"

// void get_token(t_list **list, char *line);
void add_double_quote(t_list **list, char *line, int *i);


char *ftft(const char *s, unsigned int start, size_t len)
{
	char *substr = (char *)malloc(len + 1);
	strncpy(substr, s + start, len);
	substr[len] = '\0';
	return substr;
}

int	is_space(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == ' ')
			j++;
		i++;
	}
	if ((int)ft_strlen(line) == j)
		return (1);
	return (0);
}

int ft_islogical(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '&' || c == '\n'
		|| c == '\'' || c == '\"' || c == ' ' || c == '\t' || c == '\0')
		return (1);
	if(c == '$' )
		return (3);
	return (0);
}
int help(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '\n'
		|| c == '\'' || c == '\"' || c == ' ' || c == '\t' || c == '\0')
		return (1);
	return (0);
}

int ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (c);
	return (0);
}

void add_ENV(t_list **list, char *line, int *i)
{
	int j;
	char *p;

	j = *i;
	*i = *i + 1;
	while(ft_isalpha(line[*i]))
		*i = *i + 1;
	if(*i > j)
	{
		p = ftft(line, j, *i - j);
		add_token(list, p, ENV);
	}
}

void	get_token(t_list **list,char *line)
{
	int i;
	int j;
	char *p;

	i = 0;

	while(line[i])
	{
		if(ft_islogical(line[i]) == 3 )
		{
			if(!ft_islogical(line[i + 1]))
				add_ENV(list, line, &i);
			else if(line[i + 1] == '$')
			{
				j = i;
				while(!help(line[i]))
					i++;
				p = ftft(line, j, i - j);
				add_token(list, p, WORD);
			}
			else
				add_token(list, "$", WORD);

		}
			if(!ft_islogical(line[i]))
			{
				j = i;
				while(!ft_islogical(line[i]) && line[i] != '\0')
					i++;

				p = ftft(line, j, i - j);
				add_token(list, p, WORD);
			}

				if(line[i] == '|')
					add_token(list, "|", PIPE_LINE);
				if(line[i] == '>')
				{
					if(line[i + 1] == '>')
					{
						add_token(list, ">>", APPEND);
						i++;
					}
					else
						add_token(list, ">", REDIR_OUT);
				}

				if(line[i] == '<')
				{
					if(line[i + 1] == '<')
					{
						add_token(list, "<<", HEARDOC);
						i++;
					}
					else
						add_token(list, "<", REDIR_IN);
				}

				if(line[i] == '\'')
				{
					j = i;
					i++;
					while(line[i] != '\'')
						i++;
					i++;
					p = ftft(line, j, i - j);
					add_token(list, p, WORD);
				}

				if(line[i] == '\"')
				{
					j = i;
					i++;
					while(line[i] != '\"')
						i++;
					i++;
					p = ftft(line, j, i - j);
					add_token(list, p, WORD);
				}

		i++;
	}
}


t_list *ft_lstnew(char *content, enum token type)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->token = type;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}

void add_token(t_list **list, char *token, enum token type)
{
	t_list *new;

	new = ft_lstnew(token, type);
	if(new != NULL)
		ft_lstadd_back(list, new);
}

int ggg(char *line)
{
	int i = 0;

	while(line[i])
	{
		if(line[i] == '|')
			return (1);
		i++;
	}
	return (0);
}


int main(int ac, char **av)
{
	char *line;
	t_list *list;
	t_list *lst;

	list = NULL;
	lst = NULL;
	enum token type;

	int i = 0;
	(void)ac;

	const char *token[] =
	{"WORD","CMD", "WHITE_SPACE", "NEW_LINE", "QUOTE", "DOUBLE_QUOTE", "ENV", "PIPE_LINE", "REDIR_IN", "REDIR_OUT", "AND", "HEARDOC", "APPEND",};
	const char *state[] =
	{"IN_DQUOTE", "IN_SQUOTE", "GENERAL",};



	while (1)
	{
		line = readline("minishell-> ");
			if (line == NULL)
				break ;

			if(valid_line(line) == 0)
			{
				get_token(&list, line);
				syntax_check(&list);
			}




			if(list != NULL)
			{
			if(ggg(line))
				av = ft_split(line, '|');
			else
				av[0] = line;

			i = 0;
			while(av[i])
			{
				ft_lstadd_back(&lst, ft_lstnew(av[i], CMD));
				i++;
			}

			while(lst)
			{
				printf("list->content: %s\n", lst->content);
				printf("list->token: %s\n", token[lst->token]);
				lst = lst->next;
			}
			}
	}
	return 0;
}
