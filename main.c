void get_token(t_list **list, char *line);


char *ftft(const char *s, unsigned int start, size_t len)
{
	char *substr = (char *)malloc(len + 1);
	strncpy(substr, s + start, len);
	substr[len] = '\0';
	return substr;
}
int ft_islogical(char c)
{
	if (c == '|' || c == '>' || c == '<' || c == '$' || c == '&' || c == '\n'
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

void get_token(t_list **list,char *line)
{
	int i;
	int j;
	char *p;

	i = 0;

	while(line[i])
	{
			if(!ft_islogical(line[i]))
			{
				j = i;
				while(!ft_islogical(line[i]) && line[i] != '\0') 
					i++;

				p = ftft(line, j, i - j);
				add_token(list, p, WORD, GENERAL);
			}
				if(line[i] == '|')
					add_token(list, "|", PIPE_LINE, GENERAL);
				if(line[i] == '>')
				{
					p = ft_substr(line, i, ft_strlen(line));
					if(line[i + 1] == '>')
					{
						add_token(list, ">>", APPEND, GENERAL);
						i++;
					}
					else
						add_token(list, ">", REDIR_OUT, GENERAL);
				}
				if(line[i] == '<')
				{
					p = ft_substr(line, i, ft_strlen(line));
					if(line[i + 1] == '<')
					{
						add_token(list, "<<", HEARDOC, GENERAL);
						i++;
					}
					else
						add_token(list, "<", REDIR_IN, GENERAL);
				}
				if(line[i] == '$')
					add_token(list, "$", ENV, GENERAL);
			
				if(line[i] == '\'')
				{
					add_token(list, "\'", QUOTE, IN_SQUOTE);
					i++;
					j = i;
					while(line[i] != '\'')
						i++;
					p = ftft(line, j, i - j);
					add_token(list, p, WORD, IN_SQUOTE);
					add_token(list, "\'", QUOTE, IN_SQUOTE);
				}
				if(line[i] == '\n')
					add_token(list, "\n", NEW_LINE, GENERAL);
				if(line[i] == '&')
					add_token(list, "&", AND, GENERAL);
				if(line[i] == ' ')
					add_token(list, " ", WHITE_SPACE, GENERAL);
				if(line[i] == '\"')
				{
					add_token(list, "\"", DOUBLE_QUOTE, IN_DQUOTE);
					i++;
					j = i;
					while(line[i] != '\"' && line[i] != '\0')
					{
						i++;
					}
					if(line[i] == '\0')
					{
						printf("Error: unclosed double quote\n");
						exit(1);
					}
					if(i > j)
					{
						p = ftft(line, j, i - j);
						add_token(list, p, WORD, IN_DQUOTE);
					}
					if(line[i] == '\"')
						add_token(list, "\"", DOUBLE_QUOTE, IN_DQUOTE);				
				}
					
		i++;
	}
}

t_list *ft_lstnew(char *content, enum token type, enum state state)
{
	t_list *list;

	list = malloc(sizeof(t_list));
	if (!list)
		return (NULL);
	list->content = content;
	list->token = type;
	list->state = state;
	list->next = NULL;
	return (list);
}

void add_token(t_list **list, char *token, enum token type, enum state state)
{
	t_list *new;

	new = ft_lstnew(token, type, state);
	if(new != NULL)
		ft_lstadd_back(list, new);
}

int main(int ac, char **av)
{
	char *line;

	t_list *list;

	list = NULL;
	enum token type;

	int i = 0;
	(void)ac;
	const char *token[] = 
	{"WORD", "WHITE_SPACE", "NEW_LINE", "QUOTE", "DOUBLE_QUOTE", "ENV", "PIPE_LINE", "REDIR_IN", "REDIR_OUT", "AND", "HEARDOC", "APPEND"};
	const char *state[] = {"IN_DQUOTE", "IN_SQUOTE", "GENERAL"};
	while (1)
	{
		line = readline("minishell-> ");
			if (line == NULL)
				break ;
			get_token(&list, line);
			syntax_check(&list);
			while (list)
			{
				printf("----------------------\n");
				printf("content: '%s'\n", list->content);
				printf("token: %s\n", token[list->token]);
				printf("state: %s\n", state[list->state]);
				printf("----------------------\n");
				list = list->next;
			}
	}

	return 0;
}
