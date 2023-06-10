#include "minishell.h"

void add_token(t_list **list, char *token, enum token type);
void get_token(t_list **list, char *line);


char *ftft(const char *s, unsigned int start, size_t len)
{
    char *substr = (char *)malloc(len + 1);
    strncpy(substr, s + start, len);
    substr[len] = '\0';
    return substr;
}

// char *first_token(char *line)
// {
//     int i;
//     char *token;

//     i = 0;
//     while (line[i] != '\0' && (line[i] != ' '))
//         i++;
//     token = malloc(sizeof(char) * (i + 1));
//     i = 0;
//     while (line[i] != '\0' && line[i] != ' ')
//     {
//         token[i] = line[i];
//         i++;
//     }
//     token[i] = '\0';
//     return (token);
// }

// char **get_line(char *line)
// {
//     char **tab;
//     int i;
//     int j;
//     int k;

//     i = 0;
//     j = 0;
//     k = 0;
//     tab = malloc(sizeof(char *) * (ft_strlen(line) + 1));
//     while (line[i] != '\0')
//     {
//         if (line[i] == ' ')
//         {
//             tab[j] = malloc(sizeof(char) * (i + 1));
//             while (k < i)
//             {
//                 tab[j][k] = line[k];
//                 k++;
//             }
//             tab[j][k] = '\0';
//             j++;
//             k = 0;
//         }
//         i++;
//     }
//     tab[j] = malloc(sizeof(char) * (i + 1));
//     while (k < i)
//     {
//         tab[j][k] = line[k];
//         k++;
//     }
//     tab[j][k] = '\0';
//     return (tab);
// }
// int ft_isalpha(int c)
// {
//     if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
//         return (c);
//     return (0);
// }


// char *get_rest(char *line)
// {
//     char *rest;
//     rest = ft_substr(line, ft_strlen(first_token(line)), ft_strlen(line));
//     return (rest);
// }


void get_token( t_list **list , char *line)
{
    int i;
    int j;
    int state;
    state =  1;
    i = 0;

    while (line[i] != '\0')
    {
        if(line[i] == '"')
           state = 0;
    
        else
            state = 1;
        
        if(state ==1)
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

                if(line[i] == ' ')
                    add_token(list, " ", WHITE_SPACE);
            }
        }
        else
        {
            if (line[i] == '"')
            {
                add_token(list, "\"", DOUBLE_QUOTE);
            }
            else
            {
                j = i;

                while(line[i] != '"' && line[i] != '\0')
                    i++;
                add_token(list, ft_substr(line, j, i - j), WORD);

                if(line[i] == '"')
                    add_token(list, "\"", DOUBLE_QUOTE);
            }
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
