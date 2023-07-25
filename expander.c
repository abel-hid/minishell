#include "minishell.h"

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	check_digit(t_lexer *list)
{
	int	i;

	i = 0;
	while (list->content[i])
	{
		if (list->content[i] == '$')
		{
			if (is_digit(list->content[i + 1]))
			{
				return (1);
			}
		}
	}
	return (0);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j])
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

char	*delete_quote(char *str, char c)
{
	int	i;
	int	j;

	i = 0;
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

int	check_value(char *value, char *key)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (value[i] && key[j])
	{
		if (value[i] == key[j])
		{
			i++;
			j++;
		}
		else
			return (0);
	}
	if (key[j] == '\0')
		return (1);
	return (0);
}

int	check_quote(char *line, char c)
{
	int	j;

	j = 0;
	while (line[j])
	{
		if (line[j] == c)
			return (1);
		j++;
	}
	return (0);
}

int	if_dollar_out_quote(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			if (line[i + 1] == '\"' && line[i - 1] == '\"')
				return (1);
		}
		i++;
	}
	return (0);
}

int	detect_dollar(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '$' && str[i + 1] != '$' && str[i + 1] != '\0'
			&& str[i + 1] != '?' && str[i - 1] != '$')
			return (1);
		i++;
	}
	return (0);
}

char	*my_strjoin(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = '\0';
	free(s1);
	return (new);
}

char	*handler_value(char *str, int *i, int j, char *value)
{
	char	*new;
	char	*s;

	if (value)
	{
		new = ft_substr(str, 0, *i);
		new = my_strjoin(new, value);
		s = ft_substr(str, *i + j + 1, ft_strlen(str));
		new = my_strjoin(new, s);
		free(str);
		str = new;
		new = NULL;
	}
	else
	{
		new = ft_substr(str, 0, *i);
		new = my_strjoin(new, "");
		s = ft_substr(str, *i + j + 1, ft_strlen(str));
		new = my_strjoin(new, s);
		free(str);
		str = new;
		new = NULL;
	}
	return (free(s), str);
}

char	*handler(char *str, int *i, t_env **g_env)
{
	char	*key;
	char	*value;
	int		j;
	int		a;

	a = *i + 1;
	j = 0;
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

int	is_spaces(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	hehe(char *str, int i)
{
	int	j;

	j = 0;
	if (str[i] == '\'')
	{
		while (str[i])
		{
			if (str[i] == '\'')
				j++;
				i++;
			if (j % 2 == 0)
				return (i);
		}
	}
	return (i);
}

char	*ft_expand(char *str, t_env **g_env)
{
	static char	*new;
	int			i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && is_digit(str[i + 1]))
		{
			i += 2;
			new = ft_substr(str, i, ft_strlen(str));
			free(str);
			str = new;
			new = NULL;
		}
		i = hehe(str, i);
		if (str[i] == '$' && !is_digit(str[i + 1]) && str[i + 1] != '\''
			&& str[i + 1] != '$' && str[i + 1] != '\0' && !is_spaces(str[i + 1])
			&& str[i + 1] != '\"' )
		{
			str = handler(str, &i, g_env);
			continue ;
		}
		i++;
	}
	return (str);
}

int	hh(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '\"')
			return (1);
		if (str[i] == '$' && (str[i + 1] == '$' || str[i - 1] == '$'))
			j++;
		i++;
	}
	if (j % 2 == 0)
		return (0);
	return (1);
}

char	*expand_variables(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;

	tmp = *list;
	while (tmp)
	{
		if (detect_dollar(tmp->content) || hh(tmp->content))
			tmp->content = ft_expand(tmp->content, g_env);
		tmp = tmp->next;
	}
	return (NULL);
}

void	delete_dollar(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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

int	handle_dollar(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == c && str[i - 1] == c)
			return (1);
		i++;
	}
	return (0);
}

void	function(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;
	int		i;

	tmp = *list;
	expand_variables(list, g_env);
	while (tmp)
	{
		i = 0;
		while (tmp->content[i])
		{
			if (tmp->content[i] == '$' && tmp->content[i + 1] == '\''
				&& tmp->content[i - 1] != '\'' && tmp->content[i + 2] != '\0' )
			{
				tmp->content = delete_quote(tmp->content, '\'');
				delete_dollar(tmp->content);
			}
			i++;
		}
		tmp = tmp->next;
	}
}

int	quote_check(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '\'')
		{
			return (1);
		}
		if (line[i] == '\"')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

char	*del_quote(char *str, char c, char c2)
{
	int		i;
	int		k;
	char	c3;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == c || str[i] == c2)
		{
			c3 = str[i];
			i++;
			while (str[i])
			{
				if (str[i] == c3)
				{
					i++;
					break;
				}
				str[k++] = str[i++];
			}
		}
		else
			str[k++] = str[i++];
	}
	str[k] = '\0';
	return (str);
}


void	expand(t_lexer **list, t_env **g_env)
{
	t_lexer	*tmp;
	int		i;
	int		j;

	tmp = *list;
	j = 0;
	i = 0;
	function(list, g_env);
}

