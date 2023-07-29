#include "minishell.h"

t_env	*new_env(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

void	lstadd_back(t_env **lst, t_env *new)
{
	t_env	*last_add;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_add = *lst;
	while (last_add->next != NULL)
		last_add = last_add->next;
	last_add->next = new;
	new->next = NULL;
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

void	craete_env(char **env_list, t_env **g_env)
{
	t_env	*new;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	while (env_list[i])
	{
		if (ft_strchr(env_list[i], '='))
		{
			key = ft_substr(env_list[i], 0, ft_strchr(env_list[i], '='));
			value = ft_substr(env_list[i],
					ft_strchr(env_list[i], '=') + 1, ft_strlen(env_list[i]));
			new = new_env(key, value);
			if (new)
				lstadd_back(g_env, new);
		}
		i++;
	}
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env != NULL)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			return (env->value);
		}
		env = env->next;
	}
	return (NULL);
}
