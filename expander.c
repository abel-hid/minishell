#include "minishell.h"

int is_digit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	return (0);
}

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void lstadd_back(t_env **lst, t_env *new)
{
	t_env *last_add;

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

int	ft_strchr(char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

t_env *new_env(char *key, char *value)
{
	t_env *new;

	new = malloc(sizeof(t_env));
	if(!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}


void craete_env(char **env_list, t_env **g_env)
{
	t_env *new;
	char *key;
	char *value;

	int i;
	i 	= 0;
	while(env_list[i])
	{
		if(ft_strchr(env_list[i], '='))
		{
			key = ft_substr(env_list[i], 0, ft_strchr(env_list[i], '='));
			value = ft_substr(env_list[i], ft_strchr(env_list[i], '=') + 1, ft_strlen(env_list[i]));
			new = new_env(key, value);
			if(new)
				lstadd_back(g_env, new);
		}
		i++;
	}
}

char *get_env_value(t_env *env, const char *key)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return NULL;
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
			ft_strncpy(&str[i], &str[i + j], ft_strlen(&str[i + j]) + 1);
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

int quote_check(char *line)
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
	while(str[i])
	{
		if(str[i] == '$' && str[i + 1] != '$' && str[i + 1] != '\0' )
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
		new = ft_strjoin(new, ft_substr(str, *i + j + 1, ft_strlen(str)));
		free(str);
		str = new;
		new = NULL;
	}
	return (str);
}

char *handler(char *str, int *i, t_env **g_env)
{
	char *key;
	char *value;
	int j = 0;
	int a = *i + 1;
	while (str[a] && ft_isalnum(str[a]))
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
				if(str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\'' && str[i + 1] != '$' && str[i + 1] != '\0')
				{
					if(str[i + 1] == '\"' && str[i + 2] == '\0')
					{break;}
					else
						str = handler(str, &i, g_env);
				}
				i++;
	}
	return (str);
}

int hh(char *str)
{
	int i = 0;
	int j = 0;
	while (str[i])
	{
		if(str[i] == '$')
			j++;
		i++;
	}
	if(j % 2 == 0)
		return (0);
	return (1);

}

char *expand_variables(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp;

	tmp = *list;
	int i;

	while(tmp)
	{
			i = 0;
			if(check_quote(tmp->content, '\"') || !check_quote(tmp->content, '\''))
			{
				if(detect_dollar(tmp->content) && !if_dollar_out_quote(tmp->content) && hh(tmp->content))
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

void expand(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp;
	int i;
	int j = 0;
	tmp = *list;
	while(tmp)
	{
			tmp->content = ft_delete(tmp->content, '\'');
			tmp->content = ft_delete(tmp->content, '\"');
			i = 0;
			while(tmp->content[i])
			{
				if(tmp->content[i] == '$' && tmp->content[i + 1] == '\'' && tmp->content[i + 2] != '\0' )
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
		tmp = *list;
	while(tmp)
	{
		if(quote_check(tmp->content) != 0)
		{
			if(quote_check(tmp->content) == 1)
			{
				tmp->content = delete_quote(tmp->content, '\'');
			}
			else
			{
				tmp->content = delete_quote(tmp->content, '\"');
			}
		}
		tmp = tmp->next;
	}
}

