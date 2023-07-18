#include "minishell.h"

int is_digit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}


int check_digit(t_lexer *list)
{
	int i;

	i = 0;
	while(list->content[i])
	{
		if(list->content[i] == '$')
		{
			if(is_digit(list->content[i + 1]))
			{
				return (1);
			}
		}
	}
	return (0);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *new;
	int i;
	int j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if(!new)
		return (NULL);
	while(s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while(s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = '\0';
	return (new);
}


int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}



char *delete_quote(char* str, char c)
{
	int i = 0;
	int j ;

	while (str[i])
	{
		if (str[i] == c)
		{
			j = 0;
			while (str[i + j] == c)
				j++;
			ft_strncpy(&str[i], &str[i + j], ft_strlen(str) - i - j + 1);
		}
		i++;
	}
	return (str);
}

int check_value(char *value, char *key)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while(value[i] && key[j])
	{
		if(value[i] == key[j])
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	if(key[j] == '\0')
		return (1);
	return (0);

}

int check_quote(char *line, char c)
{
	int j;

	j = 0;
	while(line[j])
	{
		if(line[j] == c)
			return (1);
		j++;
	}
	return (0);
}




int if_dollar_out_quote(char *line)
{
	int i;
	i = 0;
	while(line[i])
	{
		if(line[i] == '$')
		{
			if(line[i + 1] == '\"' && line[i - 1] == '\"')
				return (1);
		}
		i++;
	}
	return (0);
}

int detect_dollar(char *str)
{
	int i = 0;
	while(str[i] && str[i + 1])
	{
		if(str[i] == '$' && str[i + 1] != '$' && str[i + 1] != '\0' && str[i +1 ] != '?' && str[i -1] != '$')
			return (1);
		i++;
	}
	return (0);
}

char *handler_value(char *str, int *i,int j, char *value)
{
	char *new;
	if (value)
	{
		new = ft_substr(str, 0, *i);
		new = ft_strjoin(new, value);
		new = ft_strjoin(new, ft_substr(str, *i + j + 1, ft_strlen(str)));
		free(str);
		str = new;
		new = NULL;
	}
	else
	{
		new = ft_substr(str, 0, *i);
		new = ft_strjoin(new,ft_strdup(""));
		free(str);
		str = new;
		new = NULL;
	}
	return (str);
}

char * handler(char *str, int *i, t_env **g_env)
{

	char *key;
	char *value;
	int j = 0;
	int a = *i + 1;
	while ((str[a] && ft_isalnum(str[a])) || str[a] == '_')
	{
		j++;
		a++;
	}
	key = ft_substr(str, *i + 1, j);
	value = get_env_value(*g_env, key);
	str = handler_value(str, i, j, value);
	free(key);
	return (str);
}

int is_spaces(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

char *ft_expand(char *str, t_env **g_env)
{
	static char   *new;
	int i;

	i = 0;


	while (str[i])
	{
		if(str[i] == '$' && is_digit(str[i + 1]))
		{
				i += 2;
				new = ft_substr(str, i, ft_strlen(str));
				free(str);
				str = new;
				new = NULL;
		}
		if(str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\'' && str[i + 1] != '$' && str[i + 1] != '\0' && !is_spaces(str[i + 1]) && str[i + 1] != '\"' )
		{
			if(str[i + 1] == '\"' && str[i + 2] == '\0')
			{break;}
			else
				str = handler(str, &i, g_env);
				continue;
		}
		i++;
	}
	return (str);
}

int hh(char *str)
{
	int i = 0;
	int j = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '\"')
			return 1;
		if (str[i] == '$' && (str[i + 1] == '$' || str[i - 1] == '$'))
			j++;
		i++;
	}
	if (j % 2 == 0)
		return 0;
	return 1;
}


char *expand_variables(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp;
		char *new ;
		new = NULL;
	tmp = *list;
	int i;

	while(tmp)
	{
			i = 0;
			if(check_quote(tmp->content, '\"') || !check_quote(tmp->content, '\''))
			{
				if(detect_dollar(tmp->content) || hh(tmp->content))
				{
					tmp->content = ft_expand(tmp->content, g_env);
				}
			}
		tmp = tmp->next;
	}
	return NULL;

}

void delete_dollar(char *str)
{
	int i = 0;
	int j = 0;

	while (str[i])
	{
		if (str[i] != '$')
		{
			str[j] = str[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
}

char *ft_delete(char *str, char c)
{
	int i = 0;
	int j = 0;
	int length = ft_strlen(str);
	char *new = malloc(length + 1);

	if (new == NULL)
		return NULL;

	while (i < length)
	{
		if (str[i] == c && str[i + 1] == c)
		{
			i++;
		}
		else
		{
			new[j] = str[i];
			j++;
		}
		i++;
	}
	new[j] = '\0';
	return new;
}



int handle_dollar(char* str, char c)
{
	int i = 0;

	while(str[i])
	{
		if(str[i] == '$' && str[i + 1] == c && str[i - 1]  == c)
			return (1);
		i++;
	}
	return (0);
}
void fun1(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp;
	int i;
	int j;

	tmp = *list;
	i = 0;
	j = 0;

	while(tmp)
	{
			tmp->content = ft_delete(tmp->content, '\'');
			if(!detect_dollar(tmp->content))
				tmp->content = ft_delete(tmp->content, '\"');
			i = 0;
			while(tmp->content[i])
			{
				if(tmp->content[i] == '$' && tmp->content[i + 1] == '\'' && tmp->content[i - 1] != '\'' && tmp->content[i + 2] != '\0' )
				{
					j = 1;
					tmp->content = delete_quote(tmp->content, '\'');
					delete_dollar(tmp->content);
				}
				i++;
			}
		tmp = tmp->next;
	}
		if(j == 0)
			expand_variables(list, g_env);
}

int      quote_check(char *line)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while(line[i])
	{
		if(line[i] == '\'')
		{
			return (1);
		}
		if(line[i] == '\"')
		{
			return (2);
		}
		i++;
	}
	return (0);
}


char *del_quote(char *str, char c, char c2)
{
	int i = 0;
	int k = 0;

	while (str[i])
	{
		if (str[i] == c)
		{
			i++;

			while (str[i])
			{
				if (str[i] == c)
				{
					i++;
					break;
				}
				str[k++] = str[i++];
			}
		}

		if (str[i] == c2)
		{
			i++;
			while (str[i])
			{
				if (str[i] == c2)
				{
					i++;
					break;
				}
				str[k++] = str[i++];
			}
		}
		str[k++] = str[i++];
	}
	str[k] = '\0';
	return str;
}


void expand(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp;
	int i;
	int j ;

	tmp = *list;
	j = 0;
	i = 0;
		fun1(list, g_env);
	while(tmp)
	{
		tmp->content = ft_delete(tmp->content, '\"');
		if(quote_check(tmp->content) != 0)
		{
			if(quote_check(tmp->content) == 1)
			{

				tmp->content = del_quote(tmp->content, '\'', '\"');
			}
			else
			{
				tmp->content = del_quote(tmp->content, '\"', '\'');
			}
		}
		tmp = tmp->next;
	}
}

