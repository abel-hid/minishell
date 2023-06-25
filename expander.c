#include "minishell.h"
void hh(t_lexer **list);

int is_digit(char c)
{
	return (c >= '0' && c <= '9');
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
		if (strcmp(env->key, key) == 0)
		{
			return env->value;
		}
		env = env->next;
	}
	return NULL;
}
char *delete_dpuote(char *str, char c)
{
	int i;
	int j;
	char *new;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if(!new)
		return (NULL);
	while(str[i])
	{
		if(str[i] == c)
			i++;
		new[j] = str[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
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

int check_quote(char *line, int i , char c)
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
int my(char *line, char c)
{
	int i;
	 i = 0;

	while(line[i])
	{
		if(line[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int if_dollar_out_quote(char *line)
{
	int i;
	if(line[i] == '$' && line[i + 1] == '\"')
	{
		return (1);
	}
	return (0);
}

char *expand_variables(t_lexer **list, t_env **g_env)
{
	t_lexer *tmp = *list;
	char *key;
	char *value;
	char *new;
	char *str;
	char *space;
	int index;

	int   x;

	while (tmp)
	{
		index = 0;
		if(check_quote(tmp->content, index, '\"') || !check_quote(tmp->content, index, '\''))
		{
			x = if_dollar_out_quote(tmp->content);
			tmp->content = delete_dpuote(tmp->content, '\"');
			while (tmp->content[index])
			{
				if (tmp->content[index] == '$' && !x)
				{
					if(is_digit(tmp->content[index + 1]))
					{
						index += 2;
						str = ft_substr(tmp->content, index, ft_strlen(tmp->content));
						new = ft_strjoin(str, "");
						tmp->content = new;
						free(tmp->content);
					}
					else
					{

					int i = index + 1;
					int j = 0;
					while (tmp->content[i] && tmp->content[i] != ' ' && tmp->content[i] != '$')
					{
						j++;
						i++;
					}
					while (tmp->content[i] && tmp->content[i] != ' ')
						i++;

					str = ft_substr(tmp->content, 0, index);
					key = ft_substr(tmp->content, index + 1, j);
					value = get_env_value(*g_env, key);
					space = ft_substr(tmp->content, i, ft_strlen(tmp->content));
					if (value)
					{
						new = ft_strjoin(str, value);
						new = ft_strjoin(new, space);
						free(tmp->content);
						tmp->content = new;
						index += ft_strlen(value);
					}
					else
					{
						new = ft_strjoin(str, "");
						if(space)
						new = ft_strjoin(new, space);
						free(tmp->content);
						tmp->content = new;

					}
					free(key);
					}
				}
				else if(x)
				{
					x = 0;
					new = ft_substr(tmp->content, index + 1, ft_strlen(tmp->content));
					tmp->content = new;
					free(tmp->content);
				}

				index++;
			}
		}
		else
				hh(list);
		tmp = tmp->next;
	}
	return NULL;
}

void expand(t_lexer **list, t_env **g_env)
{
	expand_variables(list, g_env);
	t_lexer *tmp = *list;
	while (tmp)
	{
		if(ft_strncmp(tmp->content, "\"", 4) == 0)
		{
			tmp->content = ft_strdup("");
		}
		tmp = tmp->next;
	}

}

void hh(t_lexer **list)
{

	t_lexer *tmp = *list;
	while (tmp)
	{
		if(ft_strncmp(tmp->content, "''", 4) == 0)
		{
			tmp->content = ft_strdup("");
		}
		if(check_quote(tmp->content, 0, '\''))
			tmp->content = delete_dpuote(tmp->content, '\'');
		tmp = tmp->next;
	}
}
