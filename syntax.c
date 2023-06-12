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
int ft_isnological(char *content)
{
    if(ft_strncmp(content, "|", 1) == 0)
        return (1);
    if(ft_strncmp(content, ">", 1) == 0)
        return (1);
    if(ft_strncmp(content, "<", 1) == 0)
        return (1);
    if(ft_strncmp(content, "$", 1) == 0)
        return (1);
    if(ft_strncmp(content, "&", 1) == 0)
        return (1);
    if(ft_strncmp(content, "<<", 1) == 0)
        return (1);
    if(ft_strncmp(content, ">>", 1) == 0)
        return (1);

    return (0);
}

void check_parentheses(t_list **list)
{
    t_list *tmp;
    int left;
    int right;

    tmp = *list;
    left = 0;
    right = 0;
    while (tmp != NULL)
    {
        if(ft_strncmp(tmp->content, ")", 1) == 0 && tmp->next == NULL)
        {
            printf("syntax error near unexpected token `)'\n");
            exit(1);
        }
        if(ft_strncmp(tmp->content, "(", 1) == 0 && tmp->next == NULL)
        {
            printf("syntax error near unexpected token `)'\n");
            exit(1);
        }
        if(ft_strncmp(tmp->content, "(", 1) == 0 && ft_strncmp(tmp->next->content, ")", 1) == 0)
        {
            printf("syntax error near unexpected token `)'\n");
            exit(1);
        }
        if (ft_strncmp(tmp->content, "(", 1) == 0)
            left++;
        if (ft_strncmp(tmp->content, ")", 1) == 0)
            right++;
        tmp = tmp->next;
    }
    if (left != right)
    {
        printf("syntax error near unexpected token `)'\n");
        exit(1);
    }
}

void syntax_check(t_list **list)
{

    t_list *tmp;
    tmp = *list;
    check_parentheses(list);
    while (tmp != NULL)
    {
        if(ft_isnological(tmp->content) == 1 && tmp->next ==NULL)
        {
                printf("syntax error near unexpected token `%s'\n", tmp->content);
                exit(1);
        }
        if(ft_isnological(tmp->content) == 1 && ft_isnological(tmp->next->content) == 1)
        {
            puts("2");
            printf("syntax error near unexpected token `%s'\n", tmp->next->content);
            exit(1);
        }
        // if(ft_isnological(tmp->content) == 1 && ft_isnological(tmp->next->content) == 0)
        // {
        //     puts("2");
        //     printf("syntax error near unexpected token `%s'\n", tmp->next->content);
        //     exit(1);
        // }

        tmp = tmp->next;
    }

}
