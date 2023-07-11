#include "minishell.h"


t_lexer *ft_lstnew(char *content, t_tokens type)
{
	t_lexer *list;

	list = malloc(sizeof(t_lexer));
	if (!list)
		return (NULL);
	list->content = content;
	list->token = type;
	list->next = NULL;
	list->prev = NULL;
	return (list);
}

void free_list(t_lexer *lst)
{
	t_lexer *tmp;

	while(lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
}

int is_qoute(char *line, int i , char c)
{
	int j;

	j = 0;
	if(line[i] == c)
	{
		j++;
		while(line[i + j] && line[i + j] != c)
			j++;
		if(line[i + j] == c)
			j++;
	}
	return (j);
}

void add_token(t_lexer **list, char *line, t_tokens type)
{
	t_lexer *new;

	new = ft_lstnew(line, type);
	if(new != NULL)
		ft_lstadd_back(list, new);
}

int is_space(char c)
{
    return (c == ' ' || c == '\t');
}

int is_quote(char c)
{
    return (c == '\'' || c == '\"');
}

int skip_spaces(char *line, int i)
{
	int j = 0;
	while (line[i + j] && is_space(line[i + j]))
		j++;
	return (j);
}

int is_token(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int is_quote_end(char c, char quote_type)
{
	if (quote_type == '\'')
		return (c == '\'');
	if (quote_type == '\"')
		return (c == '\"');
	return (0);
}

int  get_quote_length(char *line, int i, char quote_type)
{
	int j = 1;
	while (line[i + j] && !is_quote_end(line[i + j], quote_type))
		j++;
	return j;
}

int get_word(char *line, int i, t_lexer **list)
{
	int j = 0;
	char quote_type = 0;
	int quote_length = 0;
	while (line[i + j] && !is_token(line[i + j]) && !is_space(line[i + j]))
	{
		if (is_quote(line[i + j]))
		{
			quote_type = line[i + j];
			quote_length = get_quote_length(line, i + j, quote_type);
			j += quote_length + 1;
		}
		else
			j++;
	}
	add_token(list, strndup(line + i, j), WORD);
	return j;
}

int get_token(char *line, int i, t_lexer **list)
{
	if (line[i] == '|')
	{
		add_token(list, "|", PIPE_LINE);
		return 1;
	}
	else if (line[i] == '<')
	{
		if (line[i + 1] == '<')
		{
			add_token(list, "<<", HEARDOC);
			return (2);
		}
		else
		{
			add_token(list, "<", REDIR_IN);
			return (1);
		}
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
		{
			add_token(list, ">>", APPEND);
			return (2);
		}
		else
		{
			add_token(list, ">", REDIR_OUT);
			return (1);
		}
	}
	return (0);
}

void create_lexer(char *line, t_lexer **list)
{
	int i = 0;
	int token_length = 0;

	while (line[i])
	{
		if (is_space(line[i]))
		{
			i += skip_spaces(line, i);
		}
		else if (is_token(line[i]))
		{
			token_length = get_token(line, i, list);
			i += token_length;
		}
		else
		 {
			token_length = get_word(line, i, list);
			i += token_length;
		}
	}
}


int help_token(t_lexer **list)
{
	t_lexer *tmp;
	tmp = *list;

		if(tmp->token == WORD || tmp->token == REDIR_IN ||
		tmp->token == REDIR_OUT || tmp->token == APPEND || tmp->token == HEARDOC)
			return (1);

	return (0);
}

void status_quote(t_lexer **list)
{
	t_lexer *tmp;
	int i;

	tmp = *list;
	i = 0;
	while(tmp)
	{
		if(tmp->token == WORD)
		{
			while(tmp->content[i])
			{
				if(tmp->content[i] == '\'' || tmp->content[i] == '\"')
				{

				}
				i++;
			}
		}
		tmp = tmp->next;
	}
}

void pipe_line_syntax(t_lexer **list)
{
	t_lexer *tmp;

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

void redir_syntax(t_lexer **list)
{
	t_lexer *tmp;

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
				printf("syntax error near unexpected token `newline'\n");
				*list = NULL;
				return ;
			}
		}
		tmp = tmp->next;
	}
}


int quote_syntax(t_lexer **list)
{
    t_lexer *tmp = *list;
    int i;
	char c;

	while (tmp != NULL)
	{
		if (tmp->token == WORD)
		{
			i = 0;
			while (tmp->content[i] != '\0')
			{
				if (tmp->content[i] == '\'' || tmp->content[i] == '\"')
				{
					c = tmp->content[i];
					i++;
					while (tmp->content[i] != '\0' && tmp->content[i] != c)
						i++;
					if (tmp->content[i] == '\0')
					{
						printf("syntax error near unexpected token `%c'\n", tmp->content[i - 1]);
						*list = NULL;
						return (0);
					}
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

void syntax_check(t_lexer **list)
{
	pipe_line_syntax(list);
	redir_syntax(list);
	quote_syntax(list);
}

void lexing(t_lexer **list, char *line)
{
	create_lexer(line, list);
	syntax_check(list);
}


