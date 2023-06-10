#include "minishell.h"

void add_token(t_list **list, char *token, enum token type);
void get_token(t_list **list, char *line);



void get_token( t_list **list , char *line)
{
    int i;
    int j;

    i = 0;

    while (line[i] != '\0')
    {
        if (line[i] == '|')
        {
            add_token(list, "|", PIPE_LINE);
        }
        else if (line[i] == '>')
        {
            add_token(list, ">", REDIR_OUT);
        }
        else if (line[i] == '<')
        {
            add_token(list, "<", REDIR_IN);
        }
        else if (line[i] == '$')
        {
            add_token(list, "$", ENV);
        }
        else if (line[i] == '\'')
        {
            add_token(list, "\'", QOUTE);
        }
        else if (line[i] == '\n')
        {
            add_token(list, "\n", NEW_LINE);
        }
        else if (line[i] == ' ')
        {
            add_token(list, " ", WHITE_SPACE);
        }
        else
        {
            j = i;

            while(line[i] != ' ' && line[i] != '\0')
                i++;
            add_token(list, ft_substr(line, j, i - j), WORD);
            j = 0;
        }
        i++;
    }
}


t_list *ft_lstnew(char *content, enum token type)
{
    t_list *list;

    list = malloc(sizeof(t_list));
    if (!list)
        return (NULL);
    list->content = content;
    list->token = type;
    list->next = NULL;
    return (list);
}

void add_token(t_list **list, char *token, enum token type)
{
    t_list *new;

    new = ft_lstnew(token, type);
    if(new != NULL)
        ft_lstadd_back(list, new);
}

int main(int ac, char **av)
{
    char *line;

    t_list *list;

    list = NULL;

    int i = 0;
    (void)ac;

    while (1)
    {
        line = readline("minishell-> ");
            if (line == NULL)
                break ;
            get_token(&list, line);

            while (list)
            {
                printf("list  '%s'\n", list->content);
                // printf("type %d\n", list->token);
                list = list->next;
            }
    }

    return 0;
}

