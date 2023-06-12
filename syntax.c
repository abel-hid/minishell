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
int ft_isnological(char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
    if (str[i] == '|' || str[i] == '>' || str[i] == '<'  || str[i] == '&'
    || str[i] == '$' || str[i] == '\'' || str[i] == '\"')
            return (1);
        i++;
    }
    return (0);
}
void syntax_check(t_list **list)
{

    t_list *tmp;
    tmp = *list;
    while (tmp != NULL)
    {
        if(ft_isnological(tmp->content) == 1 && tmp->next == NULL)
        {
            printf("syntax error near unexpected token `%s'\n", tmp->content);
            exit(1);
        }

        if(ft_isnological(tmp->content) == 1 && ft_isnological(tmp->next->content) == 1)
        {
            printf("syntax error near unexpected token `%s'\n", tmp->next->content);
            exit(1);
        }

        tmp = tmp->next;
    }

}
