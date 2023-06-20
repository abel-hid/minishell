#include "minishell.h"
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	int		i;
	char	*p;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	p = malloc(sizeof(char) * (len + 1));
	if (!p)
		return (NULL);
	while (*s1)
		p[i++] = *s1++;
	while (*s2)
		p[i++] = *s2++;
	p[i] = '\0';
	return (p);
}

int is_redir(t_list **list)
{
	t_list *tmp;
	tmp = *list;

		if(tmp->token == REDIR_IN ||tmp->token == WORD ||
		tmp->token == REDIR_OUT || tmp->token == APPEND || tmp->token == HEARDOC)
			return (1);

	return (0);
}


void parsing(char **av, char *env, t_list **list)
{
	
}
